#include <gtest/gtest.h>
#include "ScriptLoader.h"
#include "SharedLibrary.h"
#include "Level.h"
#include "CustomMacros.h"

class ScriptLoaderTests : public ::testing::Test
{
protected:
	void SetUp() override
	{	
		Logger::AddOrGet("core");

		//make sure that if we get a error or above, we throw an exception.
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::ERROR_AND_ABOVE);

		const testing::TestInfo* info = ::testing::UnitTest::GetInstance()->current_test_info();
		configDir = std::make_shared<ConfigDirectories>();
		configDir->RootSourceDirectory = { std::string{ SOURCE_DIR } };
		configDir->RootBinaryDirectory = { std::string{ BINARY_DIR } };
		configDir->EngineSourceDirectory = { configDir->RootSourceDirectory / "Engine" };
		configDir->PluginSourceDirectory = { configDir->EngineSourceDirectory / "Plugins" };
#if defined(WIN32) || defined(__WIN32)
		configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "Tools" };
#else
		configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "Tools" / "Linux" };
#endif
		configDir->RootGameSourceDirectory = { RCP::fs::path{TEST_DATA_DIR} / "ScriptLoader" / "Game" };
		configDir->RootGameBinaryDirectory = { configDir->RootBinaryDirectory / "Engine" / "Tests" / "Game" };
		configDir->RootGameBinaryDirectory /= std::string(info->test_case_name()) + info->name();

		//This setup is a subset of what happens in Level::Start(). Hopefully this does not stay too long and I can implement a
		//Scriptmanager class that does this instead of the level class.
		level = CreateLevel(configDir.get());
		level->SetupDirectories();
		level->scriptLoader = std::make_unique<ScriptLoader>(level->thisLvl, 0);
	}

	void TearDown() override
	{
		configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "Tools" };
		configDir->RootGameSourceDirectory = { RCP::fs::path{TEST_DATA_DIR} / "ScriptLoader" / "Game" };
		
		//Reset threshold for other tests.
		Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::NEVER);

		if (Logger::Find("script"))
		{
			Logger::Remove(Logger::Get("script"));
		}
	}

	std::shared_ptr<ConfigDirectories> configDir;
	std::shared_ptr<Level> level;
};

TEST_F(ScriptLoaderTests, Constructor)
{
	EXPECT_NO_FATAL_FAILURE(ScriptLoader loader(level, 0));
}

TEST_F(ScriptLoaderTests, ConstructorWhereLevelIsNullptr)
{
	EXPECT_THROW_WITH_MESSAGE(ScriptLoader loader(nullptr, 0), LoggerException, "The level pointer was null. This will probably crash!");
}

TEST_F(ScriptLoaderTests, Start)
{
	level->scriptLoader->Start();
}

TEST_F(ScriptLoaderTests, StartWithEmptyDirectory)
{
	//It is a warning so we have to bump the threshold
	Logger::SetExceptionThreshold(Logger::Get("core"), Logger::ExceptionThreshold::WARN_AND_ABOVE);
	
	configDir->RootGameSourceDirectory = { RCP::fs::path{TEST_DATA_DIR} / "ScriptLoader" / "StartWithEmptyDirectory" };
	std::string errorMessage = "No scripts detected in the folder " + configDir->RootGameSourceDirectory.string() + "/Scripts";
	EXPECT_THROW_WITH_MESSAGE(level->scriptLoader->Start(), LoggerException, errorMessage.c_str())
}

TEST_F(ScriptLoaderTests, StartWithWrongToolsFolder)
{
	configDir->RootGameSourceDirectory = { RCP::fs::path{TEST_DATA_DIR} / "ScriptLoader" / "StartWithWrongToolsFolder" };
	configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "WRONG_TOOLS_FOLDER" };
	std::string errorMessage = "error in compilation of script with type Game";
	EXPECT_THROW_WITH_MESSAGE(level->scriptLoader->Start(), LoggerException, errorMessage.c_str())
}

TEST_F(ScriptLoaderTests, Update)
{
	level->scriptLoader->Start();
	level->scriptLoader->Update(0.016f);
}

TEST_F(ScriptLoaderTests, UpdateWithNegativeDeltaTime)
{
	level->scriptLoader->Start();
	level->scriptLoader->Update(-0.016f);
}

TEST_F(ScriptLoaderTests, Restart)
{
	level->scriptLoader->Start();
	level->scriptLoader->Restart();
}
