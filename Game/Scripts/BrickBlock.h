#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


#include "Core/Object.h"
#include "Transform/Vector.h"
#include "Block.h"

struct DestroyedBlock
{
	std::vector<AnimationCompHandle> anim;
	std::vector<PhysicsCompHandle> collider;
};

class BrickBlock : public Block
{
public:
	BrickBlock(){};
	virtual ~BrickBlock(){};

	virtual void Start() override;
	void Setup(glm::vec2 aPos) override;
	virtual void Restart() override;
	virtual void FixedUpdate() override;
	virtual void Update(float aDeltatime) override;
	virtual void Delete() override;
	virtual void Serialize(Storage& storage) override;

	virtual void HitBlock() override;
	void SetPoweredUp(bool aMarioPoweredUp);;

	SpriteCompHandle brickBlock;
	AudioCompHandle bumpSound;
	DestroyedBlock destroyedBlocks;

	bool marioPoweredUp;
	bool canBeHit;
	bool destroyed;
	bool removedCollider;
};

INTERFACE_START(BrickBlock)
PUBLIC_FUNCTION(SetPoweredUp, BrickBlock)
PUBLIC_FUNCTION(GetPosition, BrickBlock)
INTERFACE_END()