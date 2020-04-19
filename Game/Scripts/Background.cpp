#include "Background.h"
#include "Rendering/AnimationComponent.h"
#include "Rendering/SpriteComponent.h"
#include "Core/Storage.h"
#include "Audio/AudioComponent.h"
#include "Core/Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include "Physics/PhysicsComponent.h"

SCRIPT_START(Background)

Background::Background()
{
}


Background::~Background()
{
}

void Background::Start()
{
	std::ifstream worldMap;
	worldMap.open("..\\..\\..\\Game\\Assets\\Map\\World1-1.mariomap");
	if(!worldMap.is_open())
	{
		LOG_ERROR(Logger::Get("scripts"), "Could not open map!");
		throw std::exception();
	}
	
	for (size_t y = 0; y < 28; y++)
	{
		for (size_t x = 0; x < 33; x++)
		{
			spriteSheet.push_back(glm::vec4(x * 32, y * 32, 32, 32));
		}
	}

	std::string line;
	int y = 0;
	while (std::getline(worldMap, line))
	{
		std::istringstream iss(line);
		int number;
		int x = -1;
		int s = 0; 
		while (iss >> number)
		{
			x++;
			if(number == 28)
			{
				if(s!=0)
				{
					CreateHorizontalStaircase(s, x, y);
					s = 0;
				}
				continue;
			}
			if(number > 923)
			{
				continue;
			}
			glm::vec2 pos(spriteSheet.at(number).x, spriteSheet.at(number).y);
			glm::vec2 off(spriteSheet.at(number).z, spriteSheet.at(number).w);
			background.push_back(SpriteComponent::AddSprite("..\\Game\\Assets\\Sprites\\NES - Super Mario Bros - Tileset.png", pos, off));
			offset.push_back(glm::vec2(x * 32, 16 + y * 32));

			if (number == 33)
			{
				s++;
			}
			else if(s!=0)
			{
				CreateHorizontalStaircase(s, x, y);
				s = 0;
			}

		}

		y++;
	}

	floors.push_back(PhysicsComponent::AddStatic(floor1Pos, floor1Size));
	floors.push_back(PhysicsComponent::AddStatic(floor2Pos, floor2Size));
	floors.push_back(PhysicsComponent::AddStatic(floor3Pos, floor3Size));
	floors.push_back(PhysicsComponent::AddStatic(floor4Pos, floor4Size));

	pipes.push_back(PhysicsComponent::AddStatic(glm::vec2(9.28, 3.68), glm::vec2(0.32, 0.32)));
	pipes.push_back(PhysicsComponent::AddStatic(glm::vec2(12.48, 3.52), glm::vec2(0.32, 0.48)));
	pipes.push_back(PhysicsComponent::AddStatic(glm::vec2(15.04, 3.36), glm::vec2(0.32, 0.64)));
	pipes.push_back(PhysicsComponent::AddStatic(glm::vec2(18.56, 3.36), glm::vec2(0.32, 0.64)));
	pipes.push_back(PhysicsComponent::AddStatic(glm::vec2(52.48, 3.68), glm::vec2(0.32, 0.32)));
	pipes.push_back(PhysicsComponent::AddStatic(glm::vec2(57.60, 3.68), glm::vec2(0.32, 0.32)));
}

void Background::Restart()
{
}

void Background::FixedUpdate()
{

}

void Background::Update(float aDeltatime)
{
	for (int i = 0; i < background.size(); i++)
	{
		SpriteComponent::SetPosition(background.at(i), offset.at(i));
	}
}

void Background::Delete()
{
}

void Background::Serialize(Storage& storage)
{
	storage.Serialize(offset, background, pipes, floors, pyramids, name);
}

void Background::CreateHorizontalStaircase(int s, int x, int y)
{
	pyramids.push_back(PhysicsComponent::AddStatic(glm::vec2(((x * 32)) - s * 16, 32 + (y * 32)) / 100.f, glm::vec2(s*0.16, 0.16)));
}

PhysicsCompHandle Background::GetPipe(size_t aIndex)
{
	return pipes.at(aIndex);
}

SCRIPT_END(Background)
