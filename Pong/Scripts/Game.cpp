#include "Game.h"
#include "Core/Storage.h"
#include "Core/Level.h"
#include "Core/Logger.h"

#include "Rendering/RenderComponent.h"
#include "Rendering/WindowComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Ball.h"

SCRIPT_START(Game)

Game::Game()
{
	
}

Game::~Game()
{ 
}

void Game::Start()
{
	first = true;
	WindowComponent::SetVerticalSync(true);
	//
	ballObject = lvl->CreateObject("Ball", "TheBall");
	//
	aiPlayer = lvl->CreateObject("AiPlayer", "Ai");
	player = lvl->CreateObject("Player", "player");
	//
	allSides[0] = PhysicsComponent::AddStatic(glm::vec2(-0.10f, 2.4f), glm::vec2(0.1, 2.4f), true);
	allSides[1] = PhysicsComponent::AddStatic(glm::vec2(2.4f ,-0.10f ), glm::vec2(2.4f, 0.1), false);
	allSides[2] = PhysicsComponent::AddStatic(glm::vec2(4.58, 2.4f), glm::vec2(0.1, 2.4f), true);
	allSides[3] = PhysicsComponent::AddStatic(glm::vec2(2.4f, 4.58), glm::vec2(2.4f, 0.1), false);
}

void Game::Restart()
{
}

void Game::FixedUpdate()
{ 
}

void Game::Update(float aDeltatime)
{
	if(first)
	{
		ballCollider = BallInterface::GetPhysicsComponent(ballObject);
		PhysicsComponent::RegisterCollisionPair(allSides[1], ballCollider);
		PhysicsComponent::RegisterCollisionPair(allSides[3], ballCollider);
		PhysicsComponent::RegisterCollisionPair(allSides[0], ballCollider);
		PhysicsComponent::RegisterCollisionPair(allSides[2], ballCollider);
	
		first = false;
	}
	
	if(PhysicsComponent::GetAreColliding(allSides[0], ballCollider) ||
		PhysicsComponent::GetAreColliding(allSides[2], ballCollider) || 
		PhysicsComponent::GetAreColliding(allSides[1], ballCollider) ||
		PhysicsComponent::GetAreColliding(allSides[3], ballCollider))
	{
		BallInterface::PlayRandomHitSound(ballObject);
	}
}

void Game::Delete()
{
}

void Game::Serialize(Storage& storage)
{
	storage.Serialize(ballObject, aiPlayer, player, allSides[0], allSides[1], allSides[2], allSides[3], ballCollider);
}

SCRIPT_END(Game)
