#include "Renderer.h"
#include "EngineUtils.h"

/**
* Turns a string into an SDL_Texture using the specified font perameters
*
* @param		renderer	SDL_Renderer* to the renderer
* @param		text		the string of text that is going to be turned into a SDL_Texture
* @param		font_color	A SDL_Color for the color of the text
* @param		font_name	The name of the font to be used
* @param		font_size	The size of the font
* @returns	    A SDL_Texture* to a SDL_Texture
*/
SDL_Texture* RendererData::ConvertTextToTexture(SDL_Renderer* renderer, const std::string& text, const SDL_Color& font_color, const std::string font_name, const int font_size)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(GetFont(font_name, font_size), text.c_str(), font_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);

	return text_texture;
}

/**
* Initializes the renderer
*
* @param	title	the title of the window to create
*/
void RendererData::Init(const std::string& title)
{
    SDL_Window* window = SDL_CreateWindow(title.c_str(), window_position.x, window_position.y, window_size.x, window_size.y, SDL_WINDOW_SHOWN);
    SetWindow(window);
    SDL_Renderer* renderer = SDL_CreateRenderer(RendererData::window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SetRenderer(renderer);

    SDL_SetRenderDrawColor(RendererData::GetRenderer(), clear_color_r, clear_color_g, clear_color_b, clear_color_a); // set renderer draw color
}

/**
Loads in the renderer settings from the rendering config
*/
bool RendererData::LoadRenderingConfig()
{
	std::string file_path = "resources/rendering.config";

	if (!FileUtils::DirectoryExists(file_path)) {
		std::cout << "error: resources/rendering.config missing";
		return false;
	}

	// read in the document
	rapidjson::Document doc;
	EngineUtils::ReadJsonFile(FileUtils::GetPath(file_path), doc);

	// parse the JSON
	if (doc.HasMember("x_position")) {
		window_position.x = doc["x_position"].GetInt();
	}
	if (doc.HasMember("y_position")) {
		window_position.y = doc["y_position"].GetInt();
	}
	if (doc.HasMember("x_resolution")) {
		window_size.x = doc["x_resolution"].GetInt();
	}
	if (doc.HasMember("y_resolution")) {
		window_size.y = doc["y_resolution"].GetInt();
	}
	if (doc.HasMember("clear_color_r")) {
		clear_color_r = doc["clear_color_r"].GetInt();
	}
	if (doc.HasMember("clear_color_g")) {
		clear_color_g = doc["clear_color_g"].GetInt();
	}
	if (doc.HasMember("clear_color_b")) {
		clear_color_b = doc["clear_color_b"].GetInt();
	}
	if (doc.HasMember("zoom_factor")) {
		zoom_factor = doc["zoom_factor"].GetFloat();
	}
    if (doc.HasMember("draw_debug")) {
        draw_debug = doc["draw_debug"].GetBool();
    }

	return true;
}

/**
* Puts the window in the specified fullscreen mode.
* More info: https://wiki.libsdl.org/SDL2/SDL_SetWindowFullscreen
*
* SDL_WINDOW_FULLSCREEN:			creates a zoomed in fullscreen that creates the largest possible square screen
* SDL_WINDOW_FULLSCREEN_DESKTOP:	makes the window the size of the current screen
* 0:								returns the window to its non-full-screen size
*
* @param	flag					can be SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP, or 0
*/
void RendererData::SetWindowFullscreen(int flag)
{
	//TODO: add SDL error checking
	SDL_SetWindowFullscreen(GetWindow(), flag);
}

/**
* Resizes the window when the window is not in full screen mode
* More info: https://wiki.libsdl.org/SDL2/SDL_SetWindowSize
*
* @param	w	the new width of the window
* @param	h	the new height of the window
*/
void RendererData::SetNonFullScreenWindowSize(int w, int h)
{
	SDL_SetWindowSize(GetWindow(), w, h);
}

/**
* Resizes the window when the window is in full screen mode.
* More info: https://wiki.libsdl.org/SDL2/SDL_SetWindowDisplayMode
*		     https://wiki.libsdl.org/SDL2/SDL_DisplayMode
*
* @param	display_mode	a SDL_DisplayMode* representing the mode to use
*/
void RendererData::SetFullScreenWindowSize(const SDL_DisplayMode* display_mode)
{
	SDL_SetWindowDisplayMode(GetWindow(), display_mode);
}

/**
Renders all image draw requests in the image_draw_request_queue
*/
void RendererData::RenderAndClearAllImageRequests()
{
	std::stable_sort(image_draw_request_queue.begin(), image_draw_request_queue.end(), CompareImageRequests());
	SDL_RenderSetScale(renderer, zoom_factor, zoom_factor);

	for (auto& request : image_draw_request_queue) {
		glm::vec2 final_rendering_position = glm::vec2(request.x, request.y) - current_cam_pos;

		SDL_Texture* tex = GetImage(request.image_name);
		SDL_Rect tex_rect;
		SDL_QueryTexture(tex, NULL, NULL, &tex_rect.w, &tex_rect.h);

		// Apply scale
		int flip_mode = SDL_FLIP_NONE;
		if (request.scale_x < 0)
			flip_mode |= SDL_FLIP_HORIZONTAL;
		if (request.scale_y < 0)
			flip_mode |= SDL_FLIP_VERTICAL;

		float x_scale = std::abs(request.scale_x);
		float y_scale = std::abs(request.scale_y);

		tex_rect.w *= x_scale;
		tex_rect.h *= y_scale;

		SDL_Point pivot_point = { static_cast<int>(request.pivot_x * tex_rect.w), static_cast<int>(request.pivot_y * tex_rect.h) };

		glm::ivec2 cam_dimensions = glm::ivec2(window_size.x, window_size.y);

		tex_rect.x = static_cast<int>(final_rendering_position.x * PIXELS_PER_METER + cam_dimensions.x * 0.5f * (1.0f / zoom_factor) - pivot_point.x);
		tex_rect.y = static_cast<int>(final_rendering_position.y * PIXELS_PER_METER + cam_dimensions.y * 0.5f * (1.0f / zoom_factor) - pivot_point.y);

		// Apply tint / alpha to texture
		SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
		SDL_SetTextureAlphaMod(tex, request.a);

		// Preform draw
		SDL_RenderCopyEx(GetRenderer(), tex, NULL, &tex_rect, request.rotation_degrees,
			&pivot_point, static_cast<SDL_RendererFlip>(flip_mode));

		// Remove tint / alpha from texture
		SDL_SetTextureColorMod(tex, 255, 255, 255);
		SDL_SetTextureAlphaMod(tex, 255);
	}

	SDL_RenderSetScale(renderer, 1, 1);

	image_draw_request_queue.clear();
}

/**
Renders all text draw requests in the text_draw_request_queue
*/
void RendererData::RenderAndClearAllTextRequests()
{
	for (auto& request : text_draw_request_queue) {
		SDL_Color font_color = { static_cast<Uint8>(request.r), static_cast<Uint8>(request.g), static_cast<Uint8>(request.b), static_cast<Uint8>(request.a) };
		SDL_Texture* text_texture = ConvertTextToTexture(renderer, request.text, font_color, request.font, request.size);

		SDL_Rect dest_rect;
		dest_rect.x = request.x;
		dest_rect.y = request.y;
		SDL_QueryTexture(text_texture, nullptr, nullptr, &dest_rect.w, &dest_rect.h); // get w and h from the text

		double angle = 0;
		SDL_Point* center = nullptr;
		SDL_RenderCopyEx(GetRenderer(), text_texture, nullptr, &dest_rect, angle, center, SDL_FLIP_NONE);
		SDL_DestroyTexture(text_texture);
	}

	text_draw_request_queue.clear();
}

/**
Renders all UI draw requests in the ui_draw_request_queue
*/
void RendererData::RenderAndClearAllUI()
{
	std::stable_sort(ui_draw_request_queue.begin(), ui_draw_request_queue.end(), CompareUIRequests());
	SDL_RenderSetScale(renderer, 1, 1);

	for (auto& request : ui_draw_request_queue) {
		glm::vec2 final_rendering_position = glm::vec2(request.x, request.y);

		SDL_Texture* tex = GetImage(request.image_name);
		SDL_Rect tex_rect;
		SDL_QueryTexture(tex, NULL, NULL, &tex_rect.w, &tex_rect.h);

		tex_rect.x = static_cast<int>(final_rendering_position.x);
		tex_rect.y = static_cast<int>(final_rendering_position.y);

		// Apply tint / alpha to texture
		SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
		SDL_SetTextureAlphaMod(tex, request.a);

		// Preform draw
		SDL_RenderCopyEx(GetRenderer(), tex, NULL, &tex_rect, 0,
			NULL, SDL_FLIP_NONE);

		// Remove tint / alpha from texture
		SDL_SetTextureColorMod(tex, 255, 255, 255);
		SDL_SetTextureAlphaMod(tex, 255);
	}

	ui_draw_request_queue.clear();
}

/**
Renders all of the pixel draw requests in the pixel_draw_request_queue
*/
void RendererData::RenderAndClearAllPixels()
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // needed to ensure that alpha works

	for (auto& request : pixel_draw_request_queue) {
		SDL_SetRenderDrawColor(renderer, request.r, request.g, request.b, request.a);
		SDL_RenderDrawPoint(renderer, request.x, request.y);
	}

	pixel_draw_request_queue.clear();

	// reset renderer when finished
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255);
}

