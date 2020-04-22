#include "Script.h"

#include "Level.h"
#include "ScriptCompiler.h"
#include "Object.h"
#include "Handle.h"
#include "Logger.h"

Script::Script(RCP::path aScriptPath, std::shared_ptr<Level> aLevel) :
	level(aLevel),
	scriptPath(aScriptPath),
	isRecompiling(false),
	isUpToDate(true),
	isCompilerError(false),
	isRuntimeError(false),
	CreateObject(nullptr),
	DeleteObject(nullptr),
	SetLevel(nullptr)
{
	SetScriptType();
}

void Script::CreateScriptCompiler()
{
	scriptCompiler = std::make_unique<ScriptCompiler>(shared_from_this(), level->directories);
}

void Script::Start()
{
	//The "Game" script is the only script that gets initialized in the core
	if(scriptType == "Game" && !isCompilerError)
	{
		ObjectHandle startObject = CreateScriptObject("Game");
	}
}

void Script::Update(float aDeltatime)
{
	if(scriptCompiler->CheckIfDLLIsUpToDate())
	{
		if (!isCompilerError && !isRuntimeError)
		{
			for (auto p : runningObjects)
			{
				try
				{
					level->objectList.at(p.index)->Update(aDeltatime);
				}
				catch(const std::exception& e)
				{
					LOG_ERROR(Logger::Get("core"), "An error occurred with exception \"{}\" in {} during the Update loop, to re-enable the script please fix the error and save or restart.", e.what(), scriptType);
					isRuntimeError = true;
				}
				catch(...)
				{
					LOG_ERROR(Logger::Get("core"), "An unknown error occurred in the script {} during the Update loop, to re-enable the script please fix the error and save or restart.", scriptType);
					isRuntimeError = true;
				}	
			}
		}
	}
	else
	{
		scriptCompiler->ReloadScript();
	}
}

void Script::FixedUpdate()
{
	if (!isCompilerError && !isRuntimeError)
	{
		for (auto p : runningObjects)
		{
			try
			{
				level->objectList.at(p.index)->FixedUpdate();
			}
			catch (const std::exception& e)
			{
				LOG_ERROR(Logger::Get("core"), "An error occurred with exception \"{}\" in {} during the FixedUpdate loop, to re-enable the script please fix the error and save or restart.", e.what(), scriptType);
				isRuntimeError = true;
			}
			catch (...)
			{
				LOG_ERROR(Logger::Get("core"), "An unknown error occurred in the script {} during the FixedUpdate loop, to re-enable the script please fix the error and save or restart.", scriptType);
				isRuntimeError = true;
			}
		}
	}
}

ObjectHandle Script::CreateScriptObject(const char* aName)
{
	if(!isCompilerError && !isRuntimeError)
	{
		std::shared_ptr<Object> tempObject(CreateObject(), DeleteObject);
		tempObject->SetName(aName);
		tempObject->SetLevel(level.get());
		tempObject->SetLogger(Logger::Get("script"));
		tempObject->script = shared_from_this();
		level->objectList.push_back(tempObject);
		const ObjectHandle obj = { level->objectList.size() - 1};
		runningObjects.push_back(obj);
		return obj;
	}

	LOG_WARN(Logger::Get("core"), "Couldn't create {} because there is an error in the script!", scriptType);
	const ObjectHandle obj = { 1231234 };
	return obj;
}

std::string Script::GetTypeName() const
{
	return scriptType;
}

void Script::SetScriptType()
{
	scriptType = scriptPath.filename().string();
	const size_t extensionLength = scriptPath.extension().string().size();
	scriptType.erase(scriptType.end() - extensionLength, scriptType.end());
}

void Script::Delete()
{
	for (auto p : runningObjects)
	{
		level->objectList.at(p.index)->Delete();
	}
}

Script::~Script()
{
	CreateObject = nullptr;
	DeleteObject = nullptr;
}



