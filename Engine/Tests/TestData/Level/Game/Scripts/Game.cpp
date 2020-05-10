#include "Game.h"
#include "Core/Storage.h"
#include "Core/Level.h"
#include "Core/Logger.h"
#include "Rendering/RenderComponent.h"
#include "Rendering/WindowComponent.h"

SCRIPT_START(Game)

Game::Game()
{
}

Game::~Game()
{ 
}

void Game::Start()
{
	
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
	//storage.Serialize();
}

SCRIPT_END(Game)
