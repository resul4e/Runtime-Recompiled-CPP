#include "PluginLoader.h"
#include "PluginBase.h"
#include <iostream>
#include "ConfigDirectories.h"
#include "SharedLibrary.h"
#include "Logger.h"
#include "SharedLibrary.h"

using std::cout;
using std::endl;

PluginLoader::PluginLoader(std::shared_ptr<ConfigDirectories> _directories) :
	directories(_directories)
{
	LoadBlackAndWhiteLists();
}


PluginLoader::~PluginLoader()
{
	for(auto plugin : pluginList)
	{
		plugin.second.reset();
	}
	pluginList.clear();

	std::unordered_map<std::string, std::unique_ptr<SharedLibrary>>::iterator it = SharedLibraryList.begin();
	while (it != SharedLibraryList.end())
	{
		it->second->UnloadSharedLibrary();
		it++;
	}
}

void PluginLoader::LoadPlugins()
{
	for (auto p : RCP::fs::directory_iterator(directories->PluginSourceDirectory))
	{
		if (!is_directory(p))
		{
			continue;
		}

		//Get the directory name as a string
		std::string sharedLibraryName = p.path().stem().string();
		//Check the whitelist and blacklist if this plugin is allowed
		if(!IsPluginAllowed(sharedLibraryName))
		{
			continue;
		}
		
		if (!LoadPlugin(sharedLibraryName))
		{
			continue;
		}
		pluginList.insert(std::pair<std::string, std::shared_ptr<PluginBase>>(sharedLibraryName, std::shared_ptr<PluginBase>(CreatePluginList.at(sharedLibraryName)(), DeletePluginList.at(sharedLibraryName))));
	}
}

void PluginLoader::Start()
{
	for (auto p : pluginList)
	{
		p.second->Start();
		p.second->SetConfigDirectories(directories);
	}
}

void PluginLoader::Update(float aDeltatime)
{
	for (auto p : pluginList)
	{
		p.second->Update(aDeltatime);
	}
}

void PluginLoader::FixedUpdate()
{
	for (auto p : pluginList)
	{
		p.second->FixedUpdate();
	}
}

void PluginLoader::Delete()
{
	for (auto p : pluginList)
	{
		p.second->Delete();
	}
}

std::vector<std::string> PluginLoader::GetLoadedPlugins()
{
	std::vector<std::string> loadedPlugins;
	for (auto plugin : pluginList)
	{
		loadedPlugins.push_back(plugin.first);
	}
	return loadedPlugins;
}

bool PluginLoader::LoadPlugin(std::string aSharedLibraryName)
{
	const RCP::fs::path libraryPath = (RCP::fs::path("bin") / (std::string(SharedLibrary::sharedLibraryPrefix) + aSharedLibraryName));
	
	std::unique_ptr<SharedLibrary> library = std::make_unique<SharedLibrary>(directories->RootBinaryDirectory);
	library->LoadSharedLibrary(libraryPath.string());

	CREATE_FUNCTION tempCreate = library->GetExportedFunction<CREATE_FUNCTION>("CreatePlugin");
	if (tempCreate == nullptr)
	{
		LOG_ERROR(Logger::Get("core"), "Couldn't load the CreatePlugin() function make sure you have added the START_PLUGIN and END_PLUGIN macros to the plugin " + aSharedLibraryName);
		return false;
	}

	DELETE_FUNCTION tempDelete = library->GetExportedFunction<DELETE_FUNCTION>("DeletePlugin");
	if (tempDelete == nullptr)
	{
		LOG_ERROR(Logger::Get("core"), "Couldn't load the DeletePlugin() function make sure you have added the START_PLUGIN and END_PLUGIN macros to the plugin " + aSharedLibraryName);
		return false;
	}

	SharedLibraryList.insert({ aSharedLibraryName, std::move(library) });
	CreatePluginList.insert({ aSharedLibraryName, tempCreate });
	DeletePluginList.insert({ aSharedLibraryName, tempDelete });

	return true;
}

void PluginLoader::LoadBlackAndWhiteLists()
{
	//TODO(Resul): Load from file on disk instead of hard coding.
	if(directories->PluginWhiteListDirectory.string() == "Test")
	{
		whitelistedPlugins.push_back("TestPlugin");
	}
	else
	{
		blackListedPlugins.push_back("TestPlugin");
	}
}

bool PluginLoader::IsPluginAllowed(std::string aPluginName)
{
	bool allowedByWhitelist = whitelistedPlugins.empty() || std::find(whitelistedPlugins.begin(), whitelistedPlugins.end(), aPluginName) != whitelistedPlugins.end();
	bool allowedByBlacklist = blackListedPlugins.empty() || std::find(blackListedPlugins.begin(), blackListedPlugins.end(), aPluginName) == blackListedPlugins.end();
	return allowedByWhitelist && allowedByBlacklist;
}
