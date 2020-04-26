#include <gtest/gtest.h>
#include "PluginLoader.h"

class PluginLoaderTests : public ::testing::Test {
protected:
	void SetUp() override {
		configDir = std::make_shared<ConfigDirectories>();
		configDir->RootSourceDirectory = { std::string{ SOURCE_DIR } };
		configDir->RootBinaryDirectory = { std::string{ BINARY_DIR } };
		configDir->EngineSourceDirectory = { configDir->RootSourceDirectory / "Engine" };
		configDir->PluginSourceDirectory = { configDir->EngineSourceDirectory / "Plugins" };
		configDir->PluginWhiteListDirectory = "Test";
	}

	std::shared_ptr<ConfigDirectories> configDir;
};

TEST_F(PluginLoaderTests, Constructor)
{
	EXPECT_NO_FATAL_FAILURE(PluginLoader loader(configDir));
}

TEST_F(PluginLoaderTests, LoadPlugins)
{
	PluginLoader loader(configDir);
	loader.LoadPlugins();
}