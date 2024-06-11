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
#include "TextDB.h"
#include "TemplateDB.h"

#include "Engine.h"

// Resources
std::unordered_map<std::string, SDL_Texture*> loaded_images; // All of the loaded images
std::unordered_map<std::string, Mix_Chunk*> loaded_sounds; // All of the loaded sounds
std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> loaded_fonts; // All of the loaded fonts
std::unordered_map<std::string, rapidjson::Document*> loaded_templates; // All of the loaded templates

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
                SDL_Renderer* r = RendererData::GetRenderer();
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
 * @param   audio_name  the name of the audio clip to get from the database
 * @returns             the audio clip with the specified name
*/
Mix_Chunk* GetSound(const std::string& audio_name)
{
    if (loaded_sounds.find(audio_name) == loaded_sounds.end())
    {
        std::cout << "error: missing audio clip " << audio_name;
        exit(0);
    }
    
    return loaded_sounds[audio_name];
} // GetSound()

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
static void AudioPlay(const int channel, const std::string& clip_name, const int num_loops)
{
    Mix_Chunk* mix_chunk = GetSound(clip_name);
    if (Mix_PlayChannel(channel, mix_chunk, num_loops) == -1) {
        std::cout << Mix_GetError();
    }
} // AudioPlay()

/*
* Halt playing of a particular channel, except for any playing music.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_HaltChannel
*
* @param	channel    channel to halt, or -1 to halt all channels
*/
static void AudioHalt(const int channel)
{
    if (Mix_HaltChannel(channel) == -1) {
        std::cout << Mix_GetError();
    }
} // AudioHalt()

/*
* Pause a particular channel. Any music is unaffected.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Pause
*
* @param	channel    the channel to pause, or -1 to pause all channels.
*/
static void AudioPause(const int channel)
{
    Mix_Pause(channel);
} // AudioPause()

/*
* Resume a particular channel. Any music is unaffected.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Resume
*
* @param	channel    the channel to resume, or -1 to resume all paused channels.
*/
static void AudioResume(const int channel)
{
    Mix_Resume(channel);
} // AudioResume()

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
static int AudioSetVolume(const int channel, const int volume)
{
    int previous_volume = Mix_Volume(channel, volume);
    return previous_volume;
} // AudioSetVolume()

/*
* Close the mixer, halting all playing audio.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_CloseAudio
*/
static void AudioCloseMixer()
{
    Mix_CloseAudio();
} // AudioCLoseMixer()

/*
* Deinitialize SDL_mixer.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_Quit
*/
static void AudioDeinitMixer()
{
    Mix_Quit();
} // AudioDeinitMixer

/*
* Set the master volume for all channels.
* More Info: https://wiki.libsdl.org/SDL2_mixer/Mix_MasterVolume
*
* @param	volume    the new volume, between 0 and MIX_MAX_VOLUME (128), or -1 to query.
*
* @returns	Returns the previous volume. If the specified volume is -1,
            this returns the current volume.
*/
static void AudioSetMasterVolume(const int volume)
{
    Mix_MasterVolume(volume);
} // AudioSetMasterVolume()

//-------------------------------------------------------
// Text Database

/**
 * Loads all of the fonts in the resources/fonts directory
*/
void LoadFonts()
{
    /* Load files from this path */
    const std::string path = "resources/fonts";
    
    // Fills up the database if the path exists
    if (std::filesystem::exists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(path))
        {
            if (file.path() != path + "/.DS_Store")
            {
                loaded_fonts[file.path().filename().stem().stem().string()][16] = TTF_OpenFont(file.path().string().c_str(), 16);
            }
        }
    }
} // LoadFonts()

/**
 * Get an font based on its size and name
 *
 * @param   font_name   the name of the font to get from the database
 * @param   font_size       the size of the font to get from the database
 * @returns             the font with the specified name and size
*/
TTF_Font* GetFont(std::string font_name, int font_size)
{
    // Throw error if the font does not exist
    if (loaded_fonts.find(font_name) == loaded_fonts.end())
    {
        std::cout << "error: font " << font_name << " missing";
        exit(0);
    }
    
    // Loads font at font_size if it does not already exist
    if (loaded_fonts[font_name].find(font_size) == loaded_fonts[font_name].end())
    {
        /* Load font file from resources/fonts */
        const std::string path = "resources/fonts/" + font_name + ".ttf";
        // Open the font at the desired size, if not done so already
        loaded_fonts[font_name][font_size] = TTF_OpenFont(path.c_str(), font_size);
    }
    
    return loaded_fonts[font_name][font_size];
} // GetFont()

//-------------------------------------------------------
// Template Database

/**
 * Loads all of the templates in the resources/actor_templates directory
*/
void LoadTemplates()
{
    /* Load files from this path */
    const std::string path = "resources/actor_templates";
    
    // Fills up the database if the path exists
    if (std::filesystem::exists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(path))
        {
            if (file.path() != path + "/.DS_Store")
            {
                rapidjson::Document* template_document = new rapidjson::Document;
                EngineUtils::ReadJsonFile(file.path().string(), *template_document);
                
                loaded_templates[file.path().filename().stem().stem().string()] = template_document;
            }
        }
    }
} // LoadTemplates()

/**
 * Get a template based on the templates name
 *
 *`@param   template_name   the name of the template to get from the database
 * @returns                 the template with the specified name
*/
rapidjson::Document* GetTemplate(std::string template_name)
{
    if (loaded_templates.find(template_name) == loaded_templates.end())
    {
        std::cout << "error: missing template " << template_name;
        exit(0);
    }
    
    return loaded_templates[template_name];
} // GetTemplate()
