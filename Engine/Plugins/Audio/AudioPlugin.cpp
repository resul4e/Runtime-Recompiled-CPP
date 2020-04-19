#include "AudioPlugin.h"
#include "AudioComponent.h"
#include "SFML/Audio.hpp"
#include "Logger.h"

#if !defined(WIN32) || !defined(_WIN32)
#define __noop(x) (void)(x)
#endif

void AudioPlugin::Start()
{
	Logger::Add("Audio");
}

void AudioPlugin::FixedUpdate()
{
}

void AudioPlugin::Update(float aDeltatime)
{
	__noop(aDeltatime);
}

void AudioPlugin::Delete()
{
	AudioComponent::soundSourceList.clear();
	AudioComponent::soundBufferList.clear();
}

void AudioPlugin::SetConfigDirectories(std::shared_ptr<ConfigDirectories> aDirectories)
{
	AudioComponent::configDirectories = aDirectories;
}

PLUGIN_END(AudioPlugin)
