#include "sol/sol.hpp"
#include "SDL.h"
#include "SDL_image.h"

int main(int argc, char* argv[]) {
	SDL_Window* window = SDL_CreateWindow("test", 100, 100, 512, 512, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // set renderer draw color
	SDL_RenderClear(renderer); // clear the renderer with the render clear color

	return 0;
}