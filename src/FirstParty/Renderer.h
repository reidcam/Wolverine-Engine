//
//	Renderer.h
//	wolverine_engine
//
//	Created by Cameron Reid on 5/21/24.
//	Handles screen rendering

#ifndef Renderer_h
#define Renderer_h

#include <deque>

#include "SDL.h"
#include "SDL_image.h"
#include "glm/glm.hpp"

#include "RenderRequests.h"
#include "ImageDB.h"

class RendererData
{
private:
	inline static SDL_Window* window = nullptr;  // The window that the game is displayed on
	inline static SDL_Renderer* renderer = nullptr;

	// initial window size and position
	inline static glm::vec2 window_position = glm::vec2(0, 0);
	inline static glm::vec2 window_size = glm::vec2(640, 360);
	inline static float zoom_factor = 1;

	inline static glm::vec2 current_cam_pos = glm::vec2(0.0f, 0.0f);

	// clear color values for the renderer
	inline static Uint8 clear_color_r = 255;
	inline static Uint8 clear_color_g = 255;
	inline static Uint8 clear_color_b = 255;
	inline static Uint8 clear_color_a = 255;
	
	// queues for different kinds of render requests
	inline static std::deque<ImageDrawRequest> image_draw_request_queue;
	inline static std::deque<TextRenderRequest> text_draw_request_queue;
	inline static std::deque<UIRenderRequest> ui_draw_request_queue;
	inline static std::deque<PixelDrawRequest> pixel_draw_request_queue;

	/*
	Sets the SDL_Renderer for the Renderer

	@param	new_renderer	A SDL_Renderer* to the new renderer
	*/
	inline static void SetRenderer(SDL_Renderer* new_renderer) { RendererData::renderer = new_renderer; } // SetRenderer()

	/*
	Sets the SDL_Window* for the Renderer
	
	@param	new_window	A SDL_Window* to the new window
	*/
	inline static void SetWindow(SDL_Window* new_window) { RendererData::window = new_window; } // SetWindow()

public:
	/*
	Returns a pointer to the SDL_Window

	@returns	a SDL_Window* to the SDL_Window
	*/
	inline static SDL_Window* GetWindow() { return RendererData::window; } // GetWindow()

	/*
	Returns a pointer to the SDL_Renderer
	
	@returns	a SDL_Renderer* to the SDL_Renderer
	*/
	inline static SDL_Renderer* GetRenderer() { return RendererData::renderer; } // GetRenderer()

	/*
	Initializes the renderer
	*/
	static void Init();

	/*
	Loads in the renderer settings from the rendering config
	*/
	static void LoadCameraSettings();

	/*
	Puts the window in the specified fullscreen mode

	@param	flag	can be SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP, or 0
	*/
	static void SetWindowFullscreen(int flag);

	/*
	Renders all image draw requests in the image_draw_request_queue
	*/
	static void RenderAndClearAllImageRequests();
};

struct CompareImageRequests {
	bool operator() (const ImageDrawRequest& request1, const ImageDrawRequest& request2) {
		return request1.sorting_order < request2.sorting_order;
	}
};

#endif /* Renderer.h */