#pragma once
#include "FileSystem.h"
#include <unordered_map>
#include "ForwardDecl.h"
#include "Handle.h"

#include "ConfigDirectories.h"

/** \brief Processes all of the script files in the GamePath.
*	
*	Compiles and Links all of the Scripts in the GamePath.
*	Also checks if everything is still up-to-date.
*	If not it will recompile and relink the Script.
*	After which it will call the appropriate functions to
*	relink the guid with the correct Object.
*/
class ScriptLoader
{
public:
	CORE_API ScriptLoader() = delete;
	CORE_API ScriptLoader(std::shared_ptr<Level> aLevel, unsigned long long aDLL);
	CORE_API ~ScriptLoader();

	/**
	 * \brief	Compiles, creates dependecies, links and loads all of the DLLs of all of the Scripts
	 * \param gamePath				The path of the game where the Scripts folder is located
	 */
	void Start();
	/**
	 * \brief Updates each script.
	 * \param deltatime				The time passed between the last frame and this frame.
	 */
	void Update(float aDeltatime);
	/**
	 * \brief Removes all of the objects currently running and creates new ones.
	 * \note This is temporary as we will not be spawning objects by putting them in the Script::Start function
	 */
	void Restart();

private:

	/**
	 * \brief Tries to call Start on the scripts. Errors will be caught by the try-catch function.
	 */
	void StartScripts();
	/** Goes through all of the files with the .cpp extension and creates a Script for them
	*	@param[in]	gamePath		The path of the game where the Scripts folder is located
	*/
	void LoadScripts();
	/**
	* \brief goes through each script and compiles them and finds the dependencies.
	* \see ScriptCompiler::Compile()
	*/
	void CompileScripts();
	/**
	* \brief Uses the depencychain created in CreatedependencyChain() to link each object in the correct order.
	* \see ScriptCompiler::Link()
	*/
	void LinkScripts();
	/**
	 * \brief Loads each dll of each script
	 * \see ScriptCompiler::LoadDLL()
	 */
	void LoadDLLs();
//variables
public:
private:
	friend class Level;
	friend class ScriptCompiler;

	///unordered_map that holds all of the Script classes and their typeName
	std::unordered_map<std::string, std::shared_ptr<Script>> scriptList;

	///the unique ID of the DLL currently in use
	unsigned long long sharedLibraryID;

	///pointer to the Level class
	std::shared_ptr<Level> level;

	LoggerHandle loggerHandle{};

	std::shared_ptr<ConfigDirectories> directories;
};

