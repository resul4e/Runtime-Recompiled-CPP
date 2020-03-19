#pragma once
#include "Core/Object.h"
#include "Transform/Vector.h"


class Enemy : public Object
{
public:
	Enemy();
	virtual ~Enemy(){};

	virtual void Start() override;
	virtual void Restart() override;
	virtual void FixedUpdate() override;
	virtual void Update(float aDeltatime) override;
	virtual void Delete() override;
	virtual void Serialize(Storage& storage) override;
};

INTERFACE_START(Enemy)
INTERFACE_END()