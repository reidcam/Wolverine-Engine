//
//	RenderRequests.h
//	wolverine_engine
//
//	Created by Cameron Reid on 5/21/24.
//	Holds all of the structs for different rendering requests

#ifndef RenderRequests_h
#define RenderRequests_h

#include <string>

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