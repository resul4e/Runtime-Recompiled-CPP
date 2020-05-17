#include <gtest/gtest.h>
#include "Level.h"
#include "Logger.h"

class LevelTests : public ::testing::Test {
protected:
	void SetUp() override
	{
		Logger::AddOrGet("core");
		Logger::AddOrGet("levelTest");

		//make sure that if we get a error or above, we throw an exception.
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::ERROR_AND_ABOVE);
		
		configDir = std::make_shared<ConfigDirectories>();
		configDir->RootSourceDirectory = { std::string{ SOURCE_DIR } };
		configDir->RootBinaryDirectory = { std::string{ BINARY_DIR } };
		configDir->EngineSourceDirectory = { configDir->RootSourceDirectory / "Engine" };
		configDir->PluginSourceDirectory = { configDir->EngineSourceDirectory / "Plugins" };
		configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "Tools" };
		configDir->RootGameSourceDirectory = { RCP::fs::path{TEST_DATA_DIR} / "Level" / "Game" };
		configDir->RootGameBinaryDirectory = { configDir->RootBinaryDirectory / "Engine" / "Tests" / "Game" };
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
	
	EXPECT_NO_FATAL_FAILURE(CreateLevel(configDir.get()););
}

TEST_F(LevelTests, CreatorNoConfigDirectory)
{
	EXPECT_THROW(CreateLevel(nullptr), LoggerException);
}

TEST_F(LevelTests, Start)
{
	//everything that calls Start() should have its own build directory to not step on anyones toes.
	configDir->RootGameBinaryDirectory /= "Start";
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
}

TEST_F(LevelTests, Update)
{
	configDir->RootGameBinaryDirectory /= "Update";
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->Update(0.016f);
}

TEST_F(LevelTests, UpdateWithNegativeDeltaTime)
{
	configDir->RootGameBinaryDirectory /= "UpdateWithNegativeDeltaTime";
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->Update(-0.016f);
}

TEST_F(LevelTests, FixedUpdate)
{
	configDir->RootGameBinaryDirectory /= "FixedUpdate";
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->FixedUpdate();
}

TEST_F(LevelTests, Delete)
{
	configDir->RootGameBinaryDirectory /= "Delete";
	std::shared_ptr<Level> lvl = CreateLevel(configDir.get());
	lvl->Start();
	lvl->Delete();
}