#include "stdafx.h"
#include "ScriptLoader.h"

#include <memory>

#include "Level.h"
#include "Object.h"
#include "Logger.h"



//TESTING
#include <chrono>
#include <ctime>
#include "PlatformDetails.h"

typedef std::chrono::system_clock Clock;
using namespace std::filesystem;

ScriptLoader::ScriptLoader(std::shared_ptr<Level> aLevel, unsigned long long aDLL) :
	level(aLevel),
	DLLID(aDLL)
{
}

void ScriptLoader::Start(path aGamePath, path aWorkingPath)
{
	workingPath = aWorkingPath;
	loggerHandle = Logger::Get("core");

	gamePath = aGamePath;
	LoadScripts(aGamePath, aWorkingPath);
	if(scriptList.empty())
	{
		LOG_WARN(loggerHandle,"No scripts detected in the folder {}/Scripts", aGamePath.string())
		return;
	}
	CompileScripts();
	LinkScripts();
	LoadDLLs();

	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		it->second->Start();
	}

	StartScripts();
}

void ScriptLoader::Update(float aDeltatime)
{
	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		it->second->Update(aDeltatime);
	}
}

void ScriptLoader::Restart()
{
	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		it->second->runningObjects.clear();
		it->second->isRuntimeError = false;
		it->second->Start();
	}

	StartScripts();
}

void ScriptLoader::StartScripts()
{
	for (int i = 0; i < level->objectList.size(); i++)
	{
		try
		{
			level->objectList.at(i)->Start();
		}
		catch (const std::exception& e)
		{
			LOG_ERROR(loggerHandle, "An error occurred with exception \"{}\" in the object {} during the Start function,\
				please fix the error and recompile the script or restart.", e.what(), level->objectList.at(i)->GetName());
			level->objectList.at(i)->script->isRuntimeError = true;
		}
		catch (...)
		{
			LOG_ERROR(loggerHandle, "An unknown error occurred in the object {} during the Start function,\
				to re-enable the script please fix the error and save or restart.", level->objectList.at(i)->GetName());
			level->objectList.at(i)->script->isRuntimeError = true;
		}
	}
}

void ScriptLoader::LoadScripts(path aGamePath, path aWorkingPath)
{
	path scriptPath = aGamePath;
	scriptPath += path("\\Scripts");
	
	//recursively go through all of the scripts and create a script class for them
	for (auto p : recursive_directory_iterator(scriptPath))
	{
		if (p.path().extension() == ".cpp")
		{
			//create script and insert it in the scriptList
			std::shared_ptr<Script> tempScript = std::make_shared<Script>(p.path(), level);
			scriptList.insert(std::pair<std::string, std::shared_ptr<Script>>(tempScript->GetTypeName(), tempScript));
			scriptList.at(tempScript->GetTypeName())->CreateScriptCompiler(aGamePath, aWorkingPath);
		}
	}
}

void ScriptLoader::CompileScripts()
{
	std::vector<std::thread*> threads;
	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		std::thread* temp = new std::thread(&ScriptCompiler::Compile, it->second->scriptCompiler.get());
		threads.push_back(temp);
	}

	for (int i = 0; i < threads.size(); i++)
	{
		threads.at(i)->join();
	}
}

#define LINKER_OUTPUT
void ScriptLoader::LinkScripts()
{
	const auto now = Clock::now();
	DLLID = std::chrono::system_clock::to_time_t(now);

	FILE *in;
	//create the command line to link the script, there is a python file that automatically selects the project configuration (DEBUG, RELEASE) and the platform (32 bit, 64 bit)
	std::string commandLine("py " + std::string(workingPath.string()) + "\\Tools\\Link.py " + PROJECT_CONFIGURATION + " " + PROJECT_PLATFORM);
	commandLine.append(" " + gamePath.string() +" "+ std::to_string(DLLID) + " " + workingPath.string());	//the gamePath and the DLLID

	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		if(!it->second->isCompilerError)
		{
			commandLine.append(" " + it->second->GetTypeName());
		}
	}

	if ((in = _popen(commandLine.c_str(), "rt")) == nullptr)
	{
		assert(false && "file in commandLine could not be opened");
		return;
	}

	char buff[2048];
	char* context[2048];
	while (fgets(buff, sizeof(buff), in) != nullptr)
	{
#ifdef LINKER_OUTPUT
		strtok_s(buff, "\n", context);
		std::string buffstring(buff);
		if (buffstring.find("error") != std::string::npos)
		{
			LOG_ERROR(loggerHandle, buff);		//prints all of the compiler output, useful for debugging, red for errors
			for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
			{
				it->second->isCompilerError = true;
			}
		}
		else if (buffstring.find("warning") != std::string::npos)
		{
			LOG_WARN(loggerHandle, buff);		//prints all of the compiler output, useful for debugging, yellow  for warnings
		}
		else
		{
			LOG_INFO(loggerHandle, buff);		//prints all of the compiler output, useful for debugging
		}
#endif
	}

	if (_pclose(in) != 0)
	{
		LOG_ERROR(loggerHandle, "error in linking")
	}
}

void ScriptLoader::LoadDLLs()
{
	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		it->second->scriptCompiler->LoadDLL();
	}
}

ScriptLoader::~ScriptLoader()
{
}


