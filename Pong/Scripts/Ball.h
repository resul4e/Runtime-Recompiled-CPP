#pragma once
#include <Core/Object.h>
#include "Transform/Vector.h"

class Ball : public Object
{
public:
	Ball();
	~Ball();

	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;

	//PUBLIC
	glm::vec2 GetPosition();
	PhysicsCompHandle GetPhysicsComponent();
	void PlayRandomHitSound();

	//consts
	const float ballRadius = 12;
	const float startVelocity = 1.20;

	//rendering
	RenderCompHandle ballShape;

	//physics
	PhysicsCompHandle ballCollider;

	//audio
	std::vector<AudioCompHandle> pongAudioList;
	bool canPlay;
	float canPlayTimer;
};

INTERFACE_START(Ball)
PUBLIC_FUNCTION(GetPosition, Ball);
PUBLIC_FUNCTION(GetPhysicsComponent, Ball);
PUBLIC_FUNCTION(PlayRandomHitSound, Ball);
INTERFACE_END()