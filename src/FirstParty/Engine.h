//
//  Engine.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/20/24.
//  The highest level script in this engine.

#ifndef Engine_h
#define Engine_h

#include <stdio.h>
#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

using namespace std;

struct EngineData
{
    static bool quit;   // True if the game should be quit out of.
    
    // TODO: Move these variables to the renderer scripts
    static SDL_Window* window;  // The window that the game is displayed on
    static SDL_Renderer* renderer;
    
}; // EngineData

/**
 * Called from the main function. Starts the game.
*/
void Game();

/**
 * Called before the first game loop.
 * Prepares the engine for the first loop by initializing other parts of the engine.
*/
void Initialize();

/**
 * Responsible for running the engine correctly on each frame.
 * Every call of GameLoop is considered to be 1 frame.
 * Called once every frame by "Game".
*/
int GameLoop();

#endif /* Engine_h */
