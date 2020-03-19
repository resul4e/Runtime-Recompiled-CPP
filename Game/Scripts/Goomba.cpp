#include "Goomba.h"
#include "Physics/PhysicsComponent.h"
#include "Rendering/AnimationComponent.h"
#include "Core/Level.h"
#include "Core/Storage.h"
#include "Mario.h"

SCRIPT_START(Goomba)

Goomba::Goomba()
{
}

Goomba::~Goomba()
{
}

void Goomba::Start()
{
	//Enemy::Start();
	activated = false;
	activationRange = 256;
	marioObject = lvl->GetObjectWithName("Mario");

	enemyCollider = PhysicsComponent::AddDynamic(glm::vec2(6.72, 3.68), glm::vec2(0.16, 0.16),0, 1,1,0.5f, 1);
	PhysicsComponent::SetFilter(enemyCollider, 0xFFFD);
	
	enemyAnim = AnimationComponent::AddSprite(160);
	AnimationComponent::AddAnimation("GoombaWalking", "..\\Game\\Assets\\Sprites\\NES - Super Mario Bros - Enemies.png");
	AnimationComponent::AddAnimationFrame("GoombaWalking", glm::vec2(0, 32), glm::vec2(32, 32));
	AnimationComponent::AddAnimationFrame("GoombaWalking", glm::vec2(32, 32), glm::vec2(32, 32));

	AnimationComponent::AddAnimation("GoombaDead", "..\\Game\\Assets\\Sprites\\NES - Super Mario Bros - Enemies.png");
	AnimationComponent::AddAnimationFrame("GoombaDead", glm::vec2(64, 32), glm::vec2(32, 32));

	AnimationComponent::SetAnimation(enemyAnim, "GoombaWalking");
	AnimationComponent::SetOrigin(enemyAnim, glm::vec2(16, 16));

	first = true;
	isDead = false;
	isDying = false;
	deathTimer = 0;
}

void Goomba::Setup(glm::vec2 aPos, std::vector<PhysicsCompHandle> aCollidables)
{
	collidables = aCollidables;
	for(int i=0; i<aCollidables.size(); i++)
	{
		PhysicsComponent::RegisterCollisionPair(aCollidables.at(i), enemyCollider);
	}

	PhysicsComponent::SetPosition(enemyCollider, aPos / 100.f);
}

PhysicsCompHandle Goomba::GetCollider()
{
	return enemyCollider;
}

void Goomba::Restart()
{
}

void Goomba::FixedUpdate()
{
}

void Goomba::Update(float aDeltatime)
{
	//if(first)
	//{
	//	PhysicsComponent::RegisterCollisionPair(MarioInterface::GetPhysicsComponent(marioObject), enemyCollider);
	//	first = false;
	//}

	if(isDying)
	{
		deathTimer += aDeltatime;
		if(deathTimer > 1)
		{
			AnimationComponent::RemoveSprite(enemyAnim);
			isDying = false;
		}
	}

	if(!isDead)
	{
		AnimationComponent::SetPosition(enemyAnim, PhysicsComponent::GetPosition(enemyCollider)*100.f);
		ShouldActivate();
		if (activated)
		{
			for (int i = 0; i < collidables.size(); i++)
			{
				if (PhysicsComponent::GetAreColliding(collidables.at(i), enemyCollider))
				{
					if(PhysicsComponent::GetContactNormal(collidables.at(i), enemyCollider).x < 0)
					{
						movement = glm::vec2(-1, 0);
					}
					else
					{
						movement = glm::vec2(1, 0);
					}
					//PhysicsComponent::SetPosition(enemyCollider, PhysicsComponent::GetPosition(enemyCollider) + movement);
				}
			}

			PhysicsComponent::SetLinearVelocity(enemyCollider, glm::vec2(movement.x,PhysicsComponent::GetLinearVelocity(enemyCollider).y));
		}

		if (PhysicsComponent::GetAreColliding(MarioInterface::GetPhysicsComponent(marioObject), enemyCollider))
		{
			if (PhysicsComponent::GetContactNormal(MarioInterface::GetPhysicsComponent(marioObject), enemyCollider).y > 0)
			{
				PhysicsComponent::RemovePhysicsBody(enemyCollider);
				AnimationComponent::SetAnimation(enemyAnim, "GoombaDead");
				isDead = true;
				isDying = true;
			}
			else
			{
				MarioInterface::Kill(lvl->GetObjectWithName("Mario"));
			}
		}
	}
}

void Goomba::Delete()
{
}

void Goomba::Serialize(Storage& storage)
{
	storage.Serialize(activated, marioObject, enemyAnim, enemyCollider, first, movement);
}

void Goomba::ShouldActivate()
{
	float dist = abs((MarioInterface::GetGlobalPosition(marioObject) - AnimationComponent::GetPosition(enemyAnim)).x);
	if(dist < activationRange)
	{
		activated = true;
	}
}

SCRIPT_END(Goomba)
