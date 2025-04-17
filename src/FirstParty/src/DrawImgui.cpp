//
//	DrawImgui.cpp
//	wolverine_engine
//
//	Created by Cmaeron Reid 1/25/25
//  Contains Draw functions and helper functions for drawing in Imgui windows

#include "DrawImgui.h"

/**
* Renders all image draw requests in the image_draw_request_queue to a ImGui widget
*/
void DrawImgui::ImageToImGUI()
{
    std::stable_sort(RendererData::GetImageDrawRequestQueue()->begin(), RendererData::GetImageDrawRequestQueue()->end(), CompareImageRequests());
    SDL_RenderSetScale(RendererData::GetRenderer(), RendererData::GetCameraZoom(), RendererData::GetCameraZoom());

    for (auto& request : *RendererData::GetImageDrawRequestQueue()) {
        glm::vec2 final_rendering_position = glm::vec2(request.x, request.y) - RendererData::GetCameraPosition();

        SDL_Texture* tex = GetImage(request.image_name);
        int tex_w = 0;
        int tex_h = 0;
        SDL_QueryTexture(tex, NULL, NULL, &tex_w, &tex_h);

        // Apply scale
        float x_scale = request.scale_x;
        float y_scale = request.scale_y;

        // account for current zoom factor
        float zoom = (1.0f / RendererData::GetCameraZoom());

        ImVec2 tex_size = ImVec2(tex_w * x_scale * zoom, tex_h * y_scale * zoom);

        // Calculate pivot point
        ImVec2 pivot_point = ImVec2(static_cast<int>(request.pivot_x * tex_size.x), static_cast<int>(request.pivot_y * tex_size.y));

        ImVec2 window_size = ImGui::GetWindowSize();

        // center the image
        float center_offset = 0.5f;

        // Calculate final rendering position
        ImVec2 final_pos = ImVec2(final_rendering_position.x * RendererData::PIXELS_PER_METER + window_size.x * center_offset * zoom - pivot_point.x,
            final_rendering_position.y * RendererData::PIXELS_PER_METER + window_size.y * center_offset * zoom - pivot_point.y);

        // Apply tint / alpha to texture
        SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
        SDL_SetTextureAlphaMod(tex, request.a);

        // calculate texture coordinates based no rotation
        float rotation_angle = static_cast<float>(request.rotation_degrees) * (M_PI / 180.0f);
        ImVec2 center = ImVec2(0.5f, 0.5f); // Center of the image
        ImVec2 uv0 = ImVec2(0, 0);
        ImVec2 uv1 = ImVec2(1, 1);

        uv0 = RotateUV(uv0, rotation_angle, center); // originally (0, 0)
        uv1 = RotateUV(uv1, rotation_angle, center); // originally (1, 1)

        // Render using ImGui
        ImGui::SetCursorPos(final_pos);
        ImGui::Image((void*)tex, tex_size, uv0 = uv0, uv1 = uv1);

        // Remove tint / alpha from texture
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
    }

    SDL_RenderSetScale(RendererData::GetRenderer(), 1, 1);
    RendererData::GetImageDrawRequestQueue()->clear();
}

/**
* Renders all text draw requests in the to to imgui
*/
void DrawImgui::TextToImGUI()
{
    std::deque<TextRenderRequest>* text_requests = RendererData::GetTextDrawRequestQueue();
    for (auto& request : *text_requests) {
        ImFont* font = GetImGuiFont(request.font, request.size);

        if (font) {
            ImGui::PushFont(font);
        }

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // set position
        ImVec2 current_window_pos = ImGui::GetWindowPos();
        ImVec2 request_position;
        request_position.x = request.x;
        request_position.y = request.y;

        ImVec2 final_position;
        final_position.x = request_position.x + current_window_pos.x;
        final_position.y = request_position.y + current_window_pos.y;

        // color
        ImU32 col = IM_COL32(request.r, request.g, request.b, request.a);

        draw_list->AddText(final_position, col, request.text.c_str());

        // return to the previous font, if used
        if (font)
            ImGui::PopFont();
    }

    text_requests->clear();
}

