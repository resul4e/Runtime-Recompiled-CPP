#include "InputPlugin.h"
#include <iostream>
#include "Rendering/RenderEngineAPI.h"
#include "InputComponent.h"


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
	_CRT_UNUSED(aDeltatime);
}

void InputPlugin::Delete()
{
}

PLUGIN_END(InputPlugin)