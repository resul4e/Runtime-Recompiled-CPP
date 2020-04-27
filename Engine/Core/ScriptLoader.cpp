#include "ScriptLoader.h"

#include <memory>

#include "ProcessFunctions.h"

#include "Level.h"
#include "Object.h"
#include "Logger.h"
#include "Script.h"
#include "ScriptCompiler.h"

//TESTING
#include <chrono>
#include "PlatformDetails.h"

typedef std::chrono::system_clock Clock;

ScriptLoader::ScriptLoader(std::shared_ptr<Level> aLevel, unsigned long long aDLL) :
	level(aLevel),
	SharedLibraryID(aDLL),
	directories(level->directories)
{
}

void ScriptLoader::Start()
{
	loggerHandle = Logger::Get("core");

	LoadScripts();
	if(scriptList.empty())
	{
		LOG_WARN(loggerHandle,"No scripts detected in the folder {}/Scripts", directories->RootGameSourceDirectory.string())
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

void ScriptLoader::LoadScripts()
{
	//recursively go through all of the scripts and create a script class for them
	for (auto p : RCP::recursive_directory_iterator(directories->RootGameSourceDirectory / "Scripts"))
	{
		if (p.path().extension() == ".cpp")
		{
			//create script and insert it in the scriptList
			std::shared_ptr<Script> tempScript = std::make_shared<Script>(p.path(), level);
			scriptList.insert(std::pair<std::string, std::shared_ptr<Script>>(tempScript->GetTypeName(), tempScript));
			scriptList.at(tempScript->GetTypeName())->CreateScriptCompiler();
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
	SharedLibraryID = std::chrono::system_clock::to_time_t(now);

	FILE *in;
	//create the command line to link the script, there is a python file that automatically selects the project configuration (DEBUG, RELEASE) and the platform (32 bit, 64 bit)
	std::string commandLine("py " + (directories->PythonToolsDirectory / "Link.py").string() + " " + PROJECT_CONFIGURATION + " " + PROJECT_PLATFORM);
	commandLine.append(" " + directories->RootGameBinaryDirectory.string() + " " + std::to_string(SharedLibraryID) + " " + directories->RootBinaryDirectory.string() + " " + directories->EngineSourceDirectory.string());	//the gamePath and the SharedLibraryID

	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		if(!it->second->isCompilerError)
		{
			commandLine.append(" " + it->second->GetTypeName());
		}
	}

	if ((in = OPEN_SOME_PROCESS(commandLine.c_str(), "rt")) == nullptr)
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

	if (CLOSE_SOME_PROCESS(in) != 0)
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


