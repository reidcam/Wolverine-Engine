//
//  Engine.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/20/24.
//  The highest level script in this engine.
//

#include <stdio.h>

#include "InputManager.h"

#include "ImageDB.h"
#include "AudioDB.h"
#include "TextDB.h"
#include "TemplateDB.h"
#include "SceneDB.h"
#include "SceneManager.h"
#include "ComponentManager.h"

#include "Engine.h"
#include "EngineUtils.h"

bool EngineData::quit = false;   // True if the game should be quit out of.

//-------------------------------------------------------

/**
 * Called from the main function. Starts the game.
*/
void Game()
{
    Initialize();
    
    // 0 means keep looping 1 means stop the loop.
    int loop_status = 0;
    
    while (loop_status != 1)
    {
        loop_status = GameLoop();
    }
    
    return;
} // Game()

//-------------------------------------------------------

/**
 * Called before the first game loop.
 * Prepares the engine for the first loop by initializing other parts of the engine.
*/
void Initialize()
{
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    
    LuaAPI::InitLuaState();
    LuaAPI::ExposeLuaAPI();
    LoadComponentTypes();
    LoadScenePaths();
    
    // Load Resources needed for scene initialization
    // Do this here because the initial scene is loaded in CheckConfigFiles.
    LoadTemplates();
    
    if( CheckConfigFiles() != 0 ) {
        // error with config files
    }
    
    RendererData::Init(EngineData::game_title);
    
    // Load Assets
    // Do this here because the renderer needs to be initialized before these assets can be loaded.
    LoadImages();
    LoadSounds();
    LoadFonts();
} // Initialize()

//-------------------------------------------------------

/**
 * Checks validity of game.config and rendering.config, and records their contents
 */
int CheckConfigFiles()
{
    std::filesystem::path currentFile = __FILE__;
    std::filesystem::path currentDirectory = currentFile.parent_path();
    std::filesystem::path relativePath = currentDirectory / ".." / ".." / ".." / "resources";
    std::filesystem::path absolutePath = std::filesystem::canonical(relativePath);

    if( !EngineUtils::DirectoryExists(absolutePath.string())) {
        std::cout << "error: resources/ missing";
        return 1;
    }
    if( CheckGameConfig() && RendererData::LoadRenderingConfig() )
        return 0;
    return 1;
}

bool CheckGameConfig()
{
    if( !EngineUtils::DirectoryExists("resources/game.config") ) {
        std::cout << "error: resources/game.config missing";
        return false;
    }
    rapidjson::Document game_config;
    EngineUtils::ReadJsonFile("resources/game.config", game_config);
    
    if(game_config.HasMember("game_title")){
        EngineData::game_title = game_config["game_title"].GetString();
    }
    if(game_config.HasMember("initial_scene")){
        std::string initial_scene = game_config["initial_scene"].GetString();
        Scene::new_scene_name = initial_scene;
        Scene::LoadNewScene();
    }
    else {
        std::cout << "error: initial scene unspecified";
        exit(0);
    }
    return true;
}

//bool CheckRenderingConfig()
//{
//    if( !EngineUtils::DirectoryExists("resources/rendering.config") ) {
//        std::cout << "error: resources/rendering.config missing";
//        return false;
//    }
//    rapidjson::Document rendering_config;
//    EngineUtils::ReadJsonFile("resources/rendering.config", rendering_config);
//
//    if(rendering_config.HasMember("x_resolution")){
//        EngineData::cam_x_resolution = rendering_config["x_resolution"].GetInt();
//    }
//    if(rendering_config.HasMember("y_resolution")){
//        EngineData::cam_y_resolution = rendering_config["y_resolution"].GetInt();
//    }
//    // read clear color
//    if(rendering_config.HasMember("clear_color_r")){
//        EngineData::clear_color[0] = rendering_config["clear_color_r"].GetInt();
//    }
//    if(rendering_config.HasMember("clear_color_g")){
//        EngineData::clear_color[1] = rendering_config["clear_color_g"].GetInt();
//    }
//    if(rendering_config.HasMember("clear_color_b")){
//        EngineData::clear_color[2] = rendering_config["clear_color_b"].GetInt();
//    }
//    if(rendering_config.HasMember("zoom_factor")){
//        EngineData::zoom_factor = rendering_config["zoom_factor"].GetDouble();
//    }
//    return true;
//}

//-------------------------------------------------------

/**
 * Responsible for running the engine correctly on each frame.
 * Every call of GameLoop is considered to be 1 frame.
 * Called once every frame by "Game".
*/
int GameLoop()
{
    LuaAPI::IncrementFrameCounter();

    if (EngineData::quit)
    {
        return 1;
    }
    
    // Process all SDL events
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            EngineData::quit = true;
        }
    }
    
    SDL_RenderClear(RendererData::GetRenderer()); // clear the renderer with the render clear color
    
    Scene::UpdateActors();
    
    // Load the new scene if asked for
    if (Scene::load_new_scene) {Scene::LoadNewScene();}
    
    // RENDER STUFF HERE
    
    SDL_RenderPresent(RendererData::GetRenderer()); // present the frame into the window
    
    return 0;
} // GameLoop()
