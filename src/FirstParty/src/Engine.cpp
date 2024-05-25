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
#include "SceneManager.h"

#include "Engine.h"
#include "EngineUtils.h"

bool EngineData::quit = false;   // True if the game should be quit out of.
SDL_Window* EngineData::window;  // The window that the game is displayed on
SDL_Renderer* EngineData::renderer;

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
    
    EngineData::window = SDL_CreateWindow("test", 0, 0, 512, 512, 0);
    EngineData::renderer = SDL_CreateRenderer(EngineData::window, -1, SDL_RENDERER_PRESENTVSYNC + SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(EngineData::renderer, 255, 255, 255, 255); // set renderer draw color
    if( CheckConfigFiles() != 0 ) {
        // error with config files
    }
    
    // Load Assets
    LoadImages();
    LoadSounds();
    LoadFonts();
    LoadTemplates();
    
    Scene::LoadScene("test");
} // Initialize()

//-------------------------------------------------------

/**
 * Checks validity of game.config and rendering.config, and records their contents
 */
int CheckConfigFiles()
{
    if( !EngineUtils::DirectoryExists("resources") ) {
        std::cout << "error: resources/ missing";
        return 1;
    }
    if( CheckGameConfig() && CheckRenderingConfig() )
        return 0;
    return 1;
}

bool CheckGameConfig()
{
    if( !EngineUtils::DirectoryExists("resources/game.config") ) {
        std::cout << "error: resources/game.config missing";
        return false;
    }
    return true;
}

bool CheckRenderingConfig()
{
    if( !EngineUtils::DirectoryExists("resources/rendering.config") ) {
        std::cout << "error: resources/rendering.config missing";
        return false;
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
    
    SDL_RenderClear(EngineData::renderer); // clear the renderer with the render clear color
    
    Scene::UpdateActors();
    
    // RENDER STUFF HERE
    
    SDL_RenderPresent(EngineData::renderer); // present the frame into the window
    
    return 0;
} // GameLoop()
