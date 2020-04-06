#pragma once
#include "Core/Object.h"
#include "glm/glm.hpp"

class Particle : public Object
{
public:
	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;

	glm::vec2 m_gravity;
	glm::vec2 m_velocity;
	glm::vec2 m_position;
	
	RenderCompHandle m_renderHandle;
};

INTERFACE_START(Particle)
INTERFACE_END()
