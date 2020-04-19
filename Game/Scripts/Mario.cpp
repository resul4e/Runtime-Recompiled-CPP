#include "Mario.h"
#include <iostream>
#include "Rendering/AnimationComponent.h"
#include "Rendering/SpriteComponent.h"
#include "Input/InputComponent.h"
#include "Core/Storage.h"
#include "Core/Logger.h"

#include "Background.h"
#include "Core/Level.h"
#include "Audio/AudioComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Rendering/RenderEngineAPI.h"
#include "Rendering/RenderComponent.h"
#include "Rendering/WindowComponent.h"
#include "Game.h"

SCRIPT_START(Mario)

Mario::Mario()
{
}

Mario::~Mario()
{
}

void Mario::Start()
{

	marioVel = glm::vec2(0, 0);

	//animiation
	AddAnimations();

	AnimationComponent::SetPosition(mario, glm::vec2(0, 0));
	glm::vec2 dynPos(1.04, 0);

	if(poweredUp)
	{
		AnimationComponent::SetOrigin(mario, glm::vec2(16, 32));
	}
	else
	{
		marioColliderSize = glm::vec2( 0.08f,0.16f );
		AnimationComponent::SetOrigin(mario, glm::vec2(16, 16));
	}
	marioCollider = PhysicsComponent::AddDynamic(dynPos, marioColliderSize, 0, 1, 1, 0, 0.05f);

	//jumping sound
	jumpingSmallSound = AudioComponent::AddSound("Assets/Audio/smb/sound effects/smb_jump-small.wav");
	jumpingPoweredSound = AudioComponent::AddSound("Assets/Audio/smb/sound effects/smb_jump-super.wav");
	//powerup sound
	powerUpSound = AudioComponent::AddSound("Assets/Audio/smb/sound effects/smb_powerup.wav");
	powerDownSound = AudioComponent::AddSound("Assets/Audio/smb/sound effects/smb_pipe.wav");
	//death sound
	dieSound = AudioComponent::AddSound("Assets/Audio/smb/sound effects/smb_mariodie.wav");

	isDead = false;
	transitioning = false;
	transitionDoneTimer = TRANSITION_TIME+1;
	poweredUp = false;
}

void Mario::Restart()
{
}

void Mario::FixedUpdate()
{
}

