#include "Renderer.h"
#include "EngineUtils.h"

void RendererData::Init()
{
    SDL_Window* window = SDL_CreateWindow("test", window_position.x, window_position.y, window_size.x, window_size.y, 0);
    SetWindow(window);
    SDL_Renderer* renderer = SDL_CreateRenderer(RendererData::window, -1, SDL_RENDERER_PRESENTVSYNC + SDL_RENDERER_ACCELERATED);
    SetRenderer(renderer);

    SDL_SetRenderDrawColor(RendererData::GetRenderer(), clear_color_r, clear_color_g, clear_color_b, clear_color_a); // set renderer draw color
}

void RendererData::LoadCameraSettings()
{
	rapidjson::Document doc;
	std::string file_path = "resources/rendering.config";

	// read in the document
	EngineUtils::ReadJsonFile(file_path, doc);

	// parse the JSON
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
}

void RendererData::SetWindowFullscreen(int flag)
{
	//TODO: add SDL error checking
	SDL_SetWindowFullscreen(GetWindow(), flag);
}

void RendererData::RenderAndClearAllImageRequests()
{
	std::stable_sort(image_draw_request_queue.begin(), image_draw_request_queue.end(), CompareImageRequests());
	SDL_RenderSetScale(renderer, zoom_factor, zoom_factor);

	for (auto& request : image_draw_request_queue) {
		const int pixels_per_meter = 100;
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

		tex_rect.x = static_cast<int>(final_rendering_position.x * pixels_per_meter + cam_dimensions.x * 0.5f * (1.0f / zoom_factor) - pivot_point.x);
		tex_rect.y = static_cast<int>(final_rendering_position.y * pixels_per_meter + cam_dimensions.y * 0.5f * (1.0f / zoom_factor) - pivot_point.y);

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
