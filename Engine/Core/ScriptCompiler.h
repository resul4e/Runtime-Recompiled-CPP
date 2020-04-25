#pragma once
#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#endif

#include <memory>
#include <unordered_map>

#include "FileSystem.h"

#include "Handle.h"
#include "ForwardDecl.h"
#include "ConfigDirectories.h"


class Script;
class Storage;

/**
 * \brief Used to compile, link and load the DLLs for a particular Script
 */
class ScriptCompiler
{
public:
	ScriptCompiler(std::shared_ptr<Script> aScript, std::shared_ptr<ConfigDirectories> aDirectories);
	~ScriptCompiler();

	/**
	 * \brief Compiles all of the scripts.
	 * 
	 * First checks if the file is out of date to speed up compilation time
	 * after the first compile.
	 * If the file is out of date calls CompileInternal()
	 * 
	 * Internally calls CompileInternal();
	 * \see GetInclude()
	 * \see CompileInternal();
	 * \see Unload();
	 */
	void Compile();
	/**
	 * \brief loads the DLL and all of the functions we need to be able to use the script
	 * 
	 * Internally calls LoadDLLInternal().
	 * \see LoadDLLInternal()
	 */
	void LoadDLL();
	/**
	 * \brief Checks if the DLL is older than the cpp file. If it is older the Script needs a recompile.
	 * \return True if the DLL is younger than the saved cpp file. False if the DLL is older than the saved cpp file.
	 */
	bool CheckIfDLLIsUpToDate();
	/**
	 * \brief recompiles and relinks the script
	 * \see Recompile()
	 * \see Relink();
	 */
	void ReloadScript();

private:
	///\brief Unloads all of the data of the old Script
	void Unload();
	///\brief Calls the batch file to compile all of the scripts and sets the dependencies of each script
	void CompileInternal();
	///\brief loads the dll and the functions the script needs
	void LoadDLLInternal();
	/**
	 * \brief formats the output of /showincludes command in the .py file to only output the includes we are interested in.
	 * \param[in] in		One line of the output f _popen.
	 * \param[out] outPath	The path of the include.
	 * \return				If an include is found, outPath is only valid if this is true.
	 */
	bool GetInclude(std::string in, RCP::path& outPath);
	/**
	* \brief Calls Compile with the isRecompiling flag turned true
	*/
	void Recompile();
	/**
	* \brief Calls the LinkInternal method
	* \see LinkInternal
	*/
	void Relink();
	/**
	 * \brief Switches the outdated Objects for new ones and populates it with the serialized data after reloading has finished.
	 */
	void SwapRunningObjects();
	/**
	* \brief Gets all of the scripts that include this script. They will have to also be recompiled, so that everything is up-to-date.
	* \param aAllIncluded All of the scripts that include the current script
	*/
	void GetIncludedScripts(std::unordered_map<std::string, std::shared_ptr<Script>>& aAllIncluded);

//variables
public:
private:
	friend class ScriptLoader;
	
	///pointer to the script parent.
	std::shared_ptr<Script> script;

	///used to check if script is up to date.
	time_t lastScriptWriteTime = 0;

	///dll handle.
	HINSTANCE dllHandle;

	///This object stores all of the data the user doesn't want to lose during recompile.
	Storage* storage;

	///the handle to the core logger
	LoggerHandle loggerHandle;

	std::shared_ptr<ConfigDirectories> directories;
	
};