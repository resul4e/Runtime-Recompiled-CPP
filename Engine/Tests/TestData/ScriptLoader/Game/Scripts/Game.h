#pragma once
#include "Core/Object.h"

class Game : public Object
{
public:
	Game();
	~Game();

	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;
};

INTERFACE_START(Game)
INTERFACE_END()