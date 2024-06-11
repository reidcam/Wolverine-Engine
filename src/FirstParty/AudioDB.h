//
//  AudioDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/21/24.
//  Provides access to all of the images being used in the game.
//
//	See the following for all SDL2_mixer functions: https://wiki.libsdl.org/SDL2_mixer/CategoryAPI

#ifndef AudioDB_h
#define AudioDB_h

#include <string>

#include "SDL.h"
#include "SDL_mixer.h"

/**
 * Loads all of the audio in the resources/audio directory
*/
void LoadSounds();

/**
 * Get an audio clip based on the clips name
 *
 *`@param   audio_name  the name of the audio clip to get from the database
 * @returns             the audio clip with the specified name
*/
Mix_Chunk* GetSound(const std::string& audio_name);

/*
* Plays the audio file with 'clip_name' on channel 'channel' and loops 'num_loops' times.
* If 'channel' is -1, play on the first free channel.
* If 'num_loops' is -1 loop infinitely
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_PlayChannel
* 
* @param	channel     the channel to play the audio file on
* @param	clip_name	the name of the audio file to play
* @param	num_loops	the number of times to loop the clip
*/
static void AudioPlay(const int channel, const std::string& clip_name, const int num_loops);

/*
* Halts the audio on the specified channel.
* A 'channel' of -1 will halt all channels
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_HaltChannel
* 
* @param	channel    the channel to halt the audio on
*/
static void AudioHalt(const int channel);

/*
* Pause a particular channel.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Pause
* 
* @param	channel    the channel to pause, or -1 to pause all channels
*/
static void AudioPause(const int channel);

/*
* Resume a particular channel
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Resume
* 
* @param	channel    the channel to resume, or -1 to resume all paused channels
*/
static void AudioResume(const int channel);

/*
* Sets the volume for a specific channel
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Volume
* 
* @param	channel    the channel on set/query the volume on, or -1 for all channels
* @param	volume     the new volume, between 0 and MIX_MAX_VOLUME, or -1 to query
* 
* @returns	Returns the previous volume. If the specified volume is -1, this returns
			the current volume. If channel is -1, this returns the average of all channels.
*/
static int AudioSetVolume(const int channel, const int volume);

/*
* Close the mixer, halting all playing audio.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_CloseAudio
*/
static void AudioCloseMixer();

/*
* Deinitialize SDL_mixer.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Quit
*/
static void AudioDeinitMixer();

#endif /* AudioDB_h */
