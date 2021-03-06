#include "Game.h"
#include "Core/Storage.h"
#include "Core/Level.h"
#include "Rendering/WindowComponent.h"
#include "Particle.h"

SCRIPT_START(Game)

Game::Game()
{
}

Game::~Game()
{ 
}

void Game::Start()
{
	WindowComponent::SetVerticalSync(true); 
	for(int i = 0; i < 500; i++)
	{
		ObjectHandle particle = lvl->CreateObject("Particle", "particle");
		ParticleInterface::SetObjectHandle(particle, particle);
		m_particles.push_back(particle);
	}

	for (auto part : m_particles)
	{
		ParticleInterface::SetParticles(part, m_particles);
	}
}

void Game::Restart()
{  
	
}

void Game::FixedUpdate()  
{ 
}

void Game::Update(float aDeltatime)
{
	
}

void Game::Delete()
{
}

void Game::Serialize(Storage& storage)
{
	//uncomment if you want to save something when recompiling
	//place whatever you want to serialize inside of the Serialize() function
	storage.Serialize(m_particles);
}

SCRIPT_END(Game)
