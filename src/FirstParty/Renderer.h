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
#include "SDL_ttf.h"
#include "glm/glm.hpp"

#include "RenderRequests.h"
#include "ImageDB.h"
#include "TextDB.h"

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

	/*
	Turns a string into an SDL_Texture using the specified font perameters

	@param		renderer	SDL_Renderer* to the renderer
	@param		text		the string of text that is going to be turned into a SDL_Texture
	@param		font_color	A SDL_Color for the color of the text
	@param		font_name	The name of the font to be used
	@param		font_size	The size of the font
	@returns	A SDL_Texture* to a SDL_Texture
	*/
	static SDL_Texture* ConvertTextToTexture(SDL_Renderer* renderer, const std::string& text, const SDL_Color& font_color, const std::string font_name, const int font_size);

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

	/*
	Renders all text draw requests in the text_draw_request_queue
	*/
	static void RenderAndClearAllTextRequests();

	/*
	Renders all UI draw requests in the ui_draw_request_queue
	*/
	static void RenderAndClearAllUI();

	/*
	Renders all of the pixel draw requests in the pixel_draw_request_queue
	*/
	static void RenderAndClearAllPixels();

	/*
	Creates a UI draw request at the specified screen position using the image with name 'image_name'

	@param	image_name	The name of the image to be draw
	@param	x			The x position to draw the image at
	@param	y			The y position to draw the image at
	*/
	static void DrawUI(const std::string& image_name, const float x, const float y);

	/*
	Creates a UI draw request at the specified screen position, with the color {r, g, b, a},
	and in the given sorting layer

	@param	image_name		The name of the image to be draw
	@param	x				The x position to draw the image at
	@param	y				The y position to draw the image at
	@param	r				[0, 255] How red the image is
	@param	g				[0, 255] How green the image is
	@param	b				[0, 255] How blue the image is
	@param	a				[0, 255] The alpha value of the image
	@param	sorting_order	The sorting layer that the image should be drawn in
	*/
	static void DrawUIEx(const std::string& image_name, const float x, const float y, const float r, const float g,
		const float b, const float a, const float sorting_order);

	/*
	Creates an image draw request at the specified screen position using the image with name 'image_name'

	@param	image_name	The name of the image to be drawn
	@param	x			The x position to draw the image at
	@param	y			The y position to draw the iamge at
	*/
	static void DrawImage(const std::string& image_name, const float x, const float y);

	/*
	Creates an image draw request for the image with name 'image_name' with more control than DrawImage

	@param	image_name			The name of the image to be drawn
	@param	x					The x position to draw the image at
	@param	y					The y position to draw the iamge at
	@param	rotation_degrees	The rotation of the image in degrees
	@param	scale_x				The scale to draw the x-axis. 1 is normal
	@param	scale_y				The scale to draw the y-axis. 1 is normal
	@param	pivot_x				[0, 1] Where on the x position of the image should be located. 0 is the left side of the image and 1 is the right.
	@param	pivot_y				[0, 1] Where on the y position of the image should be located. 0 is the top side of the image and 1 is the bottom.
	@param	r					[0, 255] How red the image is
	@param	g					[0, 255] How green the image is
	@param	b					[0, 255] How blue the image is
	@param	a					[0, 255] The alpha value of the image
	@param	sorting_order		The sorting layer that the image should be drawn in			
	*/
	static void DrawImageEx(const std::string& image_name, const float x, const float y, const float rotation_degrees,
		const float scale_x, const float scale_y, const float pivot_x, const float pivot_y, const float r, const float g,
		const float b, const float a, const float sorting_order);

	/*
	Creates a pixel draw request at the specified (x, y) screen position with color {r, g, b, a}

	@param	x	the x of the screen position for the pixel to be drawn
	@param	y	the x of the screen position for the pixel to be drawn
	@param	r	the red value of the color of the pixel to be drawn [0, 255]
	@param	g	the green value of the color of the pixel to be drawn [0, 255]
	@param	b	the blue value of the color of the pixel to be drawn [0, 255]
	@param	a	the alpha value of the color of the pixel to be drawn [0, 255]
	*/
	static void DrawPixel(const float x, const float y, const float r, const float g, const float b, const float a);

	/*
	Creates a text draw request for str_content at the specified (x, y) screen position
	
	@param	str_content		the text to be drawn
	@param	x				the x of the screen position for top left corner of the text
	@param	y				the y of the screen position for top left corner of the text
	@param	font_name		the name of the font to draw the text with
	@param	font_size		how large the text should be drawn
	@param	r	the red value of the color of the text to be drawn [0, 255]
	@param	g	the green value of the color of the text to be drawn [0, 255]
	@param	b	the blue value of the color of the text to be drawn [0, 255]
	@param	a	the alpha value of the color of the text to be drawn [0, 255]				
	*/
	static void DrawText(const std::string str_content, int x, int y, std::string font_name,
		int font_size, int r, int g, int b, int a);

	/*
	Set the (x, y) position of the camera

	@param	x	the new x screen position of the camera
	@param	y	the new y screen position of the camera
	*/
	static void SetCameraPosition(const float x, const float y);

	/*
	Get the (x, y) position of the camera as a glm::vec2

	@returns	a glm::vec2 containing the camera's current position
	*/
	static glm::vec2 GetCameraPosition();

	/*
	Set the zoom factor of the camera

	@param	new_zoom_factor		the new zoom factor for the camera
	*/
	static void SetCameraZoom(const float new_zoom_factor);

	/*
	Gets the current zoom factor of the camera

	@returns	the current zoom factor of the camera
	*/
	static float GetCameraZoom();
};

struct CompareImageRequests {
	bool operator() (const ImageDrawRequest& request1, const ImageDrawRequest& request2) {
		return request1.sorting_order < request2.sorting_order;
	}
};

struct CompareUIRequests {
	bool operator() (const UIRenderRequest& request1, const UIRenderRequest& request2) {
		return request1.sorting_order < request2.sorting_order;
	}
};

#endif /* Renderer.h */