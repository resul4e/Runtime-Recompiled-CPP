#include "Level.h"

#include <cstring>
#include <iostream>
#include <chrono>
#include "FileSystem.h"

#include "Object.h"
#include "Logger.h"
#include "PlatformDetails.h"
#include "ConfigDirectories.h"
#include "Script.h"
#include "ScriptLoader.h"

using std::cout;
using std::endl;

#define FULL_RECOMPILE

Level::Level(std::shared_ptr<ConfigDirectories> _directories) :
scriptConsole({ "" }),
coreConsole({ "" }),
directories(_directories)
{
}

void Level::Start()
{
	//setup compile and link tools
	std::string setupCommand = "py " + (directories->PythonToolsDirectory / "Setup.py").string() + " " + PROJECT_CONFIGURATION + " " + directories->RootGameBinaryDirectory.string();
	system(setupCommand.c_str());

	scriptConsole = Logger::Add("script");
	coreConsole = Logger::Add("core");

	//path gamePath(aGamePath);
#ifdef FULL_RECOMPILE
	//removes all of the files from the game bin and Intermediate files
	//to be sure that we are building the most up to date scripts
	std::string deleteCommand("del /S /Q \"" + (directories->RootGameBinaryDirectory / "Scripts" / "bin").string() + "\" \"" + (directories->RootGameBinaryDirectory / "Scripts" / "Intermediate").string() + "\"");
	system(deleteCommand.c_str());
#endif
	unsigned long long ID = RemoveOldDLL();
	scriptLoader = std::make_unique<ScriptLoader>(thisLvl, ID);

	scriptLoader->Start();
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

unsigned long long Level::RemoveOldDLL()
{
	RCP::path binDirectory = directories->RootGameBinaryDirectory / "Scripts" / "bin" / PROJECT_CONFIGURATION;
	if(!exists(binDirectory) || !is_directory(binDirectory))
	{
		LOG_WARN(coreConsole, "The " + binDirectory.string() + " directory does not exist or is something other than a directory.")
		return 0;
	}
	
	//remove all of the DLL files exept for the latest one
	RCP::path latest;
	for (auto p : RCP::directory_iterator(binDirectory))
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
		std::string latestID;
		latestID = latest.filename().string();
		
		latestID.erase(0, 7);

		//If we can't find the extension, something has gone wrong so just return 0.
		if (latestID.find(".dll") == std::string::npos)
		{
			LOG_WARN(coreConsole, "Could not remove " + latestID + " because it does not seem to be generated by us.");
			return 0;
		}
		
		latestID.erase(latestID.find(".dll"), latestID.back());

		return atoll(latestID.c_str());
	}
	return 0;
}

std::shared_ptr<Level> CreateLevel(ConfigDirectories* _directories)
{
	std::shared_ptr<Level> lvl = std::make_shared<Level>(std::shared_ptr<ConfigDirectories>(_directories));
	lvl->thisLvl = lvl;
	return lvl;
}
