//
//  Databases.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/21/24.
//  Stores all of the resources being used in the game, and gives definitions to the database functions
//

#include <stdio.h>

#include <fstream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <filesystem>
#include <cstdlib>
#include <unordered_map>
#include <map>

#include "EngineUtils.h"
#include "ImageDB.h"
#include "AudioDB.h"
#include "Engine.h"

// Resources
std::unordered_map<std::string, SDL_Texture*> loaded_images; // All of the loaded images
std::unordered_map<std::string, Mix_Chunk*> loaded_sounds; // All of the loaded sounds

//-------------------------------------------------------
// Image Database

/**
 * Loads all of the images in the resources/images directory
*/
void LoadImages()
{
    /* Load files from this path */
    const std::string path = "resources/images";
    
    // Fills up the database if the path exists
    if (std::filesystem::exists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(path))
        {
            if (file.path() != path + "/.DS_Store")
            {
                SDL_Renderer* r = EngineData::renderer;
                SDL_Texture* img = IMG_LoadTexture(r, file.path().string().c_str());
                loaded_images[file.path().filename().stem().stem().string()] = img;
            }
        }
    }
} // LoadImages()

/**
 * Get an image from loaded_images based on the images name
 *
 *`@param   image_name  the name of the image to get from the database
 * @returns             the image with the specified name
*/
SDL_Texture* GetImage(std::string image_name)
{
    if (loaded_images.find(image_name) == loaded_images.end())
    {
        std::cout << "error: missing image " << image_name;
        exit(0);
    }
    
    return loaded_images[image_name];
} // GetImage()

//-------------------------------------------------------
// Audio Database

/**
 * Loads all of the audio in the resources/audio directory
*/
void LoadSounds()
{
    /* Load files from this path */
    const std::string path = "resources/audio";
    
    // Fills up the database if the path exists
    if (std::filesystem::exists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(path))
        {
            if (file.path() != path + "/.DS_Store")
            {
                Mix_OpenAudio(48000, AUDIO_S16SYS, 1, 1024);
                Mix_Chunk* sound = Mix_LoadWAV(file.path().string().c_str());
                loaded_sounds[file.path().filename().stem().stem().string()] = sound;
            }
        }
    }
    
    Mix_AllocateChannels(50);
} // LoadSounds();

/**
 * Get an audio clip based on the clips name
 *
 *`@param   audio_name  the name of the audio clip to get from the database
 * @returns             the audio clip with the specified name
*/
Mix_Chunk* GetSound(std::string audio_name)
{
    if (loaded_sounds.find(audio_name) == loaded_sounds.end())
    {
        std::cout << "error: missing audio clip " << audio_name;
        exit(0);
    }
    
    return loaded_sounds[audio_name];
} // GetSound()
