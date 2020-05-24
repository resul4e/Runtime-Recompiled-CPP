#pragma once
#include "Block.h"
#include "Core/Object.h"
#include "Transform/Vector.h"

class CoinBlock : public Block
{
public:
	enum Prize
	{
		Coin,
		Mushroom
	};


	CoinBlock(){};
	virtual ~CoinBlock(){};

	virtual void Start() override;
	virtual void Restart() override;
	virtual void FixedUpdate() override;
	virtual void Update(float aDeltatime) override;
	virtual void Delete() override;
	virtual void Serialize(Storage& storage) override;

	virtual void HitBlock() override;
	void SetPrize(Prize aPrize) { prize = aPrize; };

	void Setup(glm::vec2 aPos) override;
	AudioCompHandle prizeSound;
	AnimationCompHandle prizeAnim;
	PhysicsCompHandle prizePhysics;
	PhysicsCompHandle backgroundPipe1;

	Prize prize;

	bool coinblockHit = false;
	bool madeStatic = false;
	bool isCoinUp = false;
	bool isPrizeGone = false;
};

INTERFACE_START(CoinBlock)
	PUBLIC_FUNCTION(SetPrize, CoinBlock)
INTERFACE_END()