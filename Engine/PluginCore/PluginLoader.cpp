#include "PluginLoader.h"
#include "PluginBase.h"
#include <iostream>
#include "ConfigDirectories.h"
#include "SharedLibrary.h"

using namespace std::filesystem;
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
	for (auto p : directory_iterator(directories->PluginSourceDirectory))
	{
		if(!is_directory(p))
		{
			continue;
		}
		
		std::string DLLName = p.path().stem().string();
		if(!LoadDLL(DLLName))
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

bool PluginLoader::LoadDLL(std::string aDLLName)
{
	const path libraryPath = (directories->RootBinaryDirectory / "bin" / std::string(CMAKE_INTDIR) / (aDLLName + ".dll"));

	SharedLibrary library;
	library.LoadSharedLibrary(libraryPath.string());
	CREATEFUNCTION tempCreate = library.GetExportedFunction<CREATEFUNCTION>("HelloWorld");
	
	return true;
	/*
	const path dll = (directories->RootBinaryDirectory / "bin" / std::string(CMAKE_INTDIR) / (aDLLName + ".dll"));
	HINSTANCE tempDLL = LoadLibraryA(dll.string().c_str());
	if (tempDLL == nullptr)
	{
		const int errorCode = GetLastError();
		cout << "Couldn't load the DLL of the plugin " << aDLLName << ". Exited with error code " << errorCode << endl;
		return false;
	}
	
	CREATEFUNCTION tempCreate = reinterpret_cast<CREATEFUNCTION>(GetProcAddress(tempDLL, "CreatePlugin"));
	if (tempCreate == nullptr)
	{
		cout << "Couldn't load the CreatePlugin() function make sure you have added the START_PLUGIN and END_PLUGIN macros to the plugin " << aDLLName << endl;
		return false;
	}
	
	DELETEFUNCTION tempDelete = reinterpret_cast<DELETEFUNCTION>(GetProcAddress(tempDLL, "DeletePlugin"));
	if (tempDelete == nullptr)
	{
		cout << "Couldn't load the DeletePlugin() function make sure you have added the START_PLUGIN and END_PLUGIN macros to the plugin " << aDLLName << endl;
		return false;
	}
	
	DLLList.insert(std::pair<std::string, HINSTANCE>(aDLLName, tempDLL));
	CreatePluginList.insert(std::pair<std::string, CREATEFUNCTION>(aDLLName, tempCreate));
	DeletePluginList.insert(std::pair<std::string, DELETEFUNCTION>(aDLLName, tempDelete));
	return true;
	*/
}
