#include "PluginLoader.h"
#include "PluginBase.h"
#include <iostream>
#include "ConfigDirectories.h"
#include "SharedLibrary.h"
#include "Logger.h"

using std::cout;
using std::endl;

PluginLoader::PluginLoader(std::shared_ptr<ConfigDirectories> _directories) :
	directories(_directories)
{
}


PluginLoader::~PluginLoader()
{
}

void PluginLoader::LoadPlugins()
{
	for (auto p : RCP::directory_iterator(directories->PluginSourceDirectory))
	{
		if (!is_directory(p))
		{
			continue;
		}

		std::string DLLName = p.path().stem().string();
		if (!LoadPlugin(DLLName))
		{
			continue;
		}
		pluginList.insert(std::pair<std::string, std::shared_ptr<PluginBase>>(DLLName, std::shared_ptr<PluginBase>(CreatePluginList.at(DLLName)(), DeletePluginList.at(DLLName))));
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

bool PluginLoader::LoadPlugin(std::string aSharedLibraryName)
{
	const RCP::path libraryPath = (RCP::path("bin") / std::string(CMAKE_INTDIR) / aSharedLibraryName);

	std::unique_ptr<SharedLibrary> library = std::make_unique<SharedLibrary>(directories->RootBinaryDirectory.string());
	library->LoadSharedLibrary(libraryPath.string());

	CREATEFUNCTION tempCreate = library->GetExportedFunction<CREATEFUNCTION>("CreatePlugin");
	if (tempCreate == nullptr)
	{
		LOG_ERROR(Logger::Get("core"), "Couldn't load the CreatePlugin() function make sure you have added the START_PLUGIN and END_PLUGIN macros to the plugin " + aSharedLibraryName);
		return false;
	}

	DELETEFUNCTION tempDelete = library->GetExportedFunction<DELETEFUNCTION>("DeletePlugin");
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
