#pragma once
#include <unordered_map>

#include "FunctionDefinition.h"

#include "ConfigDirectories.h"

//forward declaration
class PluginBase;
class SharedLibrary;

//function definitions
typedef PluginBase* (__cdecl *CREATEFUNCTION)();
typedef void(__cdecl*DELETEFUNCTION)(PluginBase*);

/**
*	\brief The class that loads all of the plugins.
*/
class PluginLoader
{
public:
	PluginLoader(std::shared_ptr<ConfigDirectories> _directories);
	~PluginLoader();

	/**
	 * \brief Loads all of the plugins in the given folder.
	 * 
	 * It finds all folders inside of the PluginFolder. It will try to load a DLL with the same name as the folder.
	 * If it can't find a DLL with the name of the folder it will print an error and return.
	 * 
	 * \warning Will not recursively search nested folders.
	 * \warning DLL name and Folder name should match!
	 */
	void LoadPlugins();

	/**
	 * \brief Goes through all of the plugins and starts them up.
	 */
	void Start();
	/**
	 * \brief Goes through each of the plugins and update them.
	 */
	void Update(float deltatime);
	/**
	 * \brief Goes through each of the plugins and updates them on a fixed interval.
	 */
	void FixedUpdate();
	/**
	 * \brief Goes through each of the plugins and delete them.
	 */
	void Delete();

private:
	/**
	 * \brief Loads the DLL for the plugin.
	 * \param aSharedLibraryName The name of the DLL to load. It will search in the output directory of this VS project
	 */
	bool LoadPlugin(std::string aSharedLibraryName);

//variables
public:
private:

	///A list of all of the loaded plugins.
	std::unordered_map<std::string, std::shared_ptr<PluginBase>> pluginList;
	///A list of all of the DLLs loaded. 
	std::unordered_map<std::string, std::unique_ptr<SharedLibrary>> SharedLibraryList;

	//Both of these functions gets plugged into a shared pointer.
	///The function that returns a pointer to the script Plugin
	std::unordered_map<std::string, CREATEFUNCTION> CreatePluginList;
	///The function that deletes a plugin using its pointer.
	std::unordered_map<std::string, DELETEFUNCTION> DeletePluginList;

	//A struct of all important directories
	std::shared_ptr<ConfigDirectories> directories;
};

