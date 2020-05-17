#pragma once
#include "Core/Level.h"
#include "Core/Object.h"

class TestScript : public Object
{
public:
	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;
	std::string TestFunction() { return "PROOF_OF_TEST_SCRIPT"; }
};

INTERFACE_START(TestScript)
INTERFACE_END()
