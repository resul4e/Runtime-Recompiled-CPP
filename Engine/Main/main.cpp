#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

#include <iostream>
#include "Windows.h"
#include <direct.h>
#include "Level.h"

#include "PluginLoader.h"
#include "Rendering/RenderEngineAPI.h"
#include "Input/InputComponent.h"

using std::cout;
using std::endl;

///Changes the working directory to be the Engine folder
std::string SetWorkingDirectory();

int main(int argc, char* argv[])
{
	//change working directory
	std::string workingDir = SetWorkingDirectory();

	//deltatime
	float oldTime = 0;
	float newTime = 0.016f;
	float deltaTime = 0;

	float accumulatedTime = 0;
	const double fixedTime = 1.f/60.f;

	//restarting level
	bool restartLevel = false;
	
	//load all of the plugins
	PluginLoader pl;
	pl.LoadPlugins(path("../../Plugins"));
	pl.Start();

	//create the Level class
	std::shared_ptr<Level> lvl = CreateLevel();
	//initialize the Start function with a standard gamePath
	if (argc < 2)
	{
		lvl->Start((workingDir + "../Game").c_str(), workingDir.c_str());
	}
	else
	{
		lvl->Start(argv[1], workingDir.c_str());
	}

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

std::string SetWorkingDirectory()
{
	char working_directory[MAX_PATH + 1];
	GetCurrentDirectoryA(sizeof(working_directory), working_directory); // **** win32 specific ****
	cout << working_directory << endl;
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
	std::string enginePath = std::string(working_directory) + std::string("/../Engine");
	//cout << enginePath << endl;
#ifdef _MSC_VER
#pragma warning(default:4996)
#endif
	_chdir(enginePath.c_str());

	GetCurrentDirectoryA(sizeof(working_directory), working_directory); // **** win32 specific ****
	cout << working_directory << endl;
	return std::string(working_directory);

}