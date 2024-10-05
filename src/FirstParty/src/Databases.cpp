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
#include "SceneDB.h"
#include "LuaAPI.h"

#include "Engine.h"

// Resources
std::unordered_map<std::string, SDL_Texture*> loaded_images; // All of the loaded images
std::unordered_map<std::string, Mix_Chunk*> loaded_sounds; // All of the loaded sounds
std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> loaded_fonts; // All of the loaded fonts
std::unordered_map<std::string, rapidjson::Document*> loaded_templates; // All of the loaded templates
std::unordered_map<std::string, std::shared_ptr<sol::table>> loaded_component_types; // All of the loaded component types
std::unordered_map<std::string, std::string> loaded_scene_paths; // All of the loaded scene paths

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
    if (FileUtils::DirectoryExists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(FileUtils::GetPath(path)))
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
 * @param   image_name    the name of the image to get from the database
 * @returns               the image with the specified name
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
    if (FileUtils::DirectoryExists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(FileUtils::GetPath(path)))
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
 * @param   audio_name    the name of the audio clip to get from the database
 * @returns               the audio clip with the specified name
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
    if (FileUtils::DirectoryExists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(FileUtils::GetPath(path)))
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
 * @param   font_size   the size of the font to get from the database
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
    if (FileUtils::DirectoryExists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(FileUtils::GetPath(path)))
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
 * @param   template_name   the name of the template to get from the database
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

//-------------------------------------------------------
// Component Type Database

/**
 * Loads all of the components in the resources/component_types directory
 *  NOTE: All scripts table names must be the EXACT SAME as the file name (minus the .lua of course)
*/
void LoadComponentTypes()
{
    /* Load files from this path */
    const std::string path = "resources/component_types";
    
    // Fills up the database if the path exists
    if (FileUtils::DirectoryExists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(FileUtils::GetPath(path)))
        {
            
            std::string type_name = file.path().stem().string();
            
            if (type_name != ".DS_Store")
            {
                sol::load_result script = LuaAPI::GetLuaState()->load_file(file.path().string().c_str());
                if (script.valid())
                {
                    // Load the script into the lua state
                    script();
                    
                    // Attaches the script to a lua table for easier member access
                    sol::table component_table = (*LuaAPI::GetLuaState())[type_name.c_str()];
                    
                    // Adds the type to this component so we don't have to re-add it later
                    component_table["type"] = type_name;
                    
                    // Load the component type into our database
                    loaded_component_types.insert(
                          {
                              type_name,
                              std::make_shared<sol::table>(component_table)
                          }
                    );
                }
                else
                {
                    std::cout << "problem with lua file " << type_name;
                    exit(0);
                }
            }
        }
    }
}

/**
 * Get a component type based on its name
 *
 * @param   component_name    the name of the component type to get from the database
 * @returns                   the component type with the specified name
*/
std::shared_ptr<sol::table> GetComponentType(std::string component_name)
{
    if (loaded_component_types.find(component_name) == loaded_component_types.end())
    {
        std::cout << "error: missing component " << component_name;
        exit(0);
    }
    
    return loaded_component_types[component_name];
}

//-------------------------------------------------------
// Scene Path Database

/**
 * Loads all of the scene paths in the resources/scenes directory
*/
void LoadScenePaths()
{
    /* Load files from this path */
    const std::string path = "resources/scenes";
    
    // Fills up the database if the path exists
    if (FileUtils::DirectoryExists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(FileUtils::GetPath(path)))
        {
            if (file.path() != path + "/.DS_Store")
            {
                std::string scene_name = file.path().stem().string();
                loaded_scene_paths[scene_name] = file.path().string();
            }
        }
    }
}

/**
 * Get a scene path based on the name of the scene
 *
 * @param   scene_name      the name of the scene path to get from the database
 * @returns                 the scene path with the specified name
*/
std::string GetScenePath(std::string scene_name)
{
    if (loaded_scene_paths.find(scene_name) == loaded_scene_paths.end())
    {
        std::cout << "error: missing scene " << scene_name;
        exit(0);
    }
    
    return loaded_scene_paths[scene_name];
}
