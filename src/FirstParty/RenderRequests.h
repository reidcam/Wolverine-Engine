//
//	RenderRequests.h
//	wolverine_engine
//
//	Created by Cameron Reid on 5/21/24.
//	Holds all of the structs for different rendering requests

#ifndef RenderRequests_h
#define RenderRequests_h

#include <string>
#include <vector>

// Image Render Request
// default values are commented next to their respective data structure

//std::vector<std::string> image_render_request_image_name; // ""
//std::vector<float> image_render_request_x; // 0.0f
//std::vector<float> image_render_request_y; // 0.0f
//std::vector<int> image_render_request_rotation_degrees; // 0
//std::vector<float> image_render_request_scale_x; // 1.0f
//std::vector<float> image_render_request_scale_y; // 1.0f
//std::vector<float> image_render_request_pivot_x; // 0.5f
//std::vector<float> image_render_request_pivot_y; // 0.5f
//std::vector<int> image_render_request_r; // 255
//std::vector<int> image_render_request_g; // 255
//std::vector<int> image_render_request_b; // 255
//std::vector<int> image_render_request_a; // 255
//std::vector<int> image_render_request_sorting_order; // 0

struct ImageDrawRequest {
	std::string image_name = "";
	float x = 0.0f;
	float y = 0.0f;
	int rotation_degrees = 0;
	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float pivot_x = 0.5f;
	float pivot_y = 0.5f;
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
	int sorting_order = 0;
}; // ImageDrawRequest

// Text Render Request
// default values are commented next to their respective data structure

//std::vector<std::string> text_render_request_text; // ""
//std::vector<std::string> text_render_request_font; // ""
//std::vector<int> text_render_request_r; // 0
//std::vector<int> text_render_request_g; // 0
//std::vector<int> text_render_request_b; // 0
//std::vector<int> text_render_request_a; // 0
//std::vector<int> text_render_request_size; // 0
//std::vector<int> text_render_request_x; // 0
//std::vector<int> text_render_request_y; // 0

struct TextRenderRequest {
	std::string text = "";
	std::string font = "";
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;
	int size = 0;
	int x = 0;
	int y = 0;
}; // TextRenderRequest

// UI Render Request
// default values are commented next to their respective data structure

//std::vector<std::string> ui_render_request_image_name; // ""
//std::vector<int> ui_render_request_x; // 0
//std::vector<int> ui_render_request_y; // 0
//std::vector<int> ui_render_request_r; // 255
//std::vector<int> ui_render_request_g; // 255
//std::vector<int> ui_render_request_b; // 255
//std::vector<int> ui_render_request_a; // 255
//std::vector<int> ui_render_request_sorting_order; // 0

struct UIRenderRequest {
	std::string image_name = "";
	int x = 0;
	int y = 0;
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
	int sorting_order = 0;
}; // UIRenderRequest

// Pixel Render Request
// default values are commented next to their respective data structure

//std::vector<int> pixel_render_request_x; // 0
//std::vector<int> pixel_render_request_y; // 0
//std::vector<int> pixel_render_request_r; // 255
//std::vector<int> pixel_render_request_g; // 255
//std::vector<int> pixel_render_request_b; // 255
//std::vector<int> pixel_render_request_a; // 255

struct PixelDrawRequest
{
	int x = 0;
	int y = 0;
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
}; // PixelDrawRequest

#endif /* RenderRequests.h */