/**
* Renders all of the line draw requests in the line_draw_request_queue
*/
void RendererData::RenderAndClearAllLines()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // needed to ensure that alpha works

    for (auto& request : line_draw_request_queue) {
        SDL_SetRenderDrawColor(renderer, request.r, request.g, request.b, request.a);
        
        glm::vec2 final_rendering_position1 = glm::vec2(request.x1, request.y1) - current_cam_pos;
        glm::vec2 final_rendering_position2 = glm::vec2(request.x2, request.y2) - current_cam_pos;
        
        glm::ivec2 cam_dimensions = glm::ivec2(window_size.x, window_size.y);
        
        int x1 = static_cast<int>(final_rendering_position1.x * PIXELS_PER_METER + cam_dimensions.x * 0.5f * (1.0f / zoom_factor));
        int y1 = static_cast<int>(final_rendering_position1.y * PIXELS_PER_METER + cam_dimensions.y * 0.5f * (1.0f / zoom_factor));
        int x2 = static_cast<int>(final_rendering_position2.x * PIXELS_PER_METER + cam_dimensions.x * 0.5f * (1.0f / zoom_factor));
        int y2 = static_cast<int>(final_rendering_position2.y * PIXELS_PER_METER + cam_dimensions.y * 0.5f * (1.0f / zoom_factor));
        
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    line_draw_request_queue.clear();

    // reset renderer when finished
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255);
}

