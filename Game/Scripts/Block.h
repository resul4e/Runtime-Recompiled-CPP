#pragma once
#include "Core/Object.h"
#include "Transform/Vector.h"

class Block : public Object
{
public:
	Block(){};
	virtual ~Block(){};

	virtual void Start() override;
	virtual void Setup(glm::vec2 aPos) {};
	virtual void Restart() override;
	virtual void FixedUpdate() override;
	virtual void Update(float aDeltatime) override;
	virtual void Delete() override;
	virtual void Serialize(Storage& storage) override;

	virtual void HitBlock();
	virtual PhysicsCompHandle GetPhysicsComponent();
	void SetColliderIndex(size_t index);
	size_t GetColliderIndex();
	void SetBlockIndex(size_t index);
	size_t GetBlockIndex();
	glm::vec2 GetPosition();

	size_t floorColliderIndex;
	size_t blockIndex;

	AnimationCompHandle block;
	PhysicsCompHandle blockCollider;

	glm::vec2 position;
	glm::vec2 startPos;

	
};

INTERFACE_START(Block)
	PUBLIC_FUNCTION(HitBlock, Block)
	PUBLIC_FUNCTION(GetPhysicsComponent,Block)
	PUBLIC_FUNCTION(SetColliderIndex, Block)
	PUBLIC_FUNCTION(GetColliderIndex, Block)
	PUBLIC_FUNCTION(SetBlockIndex, Block)
	PUBLIC_FUNCTION(GetBlockIndex, Block)
	PUBLIC_FUNCTION(GetPosition, Block)
	PUBLIC_FUNCTION(Setup, Block)
INTERFACE_END()