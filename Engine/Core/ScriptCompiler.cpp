#include "ScriptCompiler.h"

#include <string.h>

#include "FileSystem.h"
#include "ProcessFunctions.h"

#include "PlatformDetails.h"
#include "Level.h"
#include "Object.h"
#include "Logger.h"
#include "Storage.h"
#include "Script.h"
#include "ScriptLoader.h"
#include "Handle.h"
#include "SharedLibrary.h"
#include "RunPython.h"

#ifdef _DEBUG
#define COMPILER_OUTPUT
#endif

std::exception_ptr ScriptCompiler::expptr;

ScriptCompiler::ScriptCompiler(std::shared_ptr<Script> aScript, std::shared_ptr<ConfigDirectories> aDirectories) :
	script(aScript),
	directories(aDirectories),
	storage(new Storage())
{
	RCP::fs::path gamePath = directories->RootGameBinaryDirectory / "Scripts" / "bin";
	sharedLibrary = std::make_unique<SharedLibrary>(gamePath);

	loggerHandle = Logger::Get("core");
}

void ScriptCompiler::Compile()
{
	try
	{
		script->isUpToDate = true;
		script->isCompilerError = false;

		//if the file is out of date compile it
		if (script->isRecompiling || !CheckIfSharedObjectIsUpToDate())
		{
			script->isUpToDate = false;
			CompileInternal();
		}
	}
	catch(...)
	{
		expptr = std::current_exception();
	}
}

void ScriptCompiler::ReloadScript()
{
	//if the script hasn't been resaved since the last failed recompile, 
	//don't try to recompile, because it will fail again.
	std::error_code err;
	RCP::fs::file_time_type result = last_write_time(script->scriptPath, err);
	const time_t currentScriptWriteTime = result.time_since_epoch().count();
	if (lastScriptWriteTime == currentScriptWriteTime)
	{
		return;
	}

	std::unordered_map<std::string,std::shared_ptr<Script>> allDependants;
	allDependants.insert(std::pair<std::string, std::shared_ptr<Script>>(script->scriptType, script));
	GetIncludedScripts(allDependants);
	
	std::vector<std::thread*> threads;
	for (auto p : allDependants)
	{
		std::thread* temp = new std::thread(&ScriptCompiler::Recompile, p.second->scriptCompiler.get());
		threads.push_back(temp);
	}

	for (int i = 0; i < threads.size(); i++)
	{
		threads.at(i)->join();
	}

	bool noCompileErrors = true;
	for (auto p : script->level->scriptLoader->scriptList)
	{
		if(p.second->isCompilerError)
		{
			noCompileErrors = false;
			break;
		}
	}

	if (noCompileErrors)
	{
		//recompile the scripts that are dependent on this script

		Relink();
		if (!script->isCompilerError)
		{
			for(auto p : script->level->scriptLoader->scriptList)
			{
				p.second->scriptCompiler->Unload();
				p.second->scriptCompiler->LoadSharedObject();
				if (!p.second->isCompilerError)
				{
					p.second->scriptCompiler->SwapRunningObjects();
				}
			}
		}
	}
	
	if(!noCompileErrors)
	{
		for (auto p : script->level->scriptLoader->scriptList)
		{
			p.second->scriptCompiler->lastScriptWriteTime = currentScriptWriteTime;
		}
	}
}

std::exception_ptr ScriptCompiler::PopCompilerError()
{
	std::exception_ptr temp = expptr;
	expptr = nullptr;
	return temp;
}

void ScriptCompiler::GetIncludedScripts(std::unordered_map<std::string, std::shared_ptr<Script>>& aAllIncluded)
{
	if(script->includedInScripts.empty())
	{
		return;
	}

	for(int i=0; i < script->includedInScripts.size(); i++)
	{
		if (aAllIncluded.find(script->includedInScripts.at(i)->GetTypeName()) == aAllIncluded.end())
		{
			aAllIncluded.insert(std::pair<std::string, std::shared_ptr<Script>>(script->includedInScripts.at(i)->GetTypeName(), script->includedInScripts.at(i)));
			script->includedInScripts.at(i)->scriptCompiler->GetIncludedScripts(aAllIncluded);
		}
	}
}

void ScriptCompiler::Recompile()
{
	script->isRecompiling = true;
	Compile();
	script->isRecompiling = false;
}

void ScriptCompiler::Relink()
{
	script->level->scriptLoader->LinkScripts();
}