/**
* Creates a UI draw request at the specified screen position using the image with name 'image_name'
*
* @param    image_name    The name of the image to be draw
* @param    x            The x position to draw the image at
* @param    y            The y position to draw the image at
*/
void RendererData::DrawUI(const std::string& image_name, const float x, const float y)
{
	UIRenderRequest obj;
	obj.image_name = image_name;
	obj.x = static_cast<int>(x);
	obj.y = static_cast<int>(y);

	ui_draw_request_queue.push_back(obj);
}

/**
* Creates a UI draw request at the specified screen position, with the color {r, g, b, a},
* and in the given sorting layer
*
* @param	image_name		The name of the image to be draw
* @param	x				The x position to draw the image at
* @param	y				The y position to draw the image at
* @param	r				[0, 255] How red the image is
* @param	g				[0, 255] How green the image is
* @param	b				[0, 255] How blue the image is
* @param	a				[0, 255] The alpha value of the image
* @param	sorting_order	The sorting layer that the image should be drawn in
*/
void RendererData::DrawUIEx(const std::string& image_name, const float x, const float y, const float r, const float g, const float b, const float a, const float sorting_order)
{
	UIRenderRequest obj;
	obj.image_name = image_name;
	obj.x = static_cast<int>(x);
	obj.y = static_cast<int>(y);
	obj.r = static_cast<int>(r);
	obj.g = static_cast<int>(g);
	obj.b = static_cast<int>(b);
	obj.a = static_cast<int>(a);
	obj.sorting_order = static_cast<int>(sorting_order);

	ui_draw_request_queue.push_back(obj);
}

/**
* Creates an image draw request at the specified screen position using the image with name 'image_name'
*
* @param	image_name	The name of the image to be drawn
* @param	x			The x position to draw the image at
* @param	y			The y position to draw the iamge at
*/
void RendererData::DrawImage(const std::string& image_name, const float x, const float y)
{
	ImageDrawRequest obj;
	obj.image_name = image_name;
	obj.x = x;
	obj.y = y;

	image_draw_request_queue.push_back(obj);
}

/**
* Creates an image draw request for the image with name 'image_name' with more control than DrawImage
*
* @param	image_name			The name of the image to be drawn
* @param	x					The x position to draw the image at
* @param	y					The y position to draw the iamge at
* @param	rotation_degrees	The rotation of the image in degrees
* @param	scale_x				The scale to draw the x-axis. 1 is normal
* @param	scale_y				The scale to draw the y-axis. 1 is normal
* @param	pivot_x				[0, 1] Where on the x position of the image should be located. 0 is the left side of the image and 1 is the right.
* @param	pivot_y				[0, 1] Where on the y position of the image should be located. 0 is the top side of the image and 1 is the bottom.
* @param	r					[0, 255] How red the image is
* @param	g					[0, 255] How green the image is
* @param	b					[0, 255] How blue the image is
* @param	a					[0, 255] The alpha value of the image
* @param	sorting_order		The sorting layer that the image should be drawn in			
*/
void RendererData::DrawImageEx(const std::string& image_name, const float x, const float y, const float rotation_degrees, const float scale_x, const float scale_y, const float pivot_x, const float pivot_y, const float r, const float g, const float b, const float a, const float sorting_order)
{
	ImageDrawRequest obj;
	obj.image_name = image_name;
	obj.x = x;
	obj.y = y;
	obj.rotation_degrees = static_cast<int>(rotation_degrees);
	obj.scale_x = scale_x;
	obj.scale_y = scale_y;
	obj.pivot_x = pivot_x;
	obj.pivot_y = pivot_y;
	obj.r = static_cast<int>(r);
	obj.g = static_cast<int>(g);
	obj.b = static_cast<int>(b);
	obj.a = static_cast<int>(a);
	obj.sorting_order = static_cast<int>(sorting_order);

	image_draw_request_queue.push_back(obj);
}

