#pragma once
#include <unordered_map>

#include <memory>
#include "FileSystem.h"
#include "FunctionDefinition.h"

#include "ForwardDecl.h"


class ScriptCompiler;
class ScriptLoader;

//exported function definitions
typedef Object* (FUNCTION_CDECL *CREATEFUNCTION)();
typedef void(FUNCTION_CDECL *DELTEFUNCTION)(Object*);
typedef void(FUNCTION_CDECL *SET_LEVEL_FUNCTION)(Level*);

/**\brief Calls the functions of the script, holds one script
*		
*	Compiling and Linking is done in the ScriptLoader class and the ScriptCompiler class
*	Has a list of all of the spawned Objects of this type.
*	\see Object()
*	\see ScriptCompiler()
*	\see ScriptLoader()
*/
class Script : public std::enable_shared_from_this<Script>
{
//functions
public:
	/**
	 * \brief initializes script
	 * \param scriptPath	the path of the script
	 * \param level			the level that spawned this script;
	 */
	Script(RCP::fs::path scriptPath, std::shared_ptr<Level> level);
	~Script();

	/**
	 * \brief has to be separate from the CTor because we first need to have created a shared pointer
	 * \param gamePath the path where the game files are stored
	 */
	void CreateScriptCompiler();
	/**
	 * \brief Creates the "Game" script. Through this script all other scripts will be initialized
	 */
	void Start();
	/**
	 * \brief Goes through all of the scripts and updates them or recompiles them.
	 * 
	 * Checks if the CPP file is newer than the shared object. If this is the case it tries to recompile.
	 * If the recompile is unsuccessful it will continue updating the old script, which did work.
	 * If the recompile is successful it will reload all of the running objects.
	 * If a recompile is not necessary it will loop through all of the running objects and call
	 * Update on them.
	 * \param aDeltatime the time that passed since the last update
	 */
	void Update(float aDeltatime);
	/**
	 * \brief Goes through all of the scripts and updates them on a fixed interval.
	 */
	void FixedUpdate();
	/**
	 * \brief Goes through all of the scripts and calls delete on them.
	 */
	void Delete();
	/**
	 * \brief Creates a script object, saves it internally and returns a handle to the object.
	 * A handle is returned so that we can recompile and reload scripts without breaking pointers.
	 * \return A handle to the Object that is spawned.
	 */
	ObjectHandle CreateScriptObject(const char* aName);

	///The typeName of this Script
	std::string GetTypeName() const;

private:

	/**
	 * \brief Uses the scriptPath to create the scriptType
	 * ScriptType = scriptPath.filename - scriptPath.extension
	 */
	void SetScriptType();

//variables
public:
private:
	///Initializes each Script
	friend class ScriptLoader;
	///Compiles each script and loads all of the necessary objects (shared objects, functions)
	friend class ScriptCompiler;

	///the current level 
	std::shared_ptr<Level> level;

	///class that handles (re)compilation of the script
	std::unique_ptr<ScriptCompiler> scriptCompiler;

	///all objects of this script that are spawned
	std::vector<ObjectHandle> runningObjects;
	
	///path to the script and the name of the script
	RCP::fs::path scriptPath;
	std::string scriptType;
	
	/// All of the scripts that depend on this script.
	std::vector<std::shared_ptr<Script>> includedInScripts;
	/**
	 * \brief all of the scripts this script is depending on. This is used during initialization.
	 * \warning Do not use after initialization as this destroys the data contained in this vector.
	 */
	std::vector<std::shared_ptr<Script>> includeScripts;

	///if there are errors in the script
	bool isCompilerError;
	bool isRuntimeError;

	///if the shared object is up to date with the file
	bool isUpToDate;

	///if the script is recompiling
	bool isRecompiling;

	///The function that returns a pointer to the script Object
	CREATEFUNCTION CreateObject;
	DELTEFUNCTION DeleteObject;

	//TEMP
	SET_LEVEL_FUNCTION SetLevel;

};

