#pragma once
#include "Core/Object.h"

class AiPlayer :  public Object
{
public:
	AiPlayer();
	~AiPlayer();
	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;

	//paddle
	ObjectHandle aiPaddle;
	ObjectHandle ballHandle;
};

INTERFACE_START(AiPlayer)
INTERFACE_END()