#include "RenderPlugin.h"
#include <iostream>
#include "Window.h"

#include "Logger.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"

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

void RenderPlugin::SetConfigDirectories(std::shared_ptr<ConfigDirectories> aDirectories)
{
	SpriteComponent::directories = aDirectories;
	AnimationComponent::directories = aDirectories;
}
#endif

PLUGIN_END(RenderPlugin)