void ScriptCompiler::SwapRunningObjects()
{
	//create new objects and swap them with the running objects
	//then load all serialized values and restart the script
	storage->save = false;
	for (auto& p : script->runningObjects)
	{
		std::shared_ptr<Object> tempObj = std::shared_ptr<Object>(script->CreateObject(), script->DeleteObject);
		script->level->objectList.at(p.index).swap(tempObj);
		script->level->objectList.at(p.index)->SetLevel(script->level.get());
		script->SetLevel(script->level.get());
		script->level->objectList.at(p.index)->SetLogger(loggerHandle);
		script->level->objectList.at(p.index)->Serialize(*storage);
		script->level->objectList.at(p.index)->Restart();
	}
}

void ScriptCompiler::Unload()
{
	storage->save = true;
	for (auto& p : script->runningObjects)
	{
		script->level->objectList.at(p.index)->Serialize(*storage);
		script->level->objectList.at(p.index).reset();
	}

	sharedLibrary->UnloadSharedLibrary();
	script->CreateObject = nullptr;
	script->DeleteObject = nullptr;
}

bool ScriptCompiler::CheckIfSharedObjectIsUpToDate()
{
	std::error_code err;		//used for error checking instead of triggering a breakpoint.

	//Check if file is out of date
	RCP::fs::file_time_type result = RCP::fs::last_write_time(script->scriptPath, err);
	time_t localLastScriptWriteTime = result.time_since_epoch().count();
	
	/**
	\note If an error occurred during checking of the last write time of the script the function returns false. 
			This might trigger a recompile. An error often occurs when the user saves. 
			So a recompile doesn't matter, because the user wanted to recompile anyways.
	*/
	if(err.value() != 0)
	{
		LOG_WARN(loggerHandle, "Error during checking of last_write_time. This sometimes occurs when it is written to when the code wants to know when it was written to. We will return false just to be sure.");
		return false;	//recompile to be sure
	}
	
	RCP::fs::path sharedLibraryPath = directories->RootGameBinaryDirectory / "Scripts" / "bin" / (std::string(SharedLibrary::sharedLibraryPrefix) + "Scripts" + (std::to_string(script->level->scriptLoader->sharedLibraryID) + SharedLibrary::sharedLibraryExtension));
	result = RCP::fs::last_write_time(sharedLibraryPath, err);
	time_t lastSharedLibWriteTime = result.time_since_epoch().count();
	if (err.value() != 0)
	{
		LOG_WARN(loggerHandle, "Error during checking of last_write_time. We will return false just to be sure.");
		return false;
	}

	return lastSharedLibWriteTime > localLastScriptWriteTime;
}

void ScriptCompiler::LoadSharedObject()
{
	if(script->isCompilerError)
	{
		LOG_ERROR(loggerHandle, "Can't load shared object because there is an error in the script {}", script->scriptType)
		return;
	}

	LoadSharedObjectInternal();

	if (script->isCompilerError)
	{
		std::error_code err;
		RCP::fs::file_time_type result = last_write_time(script->scriptPath, err);
		lastScriptWriteTime = result.time_since_epoch().count();
	}
}

