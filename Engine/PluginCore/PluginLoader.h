#pragma once
#include <unordered_map>

#include "FunctionDefinition.h"

#include "ConfigDirectories.h"

//forward declaration
class PluginBase;
class SharedLibrary;

//function definitions
typedef PluginBase* (FUNCTION_CDECL *CREATE_FUNCTION)();
typedef void(FUNCTION_CDECL *DELETE_FUNCTION)(PluginBase*);

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

	/**
	 * \brief Returns a vector of all currently loaded plugins.
	 * \return The list of loaded plugins.
	 */
	std::vector<std::string> GetLoadedPlugins();

private:
	/**
	 * \brief Loads the DLL for the plugin.
	 * \param aSharedLibraryName The name of the DLL to load. It will search in the output directory of this VS project
	 */
	bool LoadPlugin(std::string aSharedLibraryName);

	/**
	 * \brief Loads the blacklist and whitelist.
	 */
	void LoadBlackAndWhiteLists();
	
	/**
	 * \brief Checks if the plugin is allowed by the blacklist and whitelist.
	 * \param aPluginName The plugin to check for validity.
	 * \return If the plugin is valid or not.
	 */
	bool IsPluginAllowed(std::string aPluginName);

//variables
public:
private:

	///A list of all of the loaded plugins.
	std::unordered_map<std::string, std::shared_ptr<PluginBase>> pluginList;
	///A list of all of the DLLs loaded. 
	std::unordered_map<std::string, std::unique_ptr<SharedLibrary>> SharedLibraryList;

	//Both of these functions gets plugged into a shared pointer.
	///The function that returns a pointer to the script Plugin
	std::unordered_map<std::string, CREATE_FUNCTION> CreatePluginList;
	///The function that deletes a plugin using its pointer.
	std::unordered_map<std::string, DELETE_FUNCTION> DeletePluginList;

	//A struct of all important directories
	std::shared_ptr<ConfigDirectories> directories;

	//A list of plugins that are allowed(whitelist) or not allowed(blacklist)
	std::vector<std::string> blackListedPlugins;
	std::vector<std::string> whitelistedPlugins;
};

