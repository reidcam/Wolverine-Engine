//
//	GUIRenderer.h
//	wolverine_engine
//
//	Created by Cameron Reid on 9/30/24.
//	Handles screen rendering for the ImGUI editor window

#ifndef GUIRenderer_h
#define GUIRenderer_h

#include "Renderer.h"

class GUIRenderer
{
private:
	inline static SDL_Window* window_gui = nullptr;  // The window that the editor is displayed on
	inline static SDL_Renderer* renderer_gui = nullptr;

	// initial window size and position
	inline static glm::vec2 window_position = glm::vec2(960, 30);
	inline static glm::vec2 window_size = glm::vec2(960, 540);
	inline static float zoom_factor = 1;

	inline static glm::vec2 current_cam_pos = glm::vec2(0.0f, 0.0f);

	// clear color values for the renderer
	inline static Uint8 clear_color_r = 255;
	inline static Uint8 clear_color_g = 255;
	inline static Uint8 clear_color_b = 255;
	inline static Uint8 clear_color_a = 255;

	/**
	* Sets the SDL_Renderer for the Renderer
	*
	* @param	new_renderer	A SDL_Renderer* to the new renderer
	*/
	inline static void SetRenderer(SDL_Renderer* new_renderer) { GUIRenderer::renderer_gui = new_renderer; } // SetRenderer()

	/**
	* Sets the SDL_Window* for the Renderer
	*
	* @param	new_window    A SDL_Window* to the new window
	*/
	inline static void SetWindow(SDL_Window* new_window) { GUIRenderer::window_gui = new_window; } // SetWindow()
public:
	/**
	* Initializes the renderer
	*
	* @param	title	the title of the window to create
	*/
	static void Init(const std::string& title);

	/**
	 * Cleans up the SDL renderer and window
	 */
	static void Cleanup();

	/**
	* Returns a pointer to the SDL_Window
	*
	* @returns	  a SDL_Window* to the SDL_Window
	*/
	inline static SDL_Window* GetWindow() { return GUIRenderer::window_gui; } // GetWindow()

	/**
	* Returns a pointer to the SDL_Renderer
	*
	* @returns	  a SDL_Renderer* to the SDL_Renderer
	*/
	inline static SDL_Renderer* GetRenderer() { return GUIRenderer::renderer_gui; } // GetRenderer()
};

#endif /* GUIRenderer.h */