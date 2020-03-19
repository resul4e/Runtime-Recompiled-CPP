#pragma once
#include "ExportDetails.h"
#include "Transform/Vector.h"

#include <memory>
#include <unordered_map>
#include <filesystem>

//forward declare
namespace sf
{
	class Sound;
	class SoundBuffer;
	class Music;
	class SoundSource;
}

struct AudioCompHandle;

/**
 * \brief Class that acts as the API between the game code and the plugin.
 * 
 * Supports Sounds and Music. The main difference between these two is the length.
 * If a sound is too long it should be added as a Music instead of a Sound.
 * This is because music is streamed from disk and sound is loaded in its entirety in memory.
 */
class AudioComponent
{
	AudioComponent(){};
	~AudioComponent() {};
public:
	/**
	 * \brief The status of the AudioComponent
	 */
	enum class AudioStatus
	{
		Stopped, ///<The audio is reset to the start of audio file and not playing.
		Paused,	///<The audio is not playing, but will continue from the stopped point.
		Playing ///< The audio is playing.
	};

	/**
	 * \brief Adds a sound, this should only be used for small audio files. 
	 *			Large audio files should be loaded using AddMusic()
	 * \param aAudioFile The absolute path to the sound file.
	 * \return A handle to the audio component.
	 * \see AddMusic()
	 */
	AUDIO_API static AudioCompHandle AddSound(const char* aAudioFile);
	/**
	 * \brief Adds new music, this should be used for large audio files that otherwise would take seconds to load.
	 * \param aAudioFile 
	 * \return A handle to the audio component.
	 * \see AddSound()
	 */
	AUDIO_API static AudioCompHandle AddMusic(const char* aAudioFile);

	/**
	 * \brief Sets the volume of the AudioComponent.
	 * \param aHandle The AudioComponent you want to change.
	 * \param aVolume The volume of the audio, can be set between 0-100.
	 */
	AUDIO_API static void SetVolume(AudioCompHandle aHandle, float aVolume);
	/**
	 * \brief Gets the volume of the AudioComponent.
	 * \param aHandle	The AudioComponent you want get the volume of.
	 * \return			The volume in the range of 0-100.
	 */
	AUDIO_API static float GetVolume(AudioCompHandle aHandle);
	/**
	 * \brief Sets if the audio should loop.
	 * \param aHandle		The AudioComponent you want to loop
	 * \param aIsLooping	If the audio should loop back to the beginning at the end of the playing it.
	 */
	AUDIO_API static void SetLoop(AudioCompHandle aHandle, bool aIsLooping);
	/**
	 * \brief Gets if the audio should loop.
	 * \param aHandle		The AudioComponent you want to loop.
	 * \return				If the audio should loop back to the beginning at the end of the playing it.
	 */
	AUDIO_API static bool GetLoop(AudioCompHandle aHandle);

	/**
	 * \brief Sets a new offset for the AudioComponent.
	 * \param aHandle		The AudioComponent you want to offset.
	 * \param aMilliseconds The new offset of the audio in milliseconds.
	 */
	AUDIO_API static void SetPlayingOffset(AudioCompHandle aHandle, int aMilliseconds);
	/**
	 * \brief Gets the offset of the AudioComponent.
	 * \param aHandle The AudioComponent you want the offset from.
	 * \return The offset of the audio in milliseconds.
	 */
	AUDIO_API static int GetPlayingOffset(AudioCompHandle aHandle);

	/**
	 * \brief Play the specified AudioComponent.
	 * \param aHandle The AudioComponent you want to play.
	 */
	AUDIO_API static void Play(AudioCompHandle aHandle);
	/**
	 * \brief Pauses the specified AudioComponent.
	 * \param aHandle The AudioComponent you want to Pause.
	 * \note Pausing will not reset the Offset of the AudioComponent.
	 */
	AUDIO_API static void Pause(AudioCompHandle aHandle);
	/**
	 * \brief Stops the specified AudioComponent.		
	 * \param aHandle The AudioComponent you want to stop.
	 * \note Stopping will set the Offset of the AudioComponent to 0.
	 */
	AUDIO_API static void Stop(AudioCompHandle aHandle);
	/**
	 * \brief Gets the status of the AudioComponent.
	 * \param aHandle The AudioComponent you want the status from.
	 * \return If the AudioComponent is playing, stopped of paused.
	 */
	AUDIO_API static AudioStatus GetStatus(AudioCompHandle aHandle);
private:

public:
private:
	friend class AudioPlugin;
	///The SoundBuffers
	static std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBufferList;
	///The SoundSources, this includes both the music and the sounds
	static std::vector<std::shared_ptr<sf::SoundSource>> soundSourceList;
};
