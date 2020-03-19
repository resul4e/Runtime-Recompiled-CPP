#include "Game.h"
#include "Core/Storage.h"
#include "Core/Level.h"
#include "Audio/AudioComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Rendering/RenderComponent.h"
#include "Rendering/WindowComponent.h"
#include "Mario.h"
#include "Block.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "CoinBlock.h"
#include "BrickBlock.h"
#include "Enemy.h"
#include "Goomba.h"
#include "Background.h"


SCRIPT_START(Game)

Game::Game()
{
}

Game::~Game()
{
}

void Game::Start()
{
	WindowComponent::SetBackgroundColor(glm::vec4(107, 140, 255, 255));

	background = lvl->CreateObject("Background", "Background");
	mario = lvl->CreateObject("Mario", "Mario");

	for(int i=0; i < 14; i++)
	{
		enemies.push_back(lvl->CreateObject("Goomba", "Goomba1"));
	}

	
	music = AudioComponent::AddMusic("..\\Game\\Assets\\Audio\\SuperMarioBros.ogg");
	AudioComponent::Play(music);
	AudioComponent::SetVolume(music, 40);
	WindowComponent::SetViewPosition(glm::vec2(224, 224));

	std::ifstream worldMap;
	worldMap.open("..\\Game\\Assets\\Map\\BoxMap.mariomap");

	std::string line;
	int y = 0;
	while (std::getline(worldMap, line))
	{
		int blockWidth = 0;
		std::istringstream iss(line);
		int number;
		int x = -1;
		std::vector<ObjectHandle> floorBlocks;
		while (iss >> number)
		{
			x++;
			if (number == 0)	
			{
				if(blockWidth != 0)
				{
					blockFloors.push_back(PhysicsComponent::AddStatic(glm::vec2(x * 32 - blockWidth * 16, (y * 32) + 21)/100.f, glm::vec2(blockWidth*0.16, 0.05)));
					floorBlocksContainer.push_back(floorBlocks);
				}
				blockWidth = 0;
				floorBlocks.clear();
				continue;
			}
			if (number == 201 || number == 202)
			{
				blockWidth++;
				coinBlocks.push_back(lvl->CreateObject("CoinBlock", "CoinBlock" + coinBlocks.size()));
				BlockInterface::SetColliderIndex(coinBlocks.back(), blockFloors.size());
				BlockInterface::SetBlockIndex(coinBlocks.back(), floorBlocks.size());
				floorBlocks.push_back(coinBlocks.back());
				if(number == 201)
				{
					CoinBlockInterface::SetPrize(coinBlocks.back(), CoinBlock::Coin);
				}
				else
				{
					CoinBlockInterface::SetPrize(coinBlocks.back(), CoinBlock::Mushroom);
				}
				BlockInterface::Setup(coinBlocks.back(), glm::vec2(x * 32, (y * 32) + 16));
			}
			if (number == 448)
			{
				blockWidth++;
				bricksBlocks.push_back(lvl->CreateObject("BrickBlock", "BrickBlock" + bricksBlocks.size()));
				BlockInterface::SetBlockIndex(bricksBlocks.back(), floorBlocks.size());
				floorBlocks.push_back(bricksBlocks.back());
				BlockInterface::SetColliderIndex(bricksBlocks.back(), blockFloors.size());
				BlockInterface::Setup(bricksBlocks.back(), glm::vec2(x * 32, (y * 32) + 16));
			}
		}
		y++;
	}
}

void Game::Restart()
{
}

void Game::FixedUpdate()
{
	if (MarioInterface::GetPositionOnScreen(mario).x > 224)
	{
		WindowComponent::MoveViewPosition(glm::vec2(MarioInterface::GetPositionOnScreen(mario).x - 224, 0));
	}
}


void Game::Update(float aDeltatime)
{
	if(first)
	{
		RegisterEnemies();
		RegisterMario();
		first = false;
	}
	
	for (int i = 0; i < coinBlocks.size(); i++)
	{
		if (PhysicsComponent::GetAreColliding(MarioInterface::GetPhysicsComponent(mario), BlockInterface::GetPhysicsComponent(coinBlocks.at(i))))
		{
			if (PhysicsComponent::GetContactNormal(MarioInterface::GetPhysicsComponent(mario), BlockInterface::GetPhysicsComponent(coinBlocks.at(i))).y < 0)
			{
				PhysicsComponent::UnregisterCollisionPair(MarioInterface::GetPhysicsComponent(mario), BlockInterface::GetPhysicsComponent(coinBlocks.at(i)));
 				BlockInterface::HitBlock(coinBlocks.at(i));
			}
		}
	}
	for (int i = 0; i < bricksBlocks.size(); i++)
	{
		if (PhysicsComponent::GetAreColliding(MarioInterface::GetPhysicsComponent(mario), BlockInterface::GetPhysicsComponent(bricksBlocks.at(i))))
		{
			if (PhysicsComponent::GetContactNormal(MarioInterface::GetPhysicsComponent(mario), BlockInterface::GetPhysicsComponent(bricksBlocks.at(i))).y < 0)
			{
				BlockInterface::HitBlock(bricksBlocks.at(i));
			}
		}
	}
}

void Game::Delete()
{
}

void Game::Serialize(Storage& storage)
{
	storage.Serialize(mario, background, music, bricksBlocks, coinBlocks, first, name);
}

