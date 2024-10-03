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

#ifndef NDEBUG
#include "EditorManager.h"
#endif

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
    Input::Init();
    LoadComponentTypes();
    LoadScenePaths();
    
    // Load Resources needed for scene initialization
    // Do this here because the initial scene is loaded in CheckConfigFiles.
    LoadTemplates();
    
    if( CheckConfigFiles() != 0 ) {
        // error with config files
    }
    
    RendererData::Init(EngineData::game_title);
    
#ifndef NDEBUG
    // Initializes imgui and the editor
    EditorManager::Init();
#endif
    
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
    if(!FileUtils::DirectoryExists("resources/")) {
        std::cout << "error: resources/ missing";
        return 1;
    }
    if( CheckGameConfig() && RendererData::LoadRenderingConfig() )
        return 0;
    return 1;
}

bool CheckGameConfig()
{
    if( !FileUtils::DirectoryExists("resources/game.config") ) {
        std::cout << "error: resources/game.config missing";
        return false;
    }
    rapidjson::Document game_config;
    EngineUtils::ReadJsonFile(FileUtils::GetPath("resources/game.config"), game_config);
    
    if(game_config.HasMember("game_title")){
        EngineData::game_title = game_config["game_title"].GetString();
    }
    if(game_config.HasMember("initial_scene")){
        std::string initial_scene = game_config["initial_scene"].GetString();
        Scene::initial_scene_name = initial_scene;
        Scene::new_scene_name = initial_scene;
        Scene::LoadNewScene();
    }
    else {
        std::cout << "error: initial scene unspecified";
        exit(0);
    }
    return true;
}

//-------------------------------------------------------

/**
 * Responsible for running the engine correctly on each frame.
 * Every call of GameLoop is considered to be 1 frame.
 * Called once every frame by "Game".
*/
int GameLoop()
{
#ifndef NDEBUG
    // Esures that modes are only switched at the start of any given frame
    if (EditorManager::trigger_editor_mode_toggle)
    {
        EditorManager::ToggleEditorMode();
        EditorManager::trigger_editor_mode_toggle = false;
    }
#endif
    
    // Used to pause certain engine functions if editor mode is active
    bool editor_mode = false;
#ifndef NDEBUG
    editor_mode = EditorManager::GetEditorMode();
#endif
    
    if (!editor_mode)
    {
        LuaAPI::IncrementFrameCounter();
    }

    if (EngineData::quit)
    {
#ifndef NDEBUG
        // Cleans up the imgui context
        EditorManager::Cleanup();
#endif
        // Cleans up the SDL widnow and renderer
        RendererData::Cleanup();
        return 1;
    }
    
    // Process all SDL events
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
#ifndef NDEBUG
        // Pass events to imgui for editor
        EditorManager::ImGuiProcessSDLEvent(&event);
#endif
        
        Input::ProcessEvent(event);
        if (event.type == SDL_QUIT)
        {
            EngineData::quit = true;
        }
    }
    
    SDL_RenderClear(RendererData::GetRenderer()); // clear the renderer with the render clear color
    
    if (!editor_mode)
    {
        Scene::UpdateActors();
        
        // Box2D Physics
        if (PhysicsWorld::world_initialized) {
            InitializeCollisions();
            PhysicsWorld::AdvanceWorld();
            PhysicsWorld::world->DebugDraw();
        }
    }
#ifndef NDEBUG
    else
    {
        EditorManager::EditorUpdate();
    }
#endif

    // RENDER STUFF HERE
    RendererData::RenderAndClearAllImageRequests();
    RendererData::RenderAndClearAllTextRequests();
    RendererData::RenderAndClearAllPixels();
    RendererData::RenderAndClearAllLines();
    RendererData::RenderAndClearAllUI();
    
#ifndef NDEBUG
    EditorManager::RenderEditor();
#endif
    
    SDL_RenderPresent(RendererData::GetRenderer()); // present the frame into the window
    
    Input::LateUpdate();
    
    // Load the new scene if asked for
    if (Scene::load_new_scene) {Scene::LoadNewScene();}
    
    return 0;
} // GameLoop()
