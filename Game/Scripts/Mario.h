#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "Core/Object.h"
#include "Transform/Vector.h"

class Mario : public Object
{
public:
	Mario();
	~Mario();

	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;

	glm::vec2 GetPositionOnScreen();
	glm::vec2 GetGlobalPosition();
	PhysicsCompHandle GetPhysicsComponent();
	void PowerUp();
	void Kill();

	//animation
	AnimationCompHandle mario;
	bool poweredUp = false;
	bool transitioning = false;
	float transitionTimer = 0;
	float transitionDoneTimer = 0;
	const float TRANSITION_TIME = 1;

	//movement
	glm::vec2 marioVel;
	const double MARIO_ACC = 5;
	const double MARIO_MAX_VEL = 2.5;
	//const double MARIO_MAX_VEL = 5.5;
	float gravity = 1500.f;

	//jumping
	float jumpTimer = 0;
	float MAX_JUMP_TIME = 0.29f;
	bool isJumping = false;
	bool onGround = false;
	bool spaceReleased = false;
	AudioCompHandle jumpingSmallSound;
	AudioCompHandle jumpingPoweredSound;

	//powerUp
	AudioCompHandle powerUpSound;
	AudioCompHandle powerDownSound;
	bool isTransitionPoweredUp;
	glm::vec2 powerUpColliderPos;

	//physics
	PhysicsCompHandle marioCollider;
	glm::vec2 marioColliderSize = { 0.10f, 0.32f };

	//death
	AudioCompHandle dieSound;
	bool isDead;

private:
	std::string SelectAnimation(float aDeltatime);
	void AddAnimations();
};


INTERFACE_START(Mario)
	PUBLIC_FUNCTION(GetPositionOnScreen, Mario)
	PUBLIC_FUNCTION(GetGlobalPosition, Mario)
	PUBLIC_FUNCTION(GetPhysicsComponent, Mario)
	PUBLIC_FUNCTION(PowerUp, Mario)
	PUBLIC_FUNCTION(Kill, Mario)
INTERFACE_END()