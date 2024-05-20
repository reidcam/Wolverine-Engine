#include "sol/sol.hpp"
#include "SDL.h"
#include "SDL_image.h"

int main(int argc, char* argv[]) {
    IMG_Init(IMG_INIT_PNG);
    SDL_Window* window = SDL_CreateWindow("test", 0, 0, 512, 512, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC + SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
        std::cerr << "Failed to create renderer : " << SDL_GetError() << std::endl;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // set renderer draw color
    while (true)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)) { } // Flush event queue
        SDL_RenderClear(renderer); // clear the renderer with the render clear color
        SDL_RenderPresent(renderer);
    }
	return 0;
}
