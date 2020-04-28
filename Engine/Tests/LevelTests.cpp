#include <gtest/gtest.h>
#include "Level.h"

class LevelTests : public ::testing::Test {
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

TEST_F(LevelTests, Creator)
{
	EXPECT_NO_FATAL_FAILURE(std::shared_ptr<Level> lvl = CreateLevel(configDir.get()));
}