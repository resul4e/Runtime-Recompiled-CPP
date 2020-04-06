#include "Particle.h"
#include "Core/Storage.h"
#include "Rendering/SpriteComponent.h"
#include "Rendering/RenderComponent.h"
#include <random>

SCRIPT_START(Particle)

void Particle::Start()
{
	m_gravity = glm::vec2(0, 9.81);
	m_position = glm::vec2(rand() % 40, rand() % 20);
	m_renderHandle = RenderComponent::AddCircle(10);
}

void Particle::Restart()
{
	
}

void Particle::FixedUpdate()
{
}

void Particle::Update(float aDeltatime)
{
	m_velocity += m_gravity * aDeltatime;
	m_position += m_velocity * aDeltatime;

	if(m_position.y > 42.5)
	{
		m_velocity.y *= -1;
	}
	
	RenderComponent::SetPosition(m_renderHandle, m_position * 10.f);
}

void Particle::Delete()
{
}

void Particle::Serialize(Storage& storage)
{ 
	storage.Serialize(m_gravity, m_velocity, m_position);   
}


SCRIPT_END(Particle)


