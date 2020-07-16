#include "ScriptLoader.h"

#include <memory>
#include <string>

#include "ProcessFunctions.h"

#include "Level.h"
#include "Object.h"
#include "Logger.h"
#include "Script.h"
#include "ScriptCompiler.h"

//TESTING
#include <chrono>
#include "PlatformDetails.h"
#include "RunPython.h"

typedef std::chrono::system_clock Clock;

ScriptLoader::ScriptLoader(std::shared_ptr<Level> aLevel, unsigned long long aDLL) :
	level(aLevel),
	sharedLibraryID(aDLL),
	loggerHandle(Logger::Get("core"))
{
	if(level == nullptr)
	{
		LOG_ERROR(loggerHandle, "The level pointer was null. This will probably crash!");
	}

	directories =  level->directories;
}

void ScriptLoader::Start()
{
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
	for (auto p : RCP::fs::recursive_directory_iterator(directories->RootGameSourceDirectory / "Scripts"))
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

	//Check if any of the scripts did not compile and if so rethrow the exception (if there is any).
	std::exception_ptr expptr = ScriptCompiler::PopCompilerError();
	if(expptr != nullptr)
	{
		std::rethrow_exception(expptr);
	}
}

#define LINKER_OUTPUT
void ScriptLoader::LinkScripts()
{
	const auto now = Clock::now();
	sharedLibraryID = std::chrono::system_clock::to_time_t(now);


	//create the command line to link the script, there is a python file that automatically selects the project configuration (DEBUG, RELEASE) and the platform (32 bit, 64 bit)
	std::string linkString = (directories->PythonToolsDirectory / "Link.py").string();
	std::string commandLine(linkString + " " + PROJECT_CONFIGURATION + " " + PROJECT_PLATFORM);
	commandLine.append(" " + directories->RootGameBinaryDirectory.string() + " " + std::to_string(sharedLibraryID) + " " + directories->RootBinaryDirectory.string() + " " + directories->EngineSourceDirectory.string());	//the gamePath and the sharedLibraryID

	for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
	{
		if(!it->second->isCompilerError)
		{
			commandLine.append(" " + it->second->GetTypeName());
		}
	}

	FILE* result = RunPython::Start(commandLine);
	if (result == nullptr)
	{
		LOG_ERROR(loggerHandle, "Could not run {} to link files", linkString);
		return;
	}

	std::string originalMessage;
	while (RunPython::GetLine(result, originalMessage))
	{
#ifdef LINKER_OUTPUT
		//throw buffer in string and remove trailing newline.
		std::string buffstring(originalMessage);
		if(buffstring.find("\n") != buffstring.npos)
		{
			buffstring.erase(buffstring.find("\n"));
		}
		
		if (buffstring.find("error") != std::string::npos)
		{
			LOG_ERROR(loggerHandle, buffstring);		//prints all of the compiler output, useful for debugging, red for errors
			for (auto it = scriptList.begin(); it != scriptList.end(); ++it)
			{
				it->second->isCompilerError = true;
			}
		}
		else if (buffstring.find("warning") != std::string::npos)
		{
			LOG_WARN(loggerHandle, buffstring);		//prints all of the compiler output, useful for debugging, yellow  for warnings
		}
		else
		{
			LOG_INFO(loggerHandle, buffstring);		//prints all of the compiler output, useful for debugging
		}
#endif
	}

	if (RunPython::Stop(result))
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


