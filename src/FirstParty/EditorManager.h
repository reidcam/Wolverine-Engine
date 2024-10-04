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

#include "GUIRenderer.h"
#include "Renderer.h"
#include "ComponentManager.h"
#include "FileUtils.h"
#include "EditorStyle.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

class EditorManager
{
private:
    // If editor mode is on and play mode is off, ONLY THEN CAN REAL EDITS BE MADE TO THE GAME FILES!
    
    static bool editor_mode; // True when the game is paused.
    static bool play_mode; // True after the play button is pressed until the stop button is pressed. No edits can be made in this mode.
    inline static bool first_frame = true; // used to tell if the user docking layout should be loaded
    inline static bool save_layout_as = false; // check to see if the input box for the layout should be shown
    
    static int selected_actor_id; // The actor ID of the selected actor in the hierarchy view.
    
    inline static std::unordered_set<std::string> editor_components_list
    {
      "SpriteRenderer",
    };

    inline static std::string user_docking_layout_file_name = "imgui";
    inline static std::string docking_layout_sub_path = "resources/editor_layouts"; // path from the working directory
    inline static std::filesystem::path docking_layout_file_path = ""; // actual path object, needs to be initialized
    inline static std::vector<std::string> editor_layout_files;

    // bools to track if a window is shown
    inline static bool hierarchy = true;
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
    * Creates main menu for the editor window
    */
    static void MainMenuBar();

    /**
    * Checks to see if editor shortcuts were pressed
    */
    static void CheckEditorShortcuts();

    /**
    * Saves the current docking layout to a .ini file
    * 
    * @param    file_name    the name of the file to save to minus the file extension
    */
    inline static void SaveIniSettingsToDisk(const std::string& file_name) { ImGui::SaveIniSettingsToDisk((file_name + ".ini").c_str()); }

    /**
    * Loads the last used layout from a .ini file
    * 
    * @param    file_name    the name of the file to save to minus the file extension 
    */
    inline static void LoadDockingLayout(const std::string& file_name) { ImGui::LoadIniSettingsFromDisk((file_name + ".ini").c_str()); }

    /**
    * Handles docking for the main viewport
    */
    static void ViewportDocking();

    /**
    * Gets all of the editor layout file names from resources/editor_layouts
    * 
    * @return    a vector containing strings of the file names of the editor layout files
    */
    static std::vector<std::string> GetEditorLayouts();
};

#endif /* EditorManager.h */
