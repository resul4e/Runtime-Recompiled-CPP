#pragma once

#include "FileSystem.h"

struct ConfigDirectories
{
	RCP::fs::path RootSourceDirectory;
	RCP::fs::path RootBinaryDirectory;
	RCP::fs::path RootGameSourceDirectory;
	RCP::fs::path RootGameBinaryDirectory;
	RCP::fs::path EngineSourceDirectory;
	RCP::fs::path PluginSourceDirectory;
	RCP::fs::path PluginWhiteListDirectory;
	RCP::fs::path PluginBlackListDirectory;
	RCP::fs::path PythonToolsDirectory;	
};
