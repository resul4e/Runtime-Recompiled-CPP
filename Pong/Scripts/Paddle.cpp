#include "Paddle.h"
#include "Core/Storage.h"
#include "Core/Level.h"

#include "Rendering/RenderComponent.h"
#include <Physics/PhysicsComponent.h>
#include "Input/InputComponent.h"
#include "Ball.h"

SCRIPT_START(Paddle)

Paddle::Paddle()
{
}


Paddle::~Paddle()
{
}

void Paddle::Start()
{
	paddleShape = RenderComponent::AddBox(9, 50);
	RenderComponent::SetOrigin(paddleShape, glm::vec2(9, 50));
	
	velocity = 0;
	first = true;
}

void Paddle::Restart()
{
}

void Paddle::FixedUpdate()
{
}

void Paddle::Update(float aDeltatime)
{
	if(first)
	{
		ballCollider = BallInterface::GetPhysicsComponent(lvl->GetObjectWithName("TheBall"));
		PhysicsComponent::RegisterCollisionPair(paddleCollider, ballCollider);
	}
	
	if(PhysicsComponent::GetAreColliding(paddleCollider, ballCollider))
	{
		BallInterface::PlayRandomHitSound(lvl->GetObjectWithName("TheBall"));
	}
	
	RenderComponent::SetPosition(paddleShape, PhysicsComponent::GetPosition(paddleCollider)*100.f);
	
	PhysicsComponent::SetLinearVelocity(paddleCollider, glm::vec2(0,velocity));
	velocity = 0;
}

void Paddle::Delete()
{
}

void Paddle::Serialize(Storage& storage)
{
	storage.Serialize(paddleShape, paddleCollider, anchor, velocity, ballCollider,first );
}

void Paddle::Setup(float StartPosX)
{
	paddleCollider = PhysicsComponent::AddDynamic(glm::vec2(StartPosX, 1.74f), glm::vec2(0.09f, 0.50f), 0, 0, 1, 1, 0, false);
	anchor = PhysicsComponent::AddStatic(glm::vec2(10, -100), glm::vec2(1, 1));
	
	PhysicsComponent::PrismaticJoint(paddleCollider, anchor, glm::vec2(0, 1), glm::vec2(-2.7f, 2.7f));
}

void Paddle::MoveUp()
{
	velocity -= MOVE_SPEED;
}

void Paddle::MoveDown()
{
	velocity += MOVE_SPEED;
}

glm::vec2 Paddle::GetPhysicsPosition()
{
	//return glm::vec2(-1, -1);
	return PhysicsComponent::GetPosition(paddleCollider);
}

SCRIPT_END(Paddle)
