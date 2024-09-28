//
//	EditorManager.h
//	wolverine_engine
//
//	Created by Jacob Robinson on 9/22/2024.
//	Handles the creation and functions of the editor GUI
//  DEBUG MODE ONLY
//

#ifndef EditorManager_h
#define EditorManager_h

#include <unordered_set>
#include <unordered_map>
#include <string>

#include "Renderer.h"
#include "ComponentManager.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

class EditorManager
{
private:
    // If editor mode is on and play mode is off, ONLY THEN CAN REAL EDITS BE MADE TO THE GAME FILES!
    
    static bool editor_mode; // True when the game is paused.
    static bool play_mode; // True after the play button is pressed until the stop button is pressed. No edits can be made in this mode.
    
    static int selected_actor_id; // The actor ID of the selected actor in the hierarchy view.
    
    inline static std::unordered_set<std::string> editor_components_list
    {
      "SpriteRenderer",
    };

    static inline std::unordered_map<std::string, std::vector<ImFont*>> imgui_fonts = {}; // stores fonts needed for text draw requests for the editor
public:
    static bool trigger_editor_mode_toggle;
    
    //-------------------------------------------------------
    // Lifecycle
    
    /**
    * Initializes the editor
    */
    static void Init();
    
    /**
    * Passes SDL input events to imgui
    *
    * @param    event   the SDL event to process
    */
    static void ImGuiProcessSDLEvent(const SDL_Event* event);
    
    /**
     * Renders all of the editor windows
     */
    static void RenderEditor();
    
    /**
     * Updates all of the needed aspects of the engine
     * Run once every frame while in editor mode ONLY
     */
    static void EditorUpdate();
    
    /**
    * Cleans up the imgui context when the game is closed
    */
    static void Cleanup();
    
    /**
     * Remakes the local scene file with all of our changes
     */
    static void UpdateSceneLocal();
    
    //-------------------------------------------------------
    // Getters/Setters
    
    /**
    * If editor mode is on, turn it off, if it is off, turn it on
    */
    static void ToggleEditorMode();
    
    /**
    * Returns the editor mode
    */
    static bool GetEditorMode();
    
    //-------------------------------------------------------
    // ImGui Item Displays
    
    /**
     * Displays the given variable in a sol::table in the heirarchy view as a sub-item of that table
     *
     * @param   table    the table that contains the variable we want to display
     * @param   key         the key of the variable that we want to display
     */
    static void VariableView(sol::table* table, sol::lua_value key);
    
    /**
     * Creates the mode switching buttons
     */
    static void ModeSwitchButtons();
    
    /**
     * Creates the actor hierarchy view
     */
    static void HierarchyView();

    /**
    * Creates the widget for the viewport and handles rendering
    */
    static void ViewportWidget();

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
};

#endif /* EditorManager.h */
