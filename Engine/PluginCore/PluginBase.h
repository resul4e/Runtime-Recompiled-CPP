#pragma once
#include <memory>
#include "Handle.h"
#include "ExportDetails.h"

/**
 * \brief This Macro makes sure that the engine can access your plugin.
 * \param className The name of the class that you want to export
 */
#if defined(WIN32) || defined(_WIN32)
#define PLUGIN_END(className)\
extern "C" __declspec(dllexport) PluginBase* CreatePlugin()\
{\
	return new className();\
}\
extern "C" __declspec(dllexport) void DeletePlugin(PluginBase* plg)\
{\
	delete plg;\
	plg = nullptr;\
}
#else
//TODO(Resul): make this work for linux
#define PLUGIN_END(className)
#endif

struct ConfigDirectories;
class Object;

/**
 * \brief This is the base class of all of the Plugins
 * 
 * Usage:
 *	- Derive a new class from this class.
 *	- Implement the following methods:
 *		- Start()
 *		- FixedUpdate()
 *		- Update()
 *		- Delete()
 *	- Use this class to Start, Update, and Delete your own classes
 *	
 *	\see PluginBase for when each method is called.
*/
class PluginBase
{
public:
	PluginBase();
	virtual ~PluginBase();

	/**
	 * \brief This method is called at startup before the scripts are loaded in and started.
	 */
	virtual void Start() =0;
	/**
	 * \brief This method is called during a set timeframe (see main.cpp for frametime).
	 */
	virtual void FixedUpdate() =0;
	/**
	 * \brief This method is called every time the frame is updated.
	 * \param deltatime The time that elapsed between this frame and the previous frame
	 */
	virtual void Update(float deltatime) = 0;
	/**
	 * \brief This method is called when the game is reset and when the engine closes down.
	 */
	virtual void Delete() = 0;

	/**
	 * \brief This function can be overridden if the plugin needs to know important directories.
	 * \param aDirectories The struct that contains all directories to important directories
	 */
	virtual void SetConfigDirectories(std::shared_ptr<ConfigDirectories> aDirectories);
};

