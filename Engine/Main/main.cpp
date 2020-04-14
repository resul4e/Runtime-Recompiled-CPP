#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

#include <iostream>
#include "Windows.h"
#include <direct.h>
#include <filesystem>

#include "Level.h"
#include "PluginLoader.h"
#include "ConfigDirectories.h"

#include "Rendering/RenderEngineAPI.h"
#include "Input/InputComponent.h"

using std::cout;
using std::endl;

///Changes the working directory to be the Engine folder
std::string SetWorkingDirectory();
//Set the directories needed throughout the project.
std::shared_ptr<ConfigDirectories> SetConfigDirectories();

int main(int argc, char* argv[])
{
	std::shared_ptr<ConfigDirectories> directories = SetConfigDirectories();
	
	//deltatime
	float oldTime = 0;
	float newTime = 0.016f;
	float deltaTime = 0;

	float accumulatedTime = 0;
	const double fixedTime = 1.f/60.f;

	//restarting level
	bool restartLevel = false;
	
	//load all of the plugins
	PluginLoader pl{directories};
	pl.LoadPlugins();
	pl.Start();

	//create the Level class
	std::shared_ptr<Level> lvl = CreateLevel(directories.get());

	//start the level class.
	lvl->Start();

	while (!RenderEngineAPI::GetIsWindowClosed())
	{
		//check if the use wants to restart
		if(InputComponent::GetKeyPressed(InputComponent::Key::R))
		{
			restartLevel = true;
		}

		if(restartLevel)
		{
			pl.Delete();
			lvl->Restart();
			restartLevel = false;
		}
		

		//calculate new deltatime
		oldTime = newTime;
		newTime = static_cast<float>(clock())/CLOCKS_PER_SEC;
		deltaTime = newTime - oldTime;

		//check if deltatime is way too big, and if so just set it to 16 milliseconds
		if(deltaTime > 0.128f)
		{
			deltaTime = 0.016f;
		}

		accumulatedTime += deltaTime;

		//update the level class
		lvl->Update(deltaTime);

		//update the pluginLoader
		pl.Update(deltaTime);

		//update the level class in fixed intervals
		while (accumulatedTime > fixedTime)
		{
			lvl->FixedUpdate();
			pl.FixedUpdate();
			accumulatedTime -= fixedTime;
		}
	}

	lvl->Delete();
	pl.Delete();

	return 0;
}

std::shared_ptr<ConfigDirectories> SetConfigDirectories()
{
	std::shared_ptr<ConfigDirectories> directories = std::make_shared<ConfigDirectories>();
	
	directories->RootSourceDirectory = { std::string{ SOURCE_DIR } };
	directories->RootBinaryDirectory  = { std::string{ BINARY_DIR } };
	directories->RootGameSourceDirectory = directories->RootSourceDirectory / GAME_NAME;
	directories->RootGameBinaryDirectory = directories->RootBinaryDirectory / GAME_NAME;
	directories->EngineSourceDirectory = { directories->RootSourceDirectory / "Engine" };
	directories->PluginSourceDirectory = { directories->EngineSourceDirectory / "Plugins" };
	directories->PythonToolsDirectory = { directories->EngineSourceDirectory / "Tools" };

	return directories;
}
