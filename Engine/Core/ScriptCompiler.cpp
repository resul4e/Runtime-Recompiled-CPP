#include "ScriptCompiler.h"

#include "Platform/Filesystem.h"

#include "PlatformDetails.h"
#include "Level.h"
#include "Object.h"
#include "Logger.h"
#include "Storage.h"
#include "Script.h"
#include "ScriptLoader.h"
#include "Handle.h"

#ifdef _DEBUG
#define COMPILER_OUTPUT
#endif

ScriptCompiler::ScriptCompiler(std::shared_ptr<Script> aScript, std::shared_ptr<ConfigDirectories> aDirectories) :
	script(aScript),
	directories(aDirectories),
	dllHandle(nullptr),
	storage(new Storage())
{

	loggerHandle = Logger::Get("core");
}

void ScriptCompiler::Compile()
{
	script->isUpToDate = true;
	script->isCompilerError = false;

	//if the file is out of date compile it
	if (script->isRecompiling || !CheckIfDLLIsUpToDate())
	{
		script->isUpToDate = false;
		CompileInternal();
	}
}

void ScriptCompiler::ReloadScript()
{
	//if the script hasn't been resaved since the last failed recompile, 
	//don't try to recompile, because it will fail again.
	std::error_code err;
	file_time_type result = last_write_time(script->scriptPath, err);
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
				p.second->scriptCompiler->LoadDLL();
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

	FreeLibrary(dllHandle);
	dllHandle = nullptr;
	script->CreateObject = nullptr;
	script->DeleteObject = nullptr;
}

bool ScriptCompiler::CheckIfDLLIsUpToDate()
{
	std::error_code err;		//used for error checking instead of triggering a breakpoint.

	//Check if file is out of date
	file_time_type result = last_write_time(script->scriptPath, err);
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
	
#ifdef SEPARATE_LINKING
	//TODO(Resul): either fix this path too or remove it if the ifdef is not neccessary.
	path dllPath(std::string(gamePath.string() + "/bin/" + PROJECT_PLATFORM + "/" + PROJECT_CONFIGURATION + "/" + script->scriptType + scriptIDA + ".dll"));	///\todo(Resul) dlls are windows specific
#else
	path dllPath = directories->RootGameBinaryDirectory / "Scripts" / "bin" / PROJECT_CONFIGURATION / ("Scripts" + (std::to_string(script->level->scriptLoader->DLLID)+".dll"));	///\todo(Resul) dlls are windows specific
#endif
	file_time_type DLLresult = last_write_time(dllPath, err);
	time_t lastDLLWriteTime = DLLresult.time_since_epoch().count();
	if (err.value() != 0)
	{
		lastDLLWriteTime = 0;
	}
#ifdef SEPARATE_LINKING
	dllPath = path(std::string(gamePath.string() + "/bin/" + PROJECT_PLATFORM + "/" + PROJECT_CONFIGURATION + "/" + script->scriptType + scriptIDB + ".dll")); ///\todo(Resul) dlls are windows specific

	DLLresult = last_write_time(dllPath, err);
	if (err.value() == 0)
	{
		if (lastDLLWriteTime < DLLresult.time_since_epoch().count())
		{
			lastDLLWriteTime = DLLresult.time_since_epoch().count();
		}
	}
#endif

	return lastDLLWriteTime > localLastScriptWriteTime;
}

void ScriptCompiler::LoadDLL()
{
	if(script->isCompilerError)
	{
		LOG_ERROR(loggerHandle, "Can't load DLL because there is an error in the script {}", script->scriptType)
		return;
	}

	LoadDLLInternal();

	if (script->isCompilerError)
	{
		std::error_code err;
		file_time_type result = last_write_time(script->scriptPath, err);
		lastScriptWriteTime = result.time_since_epoch().count();
	}
}

