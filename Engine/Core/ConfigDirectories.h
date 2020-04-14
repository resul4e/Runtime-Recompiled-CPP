#pragma once
#include <filesystem>

struct ConfigDirectories
{
	std::filesystem::path RootSourceDirectory;
	std::filesystem::path RootBinaryDirectory;
	std::filesystem::path RootGameSourceDirectory;
	std::filesystem::path RootGameBinaryDirectory;
	std::filesystem::path EngineSourceDirectory;
	std::filesystem::path PluginSourceDirectory;
	std::filesystem::path PythonToolsDirectory;
};
