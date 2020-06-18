#include "InputPlugin.h"
#include <iostream>
#include "Rendering/RenderEngineAPI.h"
#include "InputComponent.h"
#include "NoopFunctions.h"

InputPlugin::InputPlugin()
{
	
}

InputPlugin::~InputPlugin()
{
}

void InputPlugin::Start()
{
	InputComponent::window = RenderEngineAPI::GetRenderWindow();
}

void InputPlugin::FixedUpdate()
{
}

void InputPlugin::Update(float aDeltatime)
{
	__noop(aDeltatime);
}

void InputPlugin::Delete()
{
}

PLUGIN_END(InputPlugin)