void Game::RegisterMario()
{
	for (int i = 0; i < coinBlocks.size(); i++)
	{
		PhysicsComponent::RegisterCollisionPair(MarioInterface::GetPhysicsComponent(mario), BlockInterface::GetPhysicsComponent(coinBlocks.at(i)));
	}

	for (int i = 0; i < bricksBlocks.size(); i++)
	{
		PhysicsComponent::RegisterCollisionPair(MarioInterface::GetPhysicsComponent(mario), BlockInterface::GetPhysicsComponent(bricksBlocks.at(i)));
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		PhysicsComponent::RegisterCollisionPair(MarioInterface::GetPhysicsComponent(mario), GoombaInterface::GetCollider(enemies.at(i)));
	}
}

void Game::RegisterEnemies()
{
	std::vector<PhysicsCompHandle> tempCollidable;
	GoombaInterface::Setup(enemies.at(0), glm::vec2(672, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 1));
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 2));
	GoombaInterface::Setup(enemies.at(1), glm::vec2(1276, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 2));
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 3));
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(3)));
	GoombaInterface::Setup(enemies.at(2), glm::vec2(1600, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 2));
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 3));
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(2)));
	GoombaInterface::Setup(enemies.at(3), glm::vec2(1632, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(5)));
	GoombaInterface::Setup(enemies.at(4), glm::vec2(2560, 112), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(4)));
	GoombaInterface::Setup(enemies.at(5), glm::vec2(2592, 112), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(7)));
	GoombaInterface::Setup(enemies.at(6), glm::vec2(3104, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(6)));
	GoombaInterface::Setup(enemies.at(7), glm::vec2(3136, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(9)));
	GoombaInterface::Setup(enemies.at(8), glm::vec2(3648, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(8)));
	GoombaInterface::Setup(enemies.at(9), glm::vec2(3680, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(11)));
	GoombaInterface::Setup(enemies.at(10), glm::vec2(3744, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(10)));
	GoombaInterface::Setup(enemies.at(11), glm::vec2(3776, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(13)));
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 4));
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 5));
	GoombaInterface::Setup(enemies.at(12), glm::vec2(5568, 368), tempCollidable);
	tempCollidable.clear();
	tempCollidable.push_back(GoombaInterface::GetCollider(enemies.at(12)));
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 4));
	tempCollidable.push_back(BackgroundInterface::GetPipe(background, 5));
	GoombaInterface::Setup(enemies.at(13), glm::vec2(5600, 368), tempCollidable);
}

void Game::SetPoweredUp(bool aPoweredUp)
{
	for (int i = 0; i < bricksBlocks.size(); i++)
	{
		BrickBlockInterface::SetPoweredUp(bricksBlocks.at(i), aPoweredUp);
	}
}

void Game::ResizeCollider(size_t colliderIndex, size_t blockIndex)
{
	std::vector<ObjectHandle>& floorBlocks = floorBlocksContainer.at(colliderIndex);
	glm::vec2 pos = PhysicsComponent::GetPosition(blockFloors.at(colliderIndex));
	PhysicsComponent::RemovePhysicsBody(blockFloors.at(colliderIndex));
	floorBlocks.erase(floorBlocks.begin() + blockIndex);

	glm::vec2 size(0, 0.05);

	if(blockIndex == 0 || blockIndex == floorBlocks.size())
	{
		if(floorBlocks.empty())
		{
			return;
		}
		glm::vec2 newPos(0, pos.y);
		
		for(int i=0; i < floorBlocks.size(); i++)
		{
			BlockInterface::SetBlockIndex(floorBlocks.at(i), i);
			newPos.x+=BlockInterface::GetPosition(floorBlocks.at(i)).x/100.f;
			size.x += 0.16;
		}
		newPos.x /= static_cast<float>(floorBlocks.size());
		newPos.x += 0.16f;
		blockFloors.at(colliderIndex) = PhysicsComponent::AddStatic(newPos, size);
	}
	else
	{
		std::vector<ObjectHandle> newFloorBlocks;
		glm::vec2 LPos(0, pos.y);
		glm::vec2 RPos(0, pos.y);

		for(int i=0; i < blockIndex; i++)
		{
			newFloorBlocks.push_back(floorBlocks.at(i));
			BlockInterface::SetBlockIndex(floorBlocks.at(i), i);
			BlockInterface::SetColliderIndex(floorBlocks.at(i), blockFloors.size());
			LPos.x += BlockInterface::GetPosition(floorBlocks.at(i)).x / 100.f;
			size.x += 0.16;
		}

		LPos.x /= static_cast<float>(blockIndex);
		LPos.x += 0.16f;
 		blockFloors.push_back(PhysicsComponent::AddStatic(LPos, size));
		floorBlocksContainer.push_back(newFloorBlocks);

		newFloorBlocks.clear();
		size.x = 0;

		for (int i = 0; i < floorBlocks.size() - blockIndex; i++)
		{
			newFloorBlocks.push_back(floorBlocks.at(i+blockIndex));
			BlockInterface::SetBlockIndex(floorBlocks.at(i + blockIndex), i);
			BlockInterface::SetColliderIndex(floorBlocks.at(i + blockIndex), blockFloors.size());
			RPos.x += BlockInterface::GetPosition(floorBlocks.at(i+blockIndex)).x / 100.f;
			size.x += 0.16;
		}

		RPos.x /= static_cast<float>(blockIndex);
		RPos.x += 0.16f;
		blockFloors.push_back(PhysicsComponent::AddStatic(RPos, size));
		floorBlocksContainer.push_back(newFloorBlocks);
	}
}

void Game::StopMusic()
{
	AudioComponent::Stop(music);
}

SCRIPT_END(Game)