void Mario::Update(float aDeltatime)
{
	marioVel = PhysicsComponent::GetLinearVelocity(marioCollider);

	if(isDead)
	{
		AnimationComponent::SetPosition(mario, PhysicsComponent::GetPosition(marioCollider)*100.f);
		PhysicsComponent::SetLinearVelocity(marioCollider, glm::vec2(0,marioVel.y));
		AnimationComponent::SetAnimation(mario, "MarioDeath");
		return;
	}

	if(transitioning && transitionDoneTimer < TRANSITION_TIME)
	{
		AnimationComponent::SetColor(mario, glm::tvec4<char>(255, 255, 255, 128));
		if(transitionTimer > 0.2)
		{
			std::string animatioToPlay = "Idle";
			if(isTransitionPoweredUp)
			{
				animatioToPlay += " small";
				isTransitionPoweredUp = false;
			}
			else
			{
				isTransitionPoweredUp = true;
			}
			AnimationComponent::SetAnimation(mario,animatioToPlay.c_str());
			transitionTimer = transitionTimer - 0.2f;
		}
		transitionTimer += aDeltatime;
		transitionDoneTimer += aDeltatime;
		if(poweredUp)
		{
			return;
		}

	}

	if(transitionDoneTimer >= TRANSITION_TIME && transitioning)
	{
		AnimationComponent::SetColor(mario, glm::tvec4<char>(255, 255, 255, 255));
		PhysicsComponent::SetPosition(marioCollider, AnimationComponent::GetPosition(mario)/100.f);
		marioVel.y = 0.1f;
		transitioning = false;
		GameInterface::RegisterMario(lvl->GetObjectWithName("Game"));
		PhysicsComponent::SetCategory(marioCollider, 0x0001);
	}

	if(PhysicsComponent::RayCast(marioCollider,glm::vec2(-0.08f,0), glm::vec2(0, 1), 0.34f) ||
		PhysicsComponent::RayCast(marioCollider, glm::vec2(0, 0), glm::vec2(0, 1), 0.34f) ||
		PhysicsComponent::RayCast(marioCollider, glm::vec2(0.08f, 0), glm::vec2(0, 1), 0.34f))
	{
		onGround = true;
		if (spaceReleased)
		{
			jumpTimer = 0;
		}
	}

	AnimationComponent::SetPosition(mario, PhysicsComponent::GetPosition(marioCollider)*100.f);
	
	if (InputComponent::GetKeyPressed(InputComponent::Key::D))
	{
		if (marioVel.x < MARIO_MAX_VEL)
		{
			marioVel.x += MARIO_ACC*aDeltatime;
		}
	}
	else
	{
		if (marioVel.x > 0)
		{
			marioVel.x -= (MARIO_ACC)/2.f * aDeltatime;
		}
	}

	if (InputComponent::GetKeyPressed(InputComponent::Key::A))
	{
		if (marioVel.x > -MARIO_MAX_VEL)
		{
			marioVel.x -= MARIO_ACC*aDeltatime;
		}
	}
	else
	{
		if (marioVel.x < 0)
		{
			marioVel.x += (MARIO_ACC) / 2.f * aDeltatime;
		}
	}

	if (InputComponent::GetKeyPressed(InputComponent::Key::Space))
	{
		if (jumpTimer < MAX_JUMP_TIME)
		{
			if (onGround)
			{
				if(poweredUp)
				{
					AudioComponent::Play(jumpingPoweredSound);
				}
				else
				{
					AudioComponent::Play(jumpingSmallSound);
				}

			}
			marioVel.y = -4;
			//marioVel.y = -8;
			jumpTimer += aDeltatime;
			isJumping = true;
			onGround = false;
		}
		else
		{
			isJumping = false;
		}
		spaceReleased = false;
	}
	else
	{
		jumpTimer = MAX_JUMP_TIME;
		spaceReleased = true;
		isJumping = false;
	}

	std::string animationToPlay = SelectAnimation(aDeltatime);
	if (AnimationComponent::GetCurrentAnimation(mario) != animationToPlay)
	{
		AnimationComponent::SetAnimation(mario, animationToPlay.c_str());
	}

	if(GetPositionOnScreen().y > 448)
	{
		Kill();
	}

	if (GetPositionOnScreen().x < 16 && !InputComponent::GetKeyPressed(InputComponent::Key::D))
	{
		marioVel.x = 0;
	}

	PhysicsComponent::SetLinearVelocity(marioCollider, marioVel);
}

void Mario::Delete()
{
}

void Mario::Serialize(Storage& storage)
{
	storage.Serialize(mario, marioVel, gravity, jumpingPoweredSound, jumpingSmallSound, powerUpSound,  marioCollider , onGround, poweredUp, transitionTimer, transitionDoneTimer, name, isDead);
}

glm::vec2 Mario::GetPositionOnScreen()
{
	return AnimationComponent::GetScreenPosition(mario);
}

glm::vec2 Mario::GetGlobalPosition()
{
	return AnimationComponent::GetPosition(mario);
}

PhysicsCompHandle Mario::GetPhysicsComponent()
{
	return marioCollider;
}

void Mario::PowerUp()
{
	powerUpColliderPos = PhysicsComponent::GetPosition(marioCollider);
	poweredUp = true;
	AnimationComponent::SetOrigin(mario, glm::vec2(16, 32));
	AudioComponent::Play(powerUpSound);
	PhysicsComponent::RemovePhysicsBody(marioCollider);
	marioCollider = PhysicsComponent::AddDynamic(powerUpColliderPos, { 0.10f, 0.32f }, 0, 1, 1, 0, 0.05f);
	GameInterface::RegisterMario(lvl->GetObjectWithName("Game"));
	GameInterface::SetPoweredUp(lvl->GetObjectWithName("Game"), true);
	transitioning = true;
	transitionDoneTimer = 0;
	transitionTimer = 0;
}

