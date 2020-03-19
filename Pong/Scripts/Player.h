#pragma once
#include "Core/Object.h"

class Player : public Object
{
public:
	Player();
	~Player();

	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;

	//paddle
	ObjectHandle playerPaddle;
};

INTERFACE_START(Player)
INTERFACE_END()
