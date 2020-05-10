#include "TestPlugin.h"

static int startCounter = 0;
static float lastDeltaTime = -1;

void TestPlugin::Start()
{
	startCounter++;
}

void TestPlugin::FixedUpdate()
{
}

void TestPlugin::Update(float deltatime)
{
	lastDeltaTime = deltatime;
}

void TestPlugin::Delete()
{
	startCounter = 0;
	lastDeltaTime = 0;
}

PLUGIN_END(TestPlugin)

extern "C" EXPORT_SHARED_LIBRARY_FUNCTION int GetStartCallAmount()
{
	return startCounter;
}

extern "C" EXPORT_SHARED_LIBRARY_FUNCTION float GetLastDeltaTime()
{
	return lastDeltaTime;
}