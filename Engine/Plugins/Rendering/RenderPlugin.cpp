#include "RenderPlugin.h"
#include <iostream>
#include "Window.h"

#include "Logger.h"

#ifdef ENGINE_PROJECT
RenderPlugin::RenderPlugin()
{
	Logger::Add("Rendering");
	window = std::make_shared<Window>();
}

void RenderPlugin::Start()
{
	window->Start();
}

void RenderPlugin::FixedUpdate()
{
}

///\todo figure out if I need deltatime
void RenderPlugin::Update(float aDeltatime)
{
	window->Update(aDeltatime);
}

void RenderPlugin::Delete()
{
	window->Delete();
}
#endif

PLUGIN_END(RenderPlugin)

