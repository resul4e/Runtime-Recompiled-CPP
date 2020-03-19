#include "AudioPlugin.h"
#include "AudioComponent.h"
#include "SFML/Audio.hpp"
#include "Logger.h"

void AudioPlugin::Start()
{
	Logger::Add("Audio");
}

void AudioPlugin::FixedUpdate()
{
}

void AudioPlugin::Update(float aDeltatime)
{
	UNREFERENCED_PARAMETER(aDeltatime);
}

void AudioPlugin::Delete()
{
	AudioComponent::soundSourceList.clear();
	AudioComponent::soundBufferList.clear();
}

PLUGIN_END(AudioPlugin)