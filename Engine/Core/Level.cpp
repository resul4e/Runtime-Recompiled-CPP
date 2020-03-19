#include "stdafx.h"
#include "Level.h"

#include <cstring>
#include <iostream>
#include <chrono>
#include <filesystem>

#include "Object.h"
#include "Transform/Vector.h"
#include "Logger.h"
#include "PlatformDetails.h"


using namespace std::filesystem;
using std::cout;
using std::endl;


#define FULL_RECOMPILE

Level::Level()
{

}

void Level::Start(const char* aGamePath, const char* aWorkingDir)
{
	//setup compile and link tools
	system((std::string("py Tools/Setup.py") + " " + PROJECT_CONFIGURATION + " " + PROJECT_PLATFORM + " " + aGamePath).c_str());

	scriptConsole = Logger::Add("script");
	coreConsole = Logger::Add("core");

	path gamePath(aGamePath);
#ifdef FULL_RECOMPILE
	//removes all of the files from the game bin and Intermediate files
	//to be sure that we are building the most up to date scripts
	std::string deleteCommand("del /S /Q " + gamePath.string() + "\\bin " + gamePath.string() + "\\Intermediate");
	system(deleteCommand.c_str());
#endif
	unsigned long long ID = RemoveOldDLL(gamePath);
	scriptLoader = std::make_unique<ScriptLoader>(thisLvl, ID);

	scriptLoader->Start(gamePath, path(aWorkingDir));
}

void Level::Update(float aDeltatime)
{
	scriptLoader->Update(aDeltatime);
}

void Level::FixedUpdate()
{
	for (auto it = scriptLoader->scriptList.begin(); it != scriptLoader->scriptList.end(); ++it)
	{
		it->second->FixedUpdate();
	}
}

void Level::Delete()
{
	for (auto it = scriptLoader->scriptList.begin(); it != scriptLoader->scriptList.end(); ++it)
	{
		it->second->Delete();
	}
}

ObjectHandle Level::CreateObject(const char* typeName, const char* aObjectName)
{
	if(scriptLoader->scriptList.find(typeName) != scriptLoader->scriptList.end())
	{
		return scriptLoader->scriptList.at(typeName)->CreateScriptObject(aObjectName);
	}
	LOG_ERROR(Logger::Get("core"), "Could not create object with type {}, because the type does not exist!", typeName);
	return { 1231234 };
}

Object* Level::GetObjectPointer(ObjectHandle obj)
{
	return objectList.at(obj.index).get();
}

Level::~Level()
{
}

ObjectHandle Level::GetObjectWithName(const char* aName)
{
	ObjectHandle temp = {1231234};

	for(size_t i=0; i < objectList.size(); i++)
	{
		if(strcmp(objectList.at(i)->GetName(), aName) == 0)
		{
			temp = { i };
			break;
		}
	}
	
	return temp;
}

//TEMP
void Level::Restart()
{
	objectList.clear();
	scriptLoader->Restart();
}

unsigned long long Level::RemoveOldDLL(path aGamePath)
{
	//remove all of the DLL files exept for the latest one
	path latest;
	for (auto p : directory_iterator(aGamePath.string() + "\\bin\\" + PROJECT_PLATFORM + "\\" + PROJECT_CONFIGURATION))
	{
		if (p.path().extension() == ".dll")
		{
			if (exists(latest))
			{
				if (last_write_time(p.path()) > last_write_time(latest))
				{
					remove(latest);
					latest = p.path();
				}
				else
				{
					remove(p.path());
				}
			}
			else
			{
				latest = p.path();
			}
		}
	}

	if(!latest.empty())
	{
		std::string lastestID;
		lastestID = latest.filename().string();
		lastestID.erase(0, 7);
		lastestID.erase(lastestID.find(".dll"), lastestID.back());

		return atoll(lastestID.c_str());
	}
	return 0;
}

std::shared_ptr<Level> CreateLevel()
{
	std::shared_ptr<Level> lvl = std::make_shared<Level>();
	lvl->thisLvl = lvl;
	return lvl;
}