/**
* Renders all ui draw requests in the to to imgui
*/
void DrawImgui::UIToImGUI()
{
    std::stable_sort(RendererData::GetUIDrawRequestQueue()->begin(), RendererData::GetUIDrawRequestQueue()->end(), CompareUIRequests());

    for (auto& request : *RendererData::GetUIDrawRequestQueue()) {
        // Assuming GetImage returns an ImGui-compatible texture ID
        SDL_Texture* tex = GetImage(request.image_name);

        int tex_w = 0;
        int tex_h = 0;
        SDL_QueryTexture(tex, NULL, NULL, &tex_w, &tex_h);

        ImVec2 tex_size;
        tex_size.x = tex_w;
        tex_size.y = tex_h;

        ImVec2 tex_pos = ImVec2(request.x, request.y);

        // Apply tint / alpha to texture
        ImVec4 tint_color = ImVec4(request.r / 255.0f, request.g / 255.0f, request.b / 255.0f, request.a / 255.0f);

        // Render the image
        ImGui::SetCursorPos(tex_pos);
        ImTextureID tex_id = (ImTextureID)(intptr_t)tex;
        if (tex_id) {
            ImGui::Image(tex_id, tex_size, ImVec2(0, 0), ImVec2(1, 1), tint_color);
        }
        else {
            printf("texture is null\n");
        }

        // Remove tint / alpha from texture (reset to default)
        tint_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    RendererData::GetUIDrawRequestQueue()->clear();
}

/**
* Renders all pixel draw requests in the pixel_draw_request_queue to imgui
*/
void DrawImgui::PixelToImGUI()
{
    const int PIXEL_DRAW_SIZE = 1;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 current_window_pos = ImGui::GetWindowPos();


    for (auto& request : *RendererData::GetPixelDrawRequestQueue()) {
        ImVec2 final_draw_position;
        final_draw_position.x = current_window_pos.x + request.x;
        final_draw_position.y = current_window_pos.y + request.y;

        draw_list->AddRectFilled(final_draw_position, ImVec2(final_draw_position.x + PIXEL_DRAW_SIZE, final_draw_position.y + PIXEL_DRAW_SIZE),
            IM_COL32(request.r, request.g, request.b, request.a));
    }

    RendererData::GetPixelDrawRequestQueue()->clear();
}

/**
* Renders all of the line draw requests in the line_draw_request_queue to imgui
*/
void DrawImgui::LineToImGUI()
{
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // needed to ensure that alpha works

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (auto& request : *RendererData::GetLineDrawRequestQueue()) {

        ImVec2 current_window_pos = ImGui::GetWindowPos();

        glm::vec2 final_rendering_position1 = (glm::vec2(request.x1, request.y1) - RendererData::GetCameraPosition());
        glm::vec2 final_rendering_position2 = (glm::vec2(request.x2, request.y2) - RendererData::GetCameraPosition());

        glm::ivec2 cam_dimensions = glm::ivec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        int x1 = static_cast<int>(final_rendering_position1.x * RendererData::PIXELS_PER_METER + cam_dimensions.x * 0.5f * (1.0f / RendererData::GetCameraZoom()));
        int y1 = static_cast<int>(final_rendering_position1.y * RendererData::PIXELS_PER_METER + cam_dimensions.y * 0.5f * (1.0f / RendererData::GetCameraZoom()));
        int x2 = static_cast<int>(final_rendering_position2.x * RendererData::PIXELS_PER_METER + cam_dimensions.x * 0.5f * (1.0f / RendererData::GetCameraZoom()));
        int y2 = static_cast<int>(final_rendering_position2.y * RendererData::PIXELS_PER_METER + cam_dimensions.y * 0.5f * (1.0f / RendererData::GetCameraZoom()));

        draw_list->AddLine(ImVec2(x1 + current_window_pos.x, y1 + current_window_pos.y), ImVec2(x2 + current_window_pos.x, y2 + current_window_pos.y), IM_COL32(request.r, request.g, request.b, request.a));
    }

    RendererData::GetLineDrawRequestQueue()->clear();
}

/**
* Loads the fonts that are needed for text requests this frame, if not already loaded
*
* NOTE: Must be called before ImGui::NewFrame() and after ImGui::Render()
*/
void DrawImgui::LoadFontsImGUI()
{
    std::deque<TextRenderRequest>* text_requests = RendererData::GetTextDrawRequestQueue();
    for (auto& request : *text_requests) {
        bool font_found = false;

        // check for the font needed and that it is the correct size
        for (auto& font : imgui_fonts[request.font]) {
            if (font != nullptr && font->FontSize == request.size) {
                font_found = true;
                break;
            }
        }

        // the font was not found with the size needed, load it
        if (!font_found) {
            const std::string path = FileUtils::GetPath("resources/fonts/") + request.font + ".ttf";

            if (FileUtils::DirectoryExists("resources/fonts/"))
            {
                ImGuiIO& io = ImGui::GetIO();
                ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), request.size);

                if (font == nullptr) {
                    std::cerr << "Failed to load font!" << std::endl;
                }
                else {
                    io.Fonts->Build();

                    imgui_fonts[request.font].push_back(font); // save the font for later

                    // these two lines must be called whenever loading fonts between frames
                    ImGui_ImplSDLRenderer2_DestroyDeviceObjects();
                    ImGui_ImplSDLRenderer2_CreateDeviceObjects();
                }
            }
            else {
                // output an error and move to the next request
                std::cout << "font " + request.font + "does not exist at " + path << std::endl;
                continue;
            }
        }
    }
}

/**
* Gets a specified font for ImGui
*
* @returns    A ImFont* to the specified font if it exists, nullptr otherwise
*/
ImFont* DrawImgui::GetImGuiFont(const std::string& name, const float size)
{
    ImFont* return_font = nullptr;

    for (auto& font : imgui_fonts.at(name)) {
        if (font->FontSize == size) {
            return_font = font;
            break;
        }
    }

    if (!return_font) {
        std::cout << "font " + name + "not found at size " << size << std::endl;
    }

    return return_font;
}

/**
* Add a specified font for ImGui
*/
void DrawImgui::AddImGuiFont(const std::string& name, ImFont* font)
{
    imgui_fonts[name].push_back(font);
}

/**
* Calculates new texture coordinates based on the original coordinates, the angle of rotation, and the image's center
*
* @parameters    uv        The original texture coordinates
* @parameters    angle     The angle to rotate by
* @parameters    center    The center of the image
*
* @returns       rotated   A ImVec2 containing the new texture coordinates
*/
ImVec2 DrawImgui::RotateUV(ImVec2& uv, float angle, ImVec2& center) {
    // will only work for multiples of 90 degrees
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    ImVec2 rotated;

    rotated.x = roundf(cosA * (uv.x - center.x) - sinA * (uv.y - center.y) + center.x);
    rotated.y = roundf(sinA * (uv.x - center.x) + cosA * (uv.y - center.y) + center.y);

    return rotated;
}
