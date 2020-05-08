#include <gtest/gtest.h>
#include "Level.h"
#include "Logger.h"

class LevelTests : public ::testing::Test {
protected:
	void SetUp() override
	{
		Logger::AddOrGet("core");
		Logger::AddOrGet("levelTest");

		//make sure that if we get a warning or above, we throw an exception.
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::WARN_AND_ABOVE);
		
		configDir = std::make_shared<ConfigDirectories>();
		configDir->RootSourceDirectory = { std::string{ SOURCE_DIR } };
		configDir->RootBinaryDirectory = { std::string{ BINARY_DIR } };
		configDir->EngineSourceDirectory = { configDir->RootSourceDirectory / "Engine" };
		configDir->PluginSourceDirectory = { configDir->EngineSourceDirectory / "Plugins" };
		configDir->PluginWhiteListDirectory = "Test";
	}

	void TearDown() override
	{
		//Reset threshold for other tests.
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::NEVER);

		if(Logger::Find("script"))
		{
			Logger::Remove(Logger::Get("script"));
		}
	}

	std::shared_ptr<ConfigDirectories> configDir;
};

TEST_F(LevelTests, Creator)
{
	
	EXPECT_NO_FATAL_FAILURE(std::shared_ptr<Level> lvl = CreateLevel(configDir.get()););
}

TEST_F(LevelTests, CreatorNoConfigDirectory)
{
	EXPECT_THROW(std::shared_ptr<Level> lvl = CreateLevel(nullptr), LoggerException);
}

TEST_F(LevelTests, Start)
{
	//std::shared_ptr<Level> lvl = CreateLevel(nullptr);
	//lvl->Start();
}