void ScriptCompiler::CompileInternal()
{
	FILE *in;
	//create the command line to compile the script, the python file automatically selects the project configuration (DEBUG, RELEASE) and the platform (32 bit, 64 bit)
	std::string commandLine("py " + (directories->EngineSourceDirectory / "Tools\\Compile.py").string() + " " + PROJECT_CONFIGURATION + " " + PROJECT_PLATFORM);
	commandLine.append(" " + directories->RootGameBinaryDirectory.string() + " " + script->scriptPath.string() + " " + script->scriptType + " " + directories->EngineSourceDirectory.string());	//the gamePath, scriptPath and the scriptName
	//compile the script using a python script, found in the tools folder
	if((in = _popen(commandLine.c_str(), "rt")) == NULL)
	{
		assert(false && "file in commandLine could not be opened");
		script->isCompilerError = true;
		return;
	}

	char buff[2048];
	std::unordered_map<std::string, std::shared_ptr<Script>>& scriptList = script->level->scriptLoader->scriptList;
	path dependencyPath;
	while (fgets(buff, sizeof(buff), in) != nullptr)
	{

		char* context[2048];
		strtok_s(buff, "\n", context);
		std::string buffstring(buff);
		if(buffstring.find("error") != std::string::npos && buffstring.find("Note: including file: ") == std::string::npos)
		{
#ifdef COMPILER_OUTPUT
			LOG_ERROR(loggerHandle, buff);		//prints all of the compiler output, useful for debugging
#endif
			script->isCompilerError = true;
		}
		else if(buffstring.find("warning") != std::string::npos && buffstring.find("Note: include file: ") == std::string::npos)
		{
			LOG_WARN(loggerHandle, buff);
		}
		else
		{
#ifdef COMPILER_OUTPUT
			LOG_INFO(loggerHandle, buff);		//prints all of the compiler output, useful for debugging
#endif
		}	

		if (!script->isRecompiling)
		{
			//get all of the includes and set the includedInScripts variable for each of the included scripts
			const std::string stringbuff(buff);
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

	if(_pclose(in)!=0)
	{
		//assert(false && "error in compilation of script");
		LOG_ERROR(loggerHandle, "error in compilation of script with type {}", script->scriptType);
		script->isCompilerError = true;
	}
}

void ScriptCompiler::LoadDLLInternal()
{
#ifdef SEPARATE_LINKING
	//TODO(Resul): either fix this path too or remove it if the ifdef is not neccessary.
	std::string dll(gamePath.string() + "\\bin\\" + PROJECT_PLATFORM + "\\"  + PROJECT_CONFIGURATION + "\\" + script->scriptType + scriptID + ".dll");
#else
	path dll = directories->RootGameBinaryDirectory / "Scripts" / "bin" / PROJECT_CONFIGURATION / ("Scripts" + (std::to_string(script->level->scriptLoader->DLLID)+".dll"));
#endif
	dllHandle = LoadLibraryA(dll.string().c_str());
	if(dllHandle == nullptr)
	{
		DWORD err = GetLastError();
		script->isCompilerError = true;
		LOG_ERROR(loggerHandle, "Couldn't load the DLL of the script {} with error code {}", script->scriptType, err);
		return;
	}

	script->CreateObject = reinterpret_cast<CREATEFUNCTION>(GetProcAddress(dllHandle, (std::string("Create") + script->scriptType).c_str()));
	if(script->CreateObject == nullptr)
	{
		script->isCompilerError = true;
		LOG_ERROR(loggerHandle, "Couldn't load the CreateObject() function make sure you have added the START_SCRIPT and END_SCRIPT macros to the script {}" , script->scriptType)
		return;
	}

	script->DeleteObject = reinterpret_cast<DELTEFUNCTION>(GetProcAddress(dllHandle, (std::string("Delete") + script->scriptType).c_str()));
	if(script->DeleteObject == nullptr)
	{
		script->isCompilerError = true;
		LOG_ERROR(loggerHandle, "Couldn't load the DeleteObject() function make sure you have added the START_SCRIPT and END_SCRIPT macros to the script {}", script->scriptType)
		return;
	}

	script->SetLevel = reinterpret_cast<SETLEVELFUNCTION>(GetProcAddress(dllHandle, (std::string("SetLevel") + script->scriptType).c_str()));
	if (script->SetLevel == nullptr)
	{
		//script->isErrorInScript = true;
		LOG_ERROR(loggerHandle, "Couldn't load the SetLevel() function make sure you have added the START_SCRIPT and END_SCRIPT macros to the script {}", script->scriptType)
		return;
	}
	script->SetLevel(script->level.get());
}

bool ScriptCompiler::GetInclude(std::string aIn, path& aOutPath)
{
	const std::string noteString("Note: including file: ");

	const size_t gamePathFound = aIn.find(script->GetTypeName());
	const size_t noteFound = aIn.find(noteString);								/// /showIncludes adds "Note: including file: " in front of the include, we check if we have found this.
	const size_t scriptNameFound = aIn.find(script->scriptType);				/// we don't want a dependency on ourselves so we filter this scripts type out
	
	if(noteFound != std::string::npos && gamePathFound != std::string::npos && scriptNameFound == std::string::npos)
	{
		///\todo(Resul) find a way to also check the includes in the header file
			aIn.erase(aIn.find(noteString), aIn.find(noteString) + noteString.size());		///remove the noteString from the string.
			aOutPath = path(aIn);
			return true;
	}
	return false;
}


ScriptCompiler::~ScriptCompiler()
{
	FreeLibrary(dllHandle);
	dllHandle = nullptr;
}


