#include "Particle.h"
#include "Core/Storage.h"

#include <random>
#include <iostream>

#include "Rendering/SpriteComponent.h"
#include "Rendering/RenderComponent.h"
#include "Transform/Vector.h"

SCRIPT_START(Particle)

int Particle::counter = 0;

void Particle::Start()
{
	count = counter;
	counter++;
	m_gravity = glm::vec2(0, 9.81);
	m_position = glm::vec2(rand() % 4000 / 100.f, rand() % 2000 / 100.f);
	m_renderHandle = RenderComponent::AddCircle(2);
	m_radius = 0.2;
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

	RenderComponent::SetFillColor(m_renderHandle, 255, 255, 255, 255);
	for(auto part : m_particles)
	{
		if(part.index <= me.index)
		{
			continue;
		}
	
		glm::vec2 diff = m_position - ParticleInterface::GetPosition(part);
		float mag = glm::length((m_position - ParticleInterface::GetPosition(part)));
		float radii = m_radius + ParticleInterface::GetRadius(part);
		if(mag < radii)
		{
			glm::vec2 normal = glm::normalize(diff);

			if(mag < 0.0001f)
			{
				m_position -= glm::vec2(0, 1) * radii;
			}

			float pen = radii - mag;
			
			m_position += normal * pen * 0.5f;
			ParticleInterface::GetPosition(part) -= normal * pen * 0.5f;

			auto j = ((1 + 0.5f) * glm::dot(m_velocity - ParticleInterface::GetVelocity(part), normal)) / 2.f;
			m_velocity -= j * normal;
			ParticleInterface::GetVelocity(part) += j * normal;
			
			RenderComponent::SetFillColor(m_renderHandle, 255, 0, 0, 255);
		}
	}
	
	if(m_position.y > 42.5)
	{
		m_position.y = 42.5;
		m_velocity.y *= -0.5;
	}
	if (m_position.x > 42.5)
	{
		m_position.x = 42.5;
		m_velocity.x *= -0.5;
	}
	else if (m_position.x < 0.2)
	{
		m_position.x = 0.2;
		m_velocity.x *= -0.5;
	}
	
	RenderComponent::SetPosition(m_renderHandle, m_position * 10.f);
}

float Particle::GetRadius()
{
	return m_radius;
}

glm::vec2& Particle::GetPosition()
{
	return m_position;
}

glm::vec2& Particle::GetVelocity()
{
	return m_velocity;
}

void Particle::Delete()
{
}

void Particle::Serialize(Storage& storage)
{ 
	storage.Serialize(m_renderHandle, m_gravity, m_velocity, m_position, m_particles, m_radius, me);
}

void Particle::SetObjectHandle(ObjectHandle _me)
{
	me = _me;
}

void Particle::SetParticles(std::vector<ObjectHandle> _particles)
{
	m_particles = _particles;
}


SCRIPT_END(Particle)


