#pragma once
#include "ExportDetails.h"
#include "ForwardDecl.h"
#include "Handle.h"
#include "Logger.h"
#include "ConfigDirectories.h"

#include <vector>
#include <memory>
#include <functional>
#include "FileSystem.h"

//forward declarations
class LevelRoot;
class ScriptLoader;

/**
 * \brief Manages all of the scripts and the level. Also main class that the scripts can use
 */
class Level : std::enable_shared_from_this<Level>
{
//functions
public:
	Level(std::shared_ptr<ConfigDirectories> _directory);
	~Level();

	/**
	 * \brief Calls ScriptLoader::Start().
	 * 
	 * In scriptLoader it compiles and loads the necessary files and functions.
	 * \see ScriptLoader::Start().
	 */
	CORE_API void Start();
	/**
	 * \brief Calls ScriptLoader::Update()
	 * \param aDeltatime the time between the last frame and this fram
	 * \see ScriptLoader::Update()
	 */
	CORE_API void Update(float aDeltatime);
	/**
	 * \brief Calls ScriptLoader::FixedUpdate().
	 * \see ScriptLoader::FixedUpdate()
	 */
	CORE_API void FixedUpdate();
	/**
	 * \brief Calls ScriptLoader::Delete()
	 */
	CORE_API void Delete();

	CORE_API void Restart();
	/**
	 * \brief	Creates an Object of the type specified.
	 * 
	 * Also adds it to the hierarchy 
	 * \todo add it to the hierarchy. Can't do that until we have a LevelRoot
	 * 
	 * \param aTypeName			the name of the Script.
	 * \param aObjectName		The name of the Object.
	 * \return					a handle to the Object.
	 */
	CORE_API ObjectHandle CreateObject(const char* aTypeName,const char* aObjectName);
	/**
	 * \brief Gets the raw Object pointer to be able to access 
	 * \warning Do not use this unless you know what you are doing. Pointers can't be safely saved, as they might change during recompile!
	 * \param aObj the objectHandle of which you want the ObjectPointer
	 * \return The raw Object pointer
	 */
	CORE_API Object* GetObjectPointer(ObjectHandle aObj);

	///TODO(Resul) Implement these.
	CORE_API ObjectHandleList GetObjects();
	CORE_API ObjectHandleList GetObjectsWithName(const char* aName);
	CORE_API ObjectHandle GetObjectWithName(const char* aName);
	CORE_API ObjectHandleList GetObjectsWithType(const char* aType);
	CORE_API ObjectHandle GetObjectWithType(const char* aType);

private:

	/**
	* \brief Creates the directories that we need whe compiling the scripts. 
	*/
	CORE_API void SetupDirectories();
	
	unsigned long long RemoveOldSharedObject();

//variables
public:
private:
	friend CORE_API std::shared_ptr<Level> CreateLevel(ConfigDirectories*);
	friend class ScriptCompiler;
	friend class Script;
	friend class ScriptLoader;
	friend int main(int argc, char* args[]);

	//TODO(Resul): Remove most of these friends using a ScriptManager
	//Temp friend for testing
	friend class ScriptLoaderTests_Start_Test;
	friend class ScriptLoaderTests_StartWithEmptyDirectory_Test;
	friend class ScriptLoaderTests;
	friend class ScriptLoaderTests_Update_Test;
	friend class ScriptLoaderTests_UpdateWithNegativeDeltaTime_Test;
	friend class ScriptLoaderTests_Restart_Test;
	friend class ScriptLoaderTests_StartWithWrongToolsFolder_Test;


	///a list of all of the objects currently running
	std::vector<std::shared_ptr<Object>> objectList;

	///The class that loads all of the scripts and manages them
	std::unique_ptr<ScriptLoader> scriptLoader;

	///reference to this object
	std::shared_ptr<Level> thisLvl;

	///the scriptLogger
	LoggerHandle scriptConsole;

	///the coreLogger
	LoggerHandle coreConsole;

	///The list of important directories
	std::shared_ptr<ConfigDirectories> directories;
};

/**
 * \brief creates the level and saves a shared_ptr to it in the Level class
 * \return 
 * \todo This is stupid and should be refactored so that we don't have to save a pointer to the level in the level.
 * \todo Don't return a shared pointer in a dll, because then we need to have the dll compiled with the same settings. 
 *			Return a new instead and create a separate function that deletes this object.
 */
CORE_API std::shared_ptr<Level> CreateLevel(ConfigDirectories*);