//
//  AudioManager.h
//  wolverine_engine
//
//  Created by Cameron Reid on 6/12/24.
//  Provides functions to utilize sdl2_mixer
//
//	See the following for all SDL2_mixer functions: https://wiki.libsdl.org/SDL2_mixer/CategoryAPI

#ifndef AudioManager_h
#define AudioManager_h

#include <string>

#include "SDL.h"
#include "SDL_mixer.h"

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
void AudioPlay(const int channel, const std::string& clip_name, const int num_loops);

/*
* Halt playing of a particular channel, except for any playing music.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_HaltChannel
*
* @param	channel    channel to halt, or -1 to halt all channels
*/
void AudioHalt(const int channel);

/*
* Pause a particular channel. Any music is unaffected.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Pause
*
* @param	channel    the channel to pause, or -1 to pause all channels.
*/
void AudioPause(const int channel);

/*
* Resume a particular channel. Any music is unaffected.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Resume
*
* @param	channel    the channel to resume, or -1 to resume all paused channels.
*/
void AudioResume(const int channel);

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
int AudioSetVolume(const int channel, const int volume);

/*
* Close the mixer, halting all playing audio.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_CloseAudio
*/
void AudioCloseMixer();

/*
* Deinitialize SDL_mixer.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Quit
*/
void AudioDeinitMixer();

/*
* Set the master volume for all channels.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_MasterVolume
*
* @param	volume    the new volume, between 0 and MIX_MAX_VOLUME (128), or -1 to query.
*
* @returns	Returns the previous volume. If the specified volume is -1,
			this returns the current volume.
*/
void AudioSetMasterVolume(const int volume);
#endif /* AudioManager_h */
