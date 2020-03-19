#include "PluginLoader.h"
#include "PluginBase.h"
#include <iostream>

using namespace std::filesystem;
using std::cout;
using std::endl;

PluginLoader::PluginLoader()
{
}


PluginLoader::~PluginLoader()
{
}

void PluginLoader::LoadPlugins(const path& PluginFolder)
{
	for (auto p : directory_iterator(PluginFolder))
	{
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
	std:: string dll = std::string(TARGET_DIRECTORY + aDLLName + ".dll");
	HINSTANCE tempDLL = LoadLibraryA(dll.c_str());
	if (tempDLL == nullptr)
	{
		cout << "Couldn't load the DLL of the plugin " << aDLLName << endl;
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
}
