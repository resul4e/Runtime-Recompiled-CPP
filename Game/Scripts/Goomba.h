#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


#include "Core/Object.h"
#include "Transform/Vector.h"

class Goomba : public Object
{
public:
	Goomba();
	virtual ~Goomba();

	virtual void Start() override;
	virtual void Restart() override;
	virtual void FixedUpdate() override;
	virtual void Update(float aDeltatime) override;
	virtual void Delete() override;
	virtual void Serialize(Storage& storage) override;

	void Setup(glm::vec2 aPos, std::vector<PhysicsCompHandle> aCollidables);
	PhysicsCompHandle GetCollider();

	bool first;
	bool isDead;
	bool isDying;
	float deathTimer;

	//activation
	void ShouldActivate();

	bool activated;
	float activationRange;

	//physics
	PhysicsCompHandle enemyCollider;
	AnimationCompHandle enemyAnim;
	glm::vec2 movement = glm::vec2(-1,0);

	//mario
	ObjectHandle marioObject;

	//environment
	std::vector<PhysicsCompHandle> collidables;
};

INTERFACE_START(Goomba)
PUBLIC_FUNCTION(Setup, Goomba);
PUBLIC_FUNCTION(GetCollider, Goomba);
INTERFACE_END()
