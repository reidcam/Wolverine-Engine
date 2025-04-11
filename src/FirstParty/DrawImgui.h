//
//	DrawImgui.h
//	wolverine_engine
//
//	Created by Cmaeron Reid 1/25/25
//  Contains Draw functions and helper functions for drawing in Imgui windows

#ifndef DRAWIMGUI_H
#define DRAWIMGUI_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_map>

#include "Renderer.h"
#include "FileUtils.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

class DrawImgui
{
private:
	static inline std::unordered_map<std::string, std::vector<ImFont*>> imgui_fonts = {}; // stores fonts needed for text draw requests for the editor
public:
    /**
    * Renders all image draw requests in the image_draw_request_queue to imgui
    */
    static void ImageToImGUI();

    /**
    * Renders all text draw requests in the text_draw_request_queue to imgui
    */
    static void TextToImGUI();

    /**
    * Renders all ui draw requests in the ui_draw_request_queue to imgui
    */
    static void UIToImGUI();

    /**
    * Renders all pixel draw requests in the pixel_draw_request_queue to imgui
    */
    static void PixelToImGUI();

    /**
    * Renders all of the line draw requests in the line_draw_request_queue to imgui
    */
    static void LineToImGUI();

    /**
    * Loads the fonts that are needed for text requests this frame, if not already loaded
    *
    * NOTE: Must be called before ImGui::NewFrame() and after ImGui::Render()
    */
    static void LoadFontsImGUI();

    /**
    * Gets a specified font for ImGui
    *
    * @returns    A ImFont* to the specified font if it exists, nullptr otherwise
    */
    static ImFont* GetImGuiFont(const std::string& name, const float size);

    /**
   * Calculates new texture coordinates based on the original coordinates, the angle of rotation, and the image's center
   *
   * @parameters    uv        The original texture coordinates
   * @parameters    angle     The angle to rotate by
   * @parameters    center    The center of the image
   *
   * @returns       rotated   A ImVec2 containing the new texture coordinates
   */
    static ImVec2 RotateUV(ImVec2& uv, float angle, ImVec2& center);
};

#endif /* DrawImgui.h */