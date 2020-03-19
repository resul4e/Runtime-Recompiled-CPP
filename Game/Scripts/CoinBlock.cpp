#include "CoinBlock.h"
#include "Rendering/SpriteComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Rendering/AnimationComponent.h"
#include "Core/Level.h"
#include "Core/Storage.h"
#include "Audio/AudioComponent.h"
#include "Mario.h"
#include "Background.h"

SCRIPT_START(CoinBlock)

void CoinBlock::Start()
{

}

void CoinBlock::Setup(glm::vec2 aPos)
{
	position = aPos;

	//block
	block = AnimationComponent::AddSprite(180);
	AnimationComponent::AddAnimation("Flashing", "..\\Game\\Assets\\Sprites\\NES - Super Mario Bros - Item and Brick Blocks.png");
	AnimationComponent::AddAnimationFrame("Flashing", glm::ivec2(160, 224), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("Flashing", glm::ivec2(192, 224), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("Flashing", glm::ivec2(224, 224), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("Flashing", glm::ivec2(160, 224), glm::ivec2(32, 32));

	AnimationComponent::AddAnimation("Hit", "..\\Game\\Assets\\Sprites\\NES - Super Mario Bros - Item and Brick Blocks.png");
	AnimationComponent::AddAnimationFrame("Hit", glm::ivec2(256, 224), glm::ivec2(32, 32));

	AnimationComponent::SetAnimation(block, "Flashing");
	AnimationComponent::SetPosition(block, aPos);
	AnimationComponent::SetOrigin(block, glm::vec2(16, 16));

	PhysicsCompHandle anchor = PhysicsComponent::AddStatic(glm::vec2(5.28, -3.f), glm::vec2(0.16, 0.16));
	blockCollider = PhysicsComponent::AddDynamic(((aPos+glm::vec2(16,27))/100.f), glm::vec2(0.10, 0.05), 0, 0.5f, 1, 0, 0.07f);
	PhysicsComponent::PrismaticJoint(anchor, blockCollider, glm::vec2(0, 1), glm::vec2(-0.1f, 0.f));

	//coin
	prizeAnim = AnimationComponent::AddSprite(50);
	AnimationComponent::AddAnimation("CoinRotating", "..\\Game\\Assets\\Sprites\\NES - Super Mario Bros - Items & Objects.png");
	AnimationComponent::AddAnimationFrame("CoinRotating", glm::ivec2(0, 224), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("CoinRotating", glm::ivec2(32, 224), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("CoinRotating", glm::ivec2(64, 224), glm::ivec2(32, 32));
	AnimationComponent::AddAnimationFrame("CoinRotating", glm::ivec2(96, 224), glm::ivec2(32, 32));

	AnimationComponent::AddAnimation("Mushroom", "..\\Game\\Assets\\Sprites\\NES - Super Mario Bros - Items & Objects.png");
	AnimationComponent::AddAnimationFrame("Mushroom", glm::ivec2(0, 0), glm::ivec2(32, 32));

	AnimationComponent::SetPosition(prizeAnim, glm::vec2(aPos.x, aPos.y - 32));
	AnimationComponent::SetOrigin(prizeAnim, glm::vec2(16, 16));
	if(prize == Coin)
	{
		prizeSound = AudioComponent::AddSound("..\\Game\\Assets\\Audio\\smb\\sound effects\\smb_coin.wav");
	}
	else
	{
		prizeSound = AudioComponent::AddSound("..\\Game\\Assets\\Audio\\smb\\sound effects\\smb_powerup_appears.wav");
	}

}

void CoinBlock::Restart()
{
}

void CoinBlock::FixedUpdate()
{
}

void CoinBlock::Update(float aDeltatime)
{
	glm::vec2 pos = PhysicsComponent::GetPosition(blockCollider);
	AnimationComponent::SetPosition(block, (pos*100.f)-glm::vec2(0,11));
	if(prize == Coin)
	{
		if (coinblockHit && !isPrizeGone)
		{
			glm::vec2 prizePos = PhysicsComponent::GetPosition(prizePhysics);
			AnimationComponent::SetPosition(prizeAnim, prizePos*100.f);
			if (isCoinUp && prizePos.y > (position.y - 64) / 100.f)
			{
				PhysicsComponent::RemovePhysicsBody(prizePhysics);
				AnimationComponent::RemoveSprite(prizeAnim);
				isPrizeGone = true;
			}
			if (prizePos.y < (position.y - 64) / 100.f && !isCoinUp)
			{
				isCoinUp = true;
			}
		}
	}
	else if(prize== Mushroom)
	{
		if (coinblockHit && !isPrizeGone)
		{
			glm::vec2 prizePos = PhysicsComponent::GetPosition(prizePhysics);
			AnimationComponent::SetPosition(prizeAnim, prizePos*100.f);

			if(PhysicsComponent::GetAreColliding(backgroundPipe1,prizePhysics))
			{
				PhysicsComponent::SetLinearVelocity(prizePhysics, glm::vec2(-1, 0));
			}

			if (PhysicsComponent::GetAreColliding(MarioInterface::GetPhysicsComponent(lvl->GetObjectWithName("Mario")), prizePhysics))
			{
				PhysicsComponent::RemovePhysicsBody(prizePhysics);
				AnimationComponent::RemoveSprite(prizeAnim);
				MarioInterface::PowerUp(lvl->GetObjectWithName("Mario"));
				isPrizeGone = true;
			}
		}
	}
	if (coinblockHit && pos.y >= ((position + glm::vec2(16, 27)) / 100.f).y && !madeStatic)
	{
		PhysicsComponent::RemovePhysicsBody(blockCollider);
		blockCollider = PhysicsComponent::AddStatic((position + glm::vec2(16, 27)) / 100.f, glm::vec2(0.10, 0.05));
		madeStatic = true;
	}
}

void CoinBlock::Delete()
{
}

void CoinBlock::Serialize(Storage& storage)
{
	storage.Serialize(blockCollider, block, prizePhysics, prizeAnim, prizeSound, position, backgroundPipe1);
}

void CoinBlock::HitBlock()
{
	if(!coinblockHit)
	{
		AnimationComponent::SetAnimation(block, "Hit");
		AudioComponent::Play(prizeSound);
		if(prize == Coin)
		{
			prizePhysics = PhysicsComponent::AddDynamic((glm::vec2(position.x, position.y - 32) + glm::vec2(16)) / 100.f, glm::vec2(0.16f, 0.16f),0,1,1,0,0,true);
			PhysicsComponent::SetLinearVelocity(prizePhysics, glm::vec2(0, -7));
			AnimationComponent::SetAnimation(prizeAnim, "CoinRotating");
		}
		else
		{
			backgroundPipe1 = BackgroundInterface::GetPipe(lvl->GetObjectWithName("Background"), 0);
			prizePhysics = PhysicsComponent::AddDynamic((glm::vec2(position.x, position.y - 32) + glm::vec2(16)) / 100.f, glm::vec2(0.16f, 0.16f),0,1,1,0,0);
			PhysicsComponent::RegisterCollisionPair(backgroundPipe1, prizePhysics);
			PhysicsComponent::RegisterCollisionPair(MarioInterface::GetPhysicsComponent(lvl->GetObjectWithName("Mario")), prizePhysics);
			PhysicsComponent::SetLinearVelocity(prizePhysics, glm::vec2(1, 0));
			AnimationComponent::SetAnimation(prizeAnim, "Mushroom");
		}

		AnimationComponent::SetPosition(prizeAnim, PhysicsComponent::GetPosition(prizePhysics)*100.f);

		coinblockHit = true;
	}
}

SCRIPT_END(CoinBlock)
