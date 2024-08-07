//
//  AudioManager.cpp
//  wolverine_engine
//
//  Created by Cameron Reid 6/12/24.
//  Stores the definitions for all of the sdl2_mixer interface functions

#include <iostream>

#include "AudioManager.h"
#include "AudioDB.h"

/**
* Plays the audio file with 'clip_name' on channel 'channel' and loops 'num_loops' times.
* If 'channel' is -1, play on the first free channel.
* If 'num_loops' is -1 loop infinitely
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_PlayChannel
*
* @param	channel     the channel to play the audio file on
* @param	clip_name	the name of the audio file to play
* @param	num_loops	the number of times to loop the clip
*/
void AudioManager::AudioPlay(const int channel, const std::string& clip_name, const int num_loops)
{
    Mix_Chunk* mix_chunk = GetSound(clip_name);
    if (Mix_PlayChannel(channel, mix_chunk, num_loops) == -1) {
        std::cout << Mix_GetError();
    }
} // AudioPlay()

/**
* Halt playing of a particular channel, except for any playing music.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_HaltChannel
*
* @param	channel    channel to halt, or -1 to halt all channels
*/
void AudioManager::AudioHalt(const int channel)
{
    if (Mix_HaltChannel(channel) == -1) {
        std::cout << Mix_GetError();
    }
} // AudioHalt()

/**
* Pause a particular channel. Any music is unaffected.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Pause
*
* @param	channel    the channel to pause, or -1 to pause all channels.
*/
void AudioManager::AudioPause(const int channel)
{
    Mix_Pause(channel);
} // AudioPause()

/**
* Resume a particular channel. Any music is unaffected.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Resume
*
* @param	channel    the channel to resume, or -1 to resume all paused channels.
*/
void AudioManager::AudioResume(const int channel)
{
    Mix_Resume(channel);
} // AudioResume()

/**
* Sets the volume for a specific channel
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Volume
*
* @param	channel    the channel on set/query the volume on, or -1 for all channels
* @param	volume     the new volume, between 0 and MIX_MAX_VOLUME, or -1 to query
*
* @returns	Returns the previous volume. If the specified volume is -1, this returns
*           the current volume. If channel is -1, this returns the average of all channels.
*/
int AudioManager::AudioSetVolume(const int channel, const int volume)
{
    int previous_volume = Mix_Volume(channel, volume);
    return previous_volume;
} // AudioSetVolume()

/**
* Close the mixer, halting all playing audio.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_CloseAudio
*/
void AudioManager::AudioCloseMixer()
{
    Mix_CloseAudio();
} // AudioCLoseMixer()

/**
* Deinitialize SDL_mixer.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Quit
*/
void AudioManager::AudioDeinitMixer()
{
    Mix_Quit();
} // AudioDeinitMixer()

/**
* Set the master volume for all channels.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_MasterVolume
*
* @param	volume    the new volume, between 0 and MIX_MAX_VOLUME (128), or -1 to query.
*
* @returns	Returns the previous volume. If the specified volume is -1,
            this returns the current volume.
*/
void AudioManager::AudioSetMasterVolume(const int volume)
{
    Mix_MasterVolume(volume);
} // AudioSetMasterVolume()
