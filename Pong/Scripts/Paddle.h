#pragma once
#include <Core/Object.h>
#include "Transform/Vector.h"

class Paddle : public Object
{
public:
	Paddle();
	~Paddle();

	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;

	//PUBLIC
	void Setup(float StartPosX);
	void MoveUp();
	void MoveDown();
	glm::vec2 GetPhysicsPosition();

	//Rendering
	RenderCompHandle paddleShape;

	//physics
	PhysicsCompHandle paddleCollider;
	PhysicsCompHandle anchor;
	float velocity;
	const float MOVE_SPEED = 2;
	
	//ball
	PhysicsCompHandle ballCollider;
	bool first;

};

INTERFACE_START(Paddle)
PUBLIC_FUNCTION(MoveUp, Paddle);
PUBLIC_FUNCTION(MoveDown, Paddle);
PUBLIC_FUNCTION(GetPhysicsPosition, Paddle);
PUBLIC_FUNCTION(Setup, Paddle);
INTERFACE_END()