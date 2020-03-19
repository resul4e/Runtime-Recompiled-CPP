#include "Ball.h"
#include "Core/Storage.h"

#include <Rendering/RenderComponent.h>
#include "Physics/PhysicsComponent.h"
#include "Audio/AudioComponent.h"


SCRIPT_START(Ball)

Ball::Ball()
{
}


Ball::~Ball()
{
}

void Ball::Start()
{
	canPlay = true;
	ballShape = RenderComponent::AddCircle(ballRadius);
	RenderComponent::SetOrigin(ballShape, glm::vec2(ballRadius));
	ballCollider = PhysicsComponent::AddDynamic(glm::vec2(2.4f, 2.4f), glm::vec2(ballRadius / 100.f, ballRadius / 100.f), 0, 0, 1, 1, 0, false);
	PhysicsComponent::SetLinearVelocity(ballCollider, glm::vec2(-startVelocity, startVelocity));
	
	for (auto p : directory_iterator("../Pong/Audio"))
	{
		if (p.path().extension() == ".wav")
		{
			pongAudioList.push_back(AudioComponent::AddSound(p.path().string().c_str()));
		}
	}
}

void Ball::Restart()
{
}

void Ball::FixedUpdate()
{

}

void Ball::Update(float aDeltatime)
{
	RenderComponent::SetPosition(ballShape, PhysicsComponent::GetPosition(ballCollider)*100.f);
	
	if(!canPlay && canPlayTimer < 0.032)
	{
		canPlayTimer += aDeltatime;
	}
	else
	{
		canPlay = true;
	}
}

void Ball::Delete()
{
}

void Ball::Serialize(Storage& storage)
{
	storage.Serialize(ballShape, ballCollider, pongAudioList, canPlayTimer, canPlay);
}

glm::vec2 Ball::GetPosition()
{
	//return glm::vec2(-1, -1);
	return PhysicsComponent::GetPosition(ballCollider);
}

void Ball::PlayRandomHitSound()
{
	if(canPlay)
	{
		int index = rand() % (pongAudioList.size() - 1);
		AudioComponent::Play(pongAudioList.at(index));
		canPlay = false;
		canPlayTimer = 0;
	}

}

PhysicsCompHandle Ball::GetPhysicsComponent()
{
	//return { 0 };
	return ballCollider;
}

SCRIPT_END(Ball)