void ScriptCompiler::CompileInternal()
{
	//create the command line to compile the script.
	std::string commandLine((directories->PythonToolsDirectory / "Compile.py").string() + " " + PROJECT_CONFIGURATION + " " + PROJECT_PLATFORM);
	commandLine.append(" " + directories->RootGameBinaryDirectory.string() + " " + script->scriptPath.string() + " " + script->scriptType + " " + directories->EngineSourceDirectory.string());	//the gamePath, scriptPath and the scriptName
	FILE* output = RunPython::Start(commandLine);
	


	//compile the script using a python script, found in the tools folder
	if(output == nullptr)
	{
		//TODO(Resul): Investigate how we can actually check if a process has run successfully.
		assert(false && "file in commandLine could not be opened");
		script->isCompilerError = true;
		return;
	}

	std::unordered_map<std::string, std::shared_ptr<Script>>& scriptList = script->level->scriptLoader->scriptList;
	RCP::fs::path dependencyPath;
	std::string originalString;
	while (RunPython::GetLine(output, originalString))
	{
		//Throw buffer in string and remove trailing newline.
		std::string buffstring(originalString);
		if (buffstring.find("\n") != buffstring.npos)
		{
			buffstring.erase(buffstring.find("\n"));
		}
		
		if(buffstring.find("error") != std::string::npos && buffstring.find("Note: including file: ") == std::string::npos)
		{
#ifdef COMPILER_OUTPUT
			LOG_ERROR(loggerHandle, buffstring);		//prints all of the compiler output, useful for debugging
#endif
			script->isCompilerError = true;
		}
		else if(buffstring.find("warning") != std::string::npos && buffstring.find("Note: include file: ") == std::string::npos)
		{
			LOG_WARN(loggerHandle, buffstring);
		}
		else
		{
#ifdef COMPILER_OUTPUT
			LOG_INFO(loggerHandle, buffstring);		//prints all of the compiler output, useful for debugging
#endif
		}	

		if (!script->isRecompiling)
		{
			//get all of the includes and set the includedInScripts variable for each of the included scripts
			const std::string stringbuff(originalString);
			if (GetInclude(stringbuff, /*out*/dependencyPath))
			{
				std::string dependencyType = dependencyPath.filename().string();
				dependencyType.erase(dependencyType.end() - 2, dependencyType.end());		//remove the .h from the end of the filename
				LOG_INFO(loggerHandle, "{}", dependencyPath.string())
				scriptList.at(dependencyType)->includedInScripts.push_back(scriptList.at(script->scriptType));
				script->includeScripts.push_back(scriptList.at(dependencyType));
			}
		}
	}

	if(!RunPython::Stop(output))
	{
		LOG_ERROR(loggerHandle, "error in compilation of script with type {}", script->scriptType);
		script->isCompilerError = true;
	}
}

void ScriptCompiler::LoadSharedObjectInternal()
{

	const bool sharedLibraryLoaded = sharedLibrary->LoadSharedLibrary("Scripts" + std::to_string(script->level->scriptLoader->sharedLibraryID));
	if (!sharedLibraryLoaded)
	{
		std::string err = sharedLibrary->GetLoadingError();
		script->isCompilerError = true;
		LOG_ERROR(loggerHandle, "Couldn't load the shared object of the script \"{}\" with error code \"{}\"", script->scriptType, err);
		return;
	}

	script->CreateObject = sharedLibrary->GetExportedFunction<CREATEFUNCTION>(std::string("Create") + script->scriptType);
	if (script->CreateObject == nullptr)
	{
		script->isCompilerError = true;
		LOG_ERROR(loggerHandle, "Couldn't load the CreateObject() function make sure you have added the START_SCRIPT and END_SCRIPT macros to the script {}", script->scriptType)
		return;
	}

	script->DeleteObject = sharedLibrary->GetExportedFunction<DELTEFUNCTION>(std::string("Delete") + script->scriptType);
	if (script->DeleteObject == nullptr)
	{
		script->isCompilerError = true;
		LOG_ERROR(loggerHandle, "Couldn't load the DeleteObject() function make sure you have added the START_SCRIPT and END_SCRIPT macros to the script {}", script->scriptType)
		return;
	}

	script->SetLevel = sharedLibrary->GetExportedFunction<SET_LEVEL_FUNCTION>(std::string("SetLevel") + script->scriptType);
	if (script->SetLevel == nullptr)
	{
		LOG_ERROR(loggerHandle, "Couldn't load the SetLevel() function make sure you have added the START_SCRIPT and END_SCRIPT macros to the script {}", script->scriptType)
		return;
	}
	script->SetLevel(script->level.get());	
}

bool ScriptCompiler::GetInclude(std::string aIn, RCP::fs::path& aOutPath)
{
	const std::string noteString("Note: including file: ");

	const size_t gamePathFound = aIn.find(script->GetTypeName());
	const size_t noteFound = aIn.find(noteString);								/// /showIncludes adds "Note: including file: " in front of the include, we check if we have found this.
	const size_t scriptNameFound = aIn.find(script->scriptType);				/// we don't want a dependency on ourselves so we filter this scripts type out
	
	if(noteFound != std::string::npos && gamePathFound != std::string::npos && scriptNameFound == std::string::npos)
	{
		///\todo(Resul) find a way to also check the includes in the header file
			aIn.erase(aIn.find(noteString), aIn.find(noteString) + noteString.size());		///remove the noteString from the string.
			aOutPath = RCP::fs::path(aIn);
			return true;
	}
	return false;
}


ScriptCompiler::~ScriptCompiler()
{
	sharedLibrary->UnloadSharedLibrary();
}