/**
* Creates a pixel draw request at the specified (x, y) screen position with color {r, g, b, a}
*
* @param	x	the x of the screen position for the pixel to be drawn
* @param	y	the x of the screen position for the pixel to be drawn
* @param	r	the red value of the color of the pixel to be drawn [0, 255]
* @param	g	the green value of the color of the pixel to be drawn [0, 255]
* @param	b	the blue value of the color of the pixel to be drawn [0, 255]
* @param	a	the alpha value of the color of the pixel to be drawn [0, 255]
*/
void RendererData::DrawPixel(const float x, const float y, const float r, const float g, const float b, const float a)
{
	PixelDrawRequest obj;
	obj.x = static_cast<int>(x);
	obj.y = static_cast<int>(y);
	obj.r = static_cast<int>(r);
	obj.g = static_cast<int>(g);
	obj.b = static_cast<int>(b);
	obj.a = static_cast<int>(a);

	pixel_draw_request_queue.push_back(obj);
}
				
/*
Creates a line draw request from the specified (x1, y1) to (x2, y2) world position with color {r, g, b, a}

@param    x1    the x of the world position for the first vertex of the line
@param    y1    the y of the world position for the first vertex of the line
@param    x2    the x of the world position for the second vertex of the line
@param    y2    the y of the world position for the second vertex of the line
@param    r    the red value of the color of the pixel to be drawn [0, 255]
@param    g    the green value of the color of the pixel to be drawn [0, 255]
@param    b    the blue value of the color of the pixel to be drawn [0, 255]
@param    a    the alpha value of the color of the pixel to be drawn [0, 255]
*/
void RendererData::DrawLine(const float x1, const float y1, const float x2, const float y2, const float r, const float g, const float b, const float a)
{
    LineDrawRequest obj;
    obj.x1 = x1;
    obj.y1 = y1;
    obj.x2 = x2;
    obj.y2 = y2;
    obj.r = static_cast<int>(r);
    obj.g = static_cast<int>(g);
    obj.b = static_cast<int>(b);
    obj.a = static_cast<int>(a);
    
    line_draw_request_queue.push_back(obj);
}

/**
* Creates a text draw request for str_content at the specified (x, y) screen position
*	
* @param	str_content		the text to be drawn
* @param	x				the x of the screen position for top left corner of the text
* @param	y				the y of the screen position for top left corner of the text
* @param	font_name		the name of the font to draw the text with
* @param	font_size		how large the text should be drawn
* @param	r	the red value of the color of the text to be drawn [0, 255]
* @param	g	the green value of the color of the text to be drawn [0, 255]
* @param	b	the blue value of the color of the text to be drawn [0, 255]
* @param	a	the alpha value of the color of the text to be drawn [0, 255]
*/
void RendererData::DrawText(const std::string str_content, int x, int y, std::string font_name, int font_size, int r, int g, int b, int a)
{
	TextRenderRequest obj;
	obj.text = str_content;
	obj.font = font_name;
	obj.size = font_size;
	obj.x = x;
	obj.y = y;
	obj.r = r;
	obj.g = g;
	obj.b = b;
	obj.a = a;

	text_draw_request_queue.push_back(obj);
}

/**
* Set the (x, y) position of the camera
*
* @param	x	the new x screen position of the camera
* @param	y	the new y screen position of the camera
*/
void RendererData::SetCameraPosition(const float x, const float y)
{
	current_cam_pos = { x, y };
}

/**
* Get the (x, y) position of the camera as a glm::vec2
*
* @returns	  a glm::vec2 containing the camera's current position
*/
glm::vec2 RendererData::GetCameraPosition()
{
	return current_cam_pos;
}

/**
* Set the zoom factor of the camera
*
* @param	new_zoom_factor		the new zoom factor for the camera
*/
void RendererData::SetCameraZoom(const float new_zoom_factor)
{
	zoom_factor = new_zoom_factor;
}

/**
* Gets the current zoom factor of the camera
*
* @returns	  the current zoom factor of the camera
*/
float RendererData::GetCameraZoom()
{
	return zoom_factor;
}

/**
 * Cleans up the SDL renderer and window
 */
void RendererData::Cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