void Mario::Kill()
{
	if(transitionDoneTimer < TRANSITION_TIME)
	{
		return;
	}

	if(!poweredUp || GetPositionOnScreen().y > 448)
	{
		glm::vec2 deathPos = PhysicsComponent::GetPosition(marioCollider);
		PhysicsComponent::RemovePhysicsBody(marioCollider);
		marioCollider = PhysicsComponent::AddDynamic(deathPos, marioColliderSize, 0, 1, 1, 0, 0, true);
		PhysicsComponent::SetLinearVelocity(marioCollider, glm::vec2(0, -8));
		AudioComponent::Play(dieSound);
		isDead = true;
		GameInterface::StopMusic(lvl->GetObjectWithName("Game"));
		return;
	}

	powerUpColliderPos = PhysicsComponent::GetPosition(marioCollider);
	poweredUp = false;
	AnimationComponent::SetOrigin(mario, glm::vec2(16, 16));
	AudioComponent::Play(powerDownSound);
	PhysicsComponent::RemovePhysicsBody(marioCollider);
	GameInterface::SetPoweredUp(lvl->GetObjectWithName("Game"), false);
	marioCollider = PhysicsComponent::AddDynamic(powerUpColliderPos, marioColliderSize, 0, 1, 1, 0, 0.05f);
	transitioning = true;
	transitionDoneTimer = 0;
	transitionTimer = 0;
	PhysicsComponent::SetCategory(marioCollider, 0x0002);
}

std::string Mario::SelectAnimation(float aDeltatime)
{
	std::string addition = " small";
	if(poweredUp)
	{
		addition = "";
	}
	if (!onGround)
	{
		return "Jumping"+ addition;
	}
	else if (abs(marioVel.x) < MARIO_ACC * aDeltatime)
	{
		marioVel.x = 0;
		return "Idle" + addition;
	}
	else if (marioVel.x > 0 && !InputComponent::GetKeyPressed(InputComponent::Key::A))
	{
		AnimationComponent::SetScale(mario, glm::vec2(1, 1));
		return "Walking" + addition;
	}
	else if (marioVel.x > 0 && InputComponent::GetKeyPressed(InputComponent::Key::A))
	{
		AnimationComponent::SetScale(mario, glm::vec2(-1, 1));
		return "Slide" + addition;
	}
	else if (marioVel.x < 0 && !InputComponent::GetKeyPressed(InputComponent::Key::D))
	{
		AnimationComponent::SetScale(mario, glm::vec2(-1, 1));
		return "Walking" + addition;
	}
	else if (marioVel.x < 0 && InputComponent::GetKeyPressed(InputComponent::Key::D))
	{
		AnimationComponent::SetScale(mario, glm::vec2(1, 1));
		return "Slide" + addition;
	}
	else
	{
		return "Nothing";
	}
}

void Mario::AddAnimations()
{
	mario = AnimationComponent::AddSprite(80);
	AnimationComponent::AddAnimation("Walking", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Walking", glm::ivec2(194, 3/*68*/), glm::ivec2(32, 64/*32*/));
	AnimationComponent::AddAnimationFrame("Walking", glm::ivec2(228, 3/*68*/), glm::ivec2(32, 64/*32*/));
	AnimationComponent::AddAnimationFrame("Walking", glm::ivec2(262, 3/*68*/), glm::ivec2(32, 64/*32*/));

	AnimationComponent::AddAnimation("Walking small", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Walking small", glm::ivec2(194, 68), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("Walking small", glm::ivec2(228, 68), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("Walking small", glm::ivec2(262, 68), glm::ivec2(32, 32));

	AnimationComponent::AddAnimation("Idle", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Idle", glm::ivec2(160, 3), glm::ivec2(32, 64));

	AnimationComponent::AddAnimation("Idle small", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Idle small", glm::ivec2(160, 68), glm::ivec2(32, 32));

	AnimationComponent::AddAnimation("Slide", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Slide", glm::ivec2(296, 3), glm::ivec2(32, 64));

	AnimationComponent::AddAnimation("Slide small", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Slide small", glm::ivec2(296, 68), glm::ivec2(32, 32));

	AnimationComponent::AddAnimation("Jumping", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Jumping", glm::ivec2(330, 3), glm::ivec2(32, 64));

	AnimationComponent::AddAnimation("Jumping small", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("Jumping small", glm::ivec2(330, 68), glm::ivec2(32, 32));

	AnimationComponent::AddAnimation("MarioDeath", "../Game/Assets/Sprites/NES - Super Mario Bros - Mario & Luigi (1).png");
	AnimationComponent::AddAnimationFrame("MarioDeath", glm::ivec2(364, 68), glm::ivec2(32, 32));
}

SCRIPT_END(Mario)
