#include <gtest/gtest.h>
#include "PluginLoader.h"
#include "SharedLibrary.h"
#include "FunctionDefinition.h"

typedef int (FUNCTION_CDECL* START_CALL_AMOUNT_FUNCTION)();
typedef float (FUNCTION_CDECL* LAST_DELTA_TIME_FUNCTION)();

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

	void LoadPlugin()
	{
		plLoader = std::make_shared<PluginLoader>(configDir);
		plLoader->LoadPlugins();
	}

	std::shared_ptr<PluginLoader> plLoader;
	std::shared_ptr<ConfigDirectories> configDir;
};

TEST_F(PluginLoaderTests, Constructor)
{
	EXPECT_NO_FATAL_FAILURE(PluginLoader loader(configDir));
}

TEST_F(PluginLoaderTests, LoadPlugins)
{
	PluginLoader loader(configDir);
	EXPECT_NO_FATAL_FAILURE(loader.LoadPlugins());
	std::vector<std::string> plugins = loader.GetLoadedPlugins();
	EXPECT_EQ(plugins.size(), 1);
	EXPECT_STREQ(plugins.at(0).c_str(), "TestPlugin");
}

TEST_F(PluginLoaderTests, Start)
{
	LoadPlugin();

	//load the shared lib separately so that we can call monitoring functions.
	SharedLibrary lib(configDir->RootBinaryDirectory);
	lib.LoadSharedLibrary(std::string("bin/")+ BUILD_CONFIG_NAME +"/TestPlugin");
	const auto StartCallAmount = lib.GetExportedFunction<START_CALL_AMOUNT_FUNCTION>("GetStartCallAmount");

	//check that we have a valid connection to the shared lib.
	int amount = StartCallAmount();
	EXPECT_EQ(amount, 0);

	//call the start function
	plLoader->Start();

	//check that start was called.
	amount = StartCallAmount();
	EXPECT_EQ(amount, 1);
}

TEST_F(PluginLoaderTests, Update)
{
	LoadPlugin();

	//load the shared lib separately so that we can call monitoring functions.
	SharedLibrary lib(configDir->RootBinaryDirectory);
	lib.LoadSharedLibrary(std::string("bin/") + BUILD_CONFIG_NAME + "/TestPlugin");
	const auto LastDeltaTime = lib.GetExportedFunction<LAST_DELTA_TIME_FUNCTION>("GetLastDeltaTime");

	//check that we have a valid connection to the shared lib.
	float amount = LastDeltaTime();
	EXPECT_FLOAT_EQ(amount, -1);

	//call the start function
	plLoader->Update(0.45321f);

	//check that start was called.
	amount = LastDeltaTime();
	EXPECT_FLOAT_EQ(amount, 0.45321f);
}

