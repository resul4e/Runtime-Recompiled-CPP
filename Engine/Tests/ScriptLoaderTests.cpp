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
		configDir->PythonToolsDirectory = { configDir->EngineSourceDirectory / "Tools" };
		configDir->RootGameSourceDirectory = { RCP::fs::path{TEST_DATA_DIR} / "Level" / "Game" };
		configDir->RootGameBinaryDirectory = { configDir->RootBinaryDirectory / "Engine" / "Tests" / "Game" };
		configDir->RootGameBinaryDirectory /= info->name();


		
		level = CreateLevel(configDir.get());
	}

	void TearDown() override
	{
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