#include "GUIRenderer.h"

/**
* Initializes the renderer
*
* @param	title	the title of the window to create
*/
void GUIRenderer::Init(const std::string& title)
{
    SDL_Window* window = SDL_CreateWindow(title.c_str(), window_position.x, window_position.y, window_size.x, window_size.y, SDL_WINDOW_SHOWN);
    GUIRenderer::SetWindow(window);
    SDL_Renderer* renderer = SDL_CreateRenderer(GUIRenderer::GetWindow(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    GUIRenderer::SetRenderer(renderer);

    SDL_SetRenderDrawColor(GUIRenderer::GetRenderer(), clear_color_r, clear_color_g, clear_color_b, clear_color_a); // set renderer draw color
}

/**
 * Cleans up the SDL renderer and window
 */
void GUIRenderer::Cleanup()
{
    SDL_DestroyRenderer(renderer_gui);
    SDL_DestroyWindow(window_gui);
}
