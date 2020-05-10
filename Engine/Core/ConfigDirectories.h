#pragma once

#include "FileSystem.h"

struct ConfigDirectories
{
	RCP::path RootSourceDirectory;
	RCP::path RootBinaryDirectory;
	RCP::path RootGameSourceDirectory;
	RCP::path RootGameBinaryDirectory;
	RCP::path EngineSourceDirectory;
	RCP::path PluginSourceDirectory;
	RCP::path PythonToolsDirectory;
};
