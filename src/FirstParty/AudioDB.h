//
//  AudioDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/21/24.
//  Provides access to all of the images being used in the game.

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
#endif /* AudioDB_h */
