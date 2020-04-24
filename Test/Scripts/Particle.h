#pragma once
#include "Core/Level.h"
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

	void SetObjectHandle(ObjectHandle _me);
	void SetParticles(std::vector<ObjectHandle> _particles);
	float GetRadius();
	glm::vec2& GetPosition();
	glm::vec2& GetVelocity();

	glm::vec2 m_gravity;
	glm::vec2 m_velocity;
	glm::vec2 m_position;
	
	RenderCompHandle m_renderHandle;

	int count;
	static int counter;
	std::vector<ObjectHandle> m_particles;
	float m_radius;
	ObjectHandle me;
};

INTERFACE_START(Particle)
	PUBLIC_FUNCTION(SetParticles, Particle)
	PUBLIC_FUNCTION(GetRadius, Particle)
	PUBLIC_FUNCTION(SetObjectHandle, Particle)
	PUBLIC_FUNCTION(GetPosition, Particle)
	PUBLIC_FUNCTION(GetVelocity, Particle)
INTERFACE_END()
