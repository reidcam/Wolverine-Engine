//
//  Engine.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/20/24.
//  The highest level script in this engine.
//

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
    static std::string game_title;
    
    // TODO: Move these variables to the renderer scripts
    static SDL_Window* window;  // The window that the game is displayed on
    static SDL_Renderer* renderer;
    static inline int cam_x_resolution = 1280;
    static inline int cam_y_resolution = 720;
    static inline float zoom_factor = 1.0f;
    static inline uint8_t clear_color[3] = {255, 255, 255};
    
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
 * Checks validity of game.config and rendering.config, and records their contents
 * parameters specified in config files are currently stored in EngineData
 */
int CheckConfigFiles();
/**
 * resources/game.config accepts fields:
 * `game_title` and `initial_scene`
 */
bool CheckGameConfig();
/**
 * resources/rendering.config accepts fields:
 * `x_resolution`, `y_resolution`, `zoom_factor`, `clear_color_r`, `clear_color_g`, `clear_color_b`
 */
bool CheckRenderingConfig();

/**
 * Responsible for running the engine correctly on each frame.
 * Every call of GameLoop is considered to be 1 frame.
 * Called once every frame by "Game".
*/
int GameLoop();

#endif /* Engine_h */
