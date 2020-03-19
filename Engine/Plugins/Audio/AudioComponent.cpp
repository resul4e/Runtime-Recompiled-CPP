#include "AudioComponent.h"

#include "Handle.h"
#include "Logger.h"

#include "SFML/Audio.hpp"

std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> AudioComponent::soundBufferList;
std::vector<std::shared_ptr<sf::SoundSource>> AudioComponent::soundSourceList;

extern "C" AUDIO_API AudioCompHandle AudioComponent::AddSound(const char* aAudioFile)
{
	std::shared_ptr<sf::Sound> tempSound = std::make_shared<sf::Sound>();
	std::shared_ptr<sf::SoundBuffer> tempBuffer = std::make_shared<sf::SoundBuffer>();

	//check if the audio is already in the list
	//if not load a new one from the audioFile provided
	//if it is already in the list use the one stored
	if(soundBufferList.find(aAudioFile) == soundBufferList.end())
	{
		//check if there is an audiofile
		//if not return an error
		if (!tempBuffer->loadFromFile(aAudioFile))
		{
			LOG_ERROR(Logger::Get("Audio"), "No compatible audio file was found at {}", aAudioFile);
			return { 1231234 , "ERROR"};
		}

		//create a new soundbuffer and sound and save them.
		soundBufferList.insert(std::pair<std::string, std::shared_ptr<sf::SoundBuffer>>(aAudioFile, tempBuffer));
	}
	else
	{
		tempBuffer = soundBufferList.at(aAudioFile);
	}

	tempSound->setBuffer(*tempBuffer);
	soundSourceList.push_back(tempSound);
	return { soundSourceList.size() -1, aAudioFile };
}

extern "C" AUDIO_API AudioCompHandle AudioComponent::AddMusic(const char* aAudioFile)
{
	std::shared_ptr<sf::Music> tempMusic = std::make_shared<sf::Music>();
	if(!tempMusic->openFromFile(aAudioFile))
	{
		LOG_ERROR(Logger::Get("Audio"), "No compatible audio file was found at {}", aAudioFile);
		return { 1231234 };
	}

	soundSourceList.push_back(tempMusic);

	return { soundSourceList.size() -1 };
}

extern "C" AUDIO_API void AudioComponent::SetVolume(AudioCompHandle aHandle, float aVolume)
{
	soundSourceList.at(aHandle.index)->setVolume(aVolume);
}

extern "C" AUDIO_API float AudioComponent::GetVolume(AudioCompHandle aHandle)
{
	return soundSourceList.at(aHandle.index)->getVolume();
}

extern "C" AUDIO_API void AudioComponent::SetLoop(AudioCompHandle aHandle, bool aIsLooping)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if (tempSound != nullptr)
	{
		tempSound->setLoop(aIsLooping);
		return;
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if (tempMusic != nullptr)
	{
		tempMusic->setLoop(aIsLooping);
		return;
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
}

extern "C" AUDIO_API bool AudioComponent::GetLoop(AudioCompHandle aHandle)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if (tempSound != nullptr)
	{
		return tempSound->getLoop();
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if (tempMusic != nullptr)
	{
		return tempMusic->getLoop();;
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
	return false;
}

extern "C" AUDIO_API void AudioComponent::SetPlayingOffset(AudioCompHandle aHandle, int aMilliseconds)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if (tempSound != nullptr)
	{
		tempSound->setPlayingOffset(sf::milliseconds(aMilliseconds));
		return;
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if (tempMusic != nullptr)
	{
		tempMusic->setPlayingOffset(sf::milliseconds(aMilliseconds));
		return;
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
}

extern "C" AUDIO_API int AudioComponent::GetPlayingOffset(AudioCompHandle aHandle)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if (tempSound != nullptr)
	{
		
		return tempSound->getPlayingOffset().asMilliseconds();
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if (tempMusic != nullptr)
	{
		
		return tempMusic->getPlayingOffset().asMilliseconds();
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
	return -1;
}

extern "C" AUDIO_API void AudioComponent::Play(AudioCompHandle aHandle)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if(tempSound!= nullptr)
	{
		tempSound->play();
		return;
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if(tempMusic != nullptr)
	{
		tempMusic->play();
		return;
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
}

extern "C" AUDIO_API void AudioComponent::Pause(AudioCompHandle aHandle)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if (tempSound != nullptr)
	{
		tempSound->pause();
		return;
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if (tempMusic != nullptr)
	{
		tempMusic->pause();
		return;
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
}

extern "C" AUDIO_API void AudioComponent::Stop(AudioCompHandle aHandle)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if (tempSound != nullptr)
	{
		tempSound->stop();
		return;
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if (tempMusic != nullptr)
	{
		tempMusic->stop();
		return;
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
}

extern "C" AUDIO_API AudioComponent::AudioStatus AudioComponent::GetStatus(AudioCompHandle aHandle)
{
	std::shared_ptr<sf::Sound> tempSound = std::dynamic_pointer_cast<sf::Sound>(soundSourceList.at(aHandle.index));
	if (tempSound != nullptr)
	{
		return static_cast<AudioStatus>(tempSound->getStatus());
	}
	std::shared_ptr<sf::Music> tempMusic = std::dynamic_pointer_cast<sf::Music>(soundSourceList.at(aHandle.index));
	if (tempMusic != nullptr)
	{
		return static_cast<AudioStatus>(tempMusic->getStatus());
	}

	LOG_WARN(Logger::Get("Audio"), "Handle was neither a Sound or a Music")
	return AudioStatus::Stopped;
}
