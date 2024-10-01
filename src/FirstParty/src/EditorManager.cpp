//
//  EditorManager.cpp
//  wolverine-engine-demo
//
//  Created by Jacob Robinson on 9/22/24.
//  Handles the creation and functions of the editor GUI
//  DEBUG MODE ONLY
//

#include <stdio.h>

#include "EditorManager.h"
#include "SceneManager.h"
#include "PhysicsWorld.h"
#include "LuaAPI.h"

bool EditorManager::editor_mode = true; // True when the game is paused and edits can be made
bool EditorManager::play_mode = false; // True after the play button is pressed until the stop button is pressed. No edits can be made in this mode.
bool EditorManager::trigger_editor_mode_toggle = false;
int EditorManager::selected_actor_id = -1; // The actor ID of the selected actor in the hierarchy view.

//-------------------------------------------------------
// Lifecycle

/**
* Initializes the editor
*/
void EditorManager::Init()
{
    // Set up imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.IniFilename = NULL; // Disable automatic .ini file handling for docking layouts
    
    // Set up imgui style
    ImGui::StyleColorsDark();
    
    // Set up platfomr/renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(GUIRenderer::GetWindow(), GUIRenderer::GetRenderer());
    ImGui_ImplSDLRenderer2_Init(GUIRenderer::GetRenderer());

    // init file path
    docking_layout_file_path = std::filesystem::path(FileUtils::GetPath(docking_layout_sub_path));
}

/**
* Passes SDL input events to imgui
* @param    event   the SDL event to process
*/
void EditorManager::ImGuiProcessSDLEvent(const SDL_Event* event)
{
    ImGui_ImplSDL2_ProcessEvent(event);
}

/**
 * Renders all of the editor windows
 */
void EditorManager::RenderEditor()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    ViewportDocking();

    // check to see if any windows should be opened/closed this frame
    CheckEditorShortcuts();

    // Create all of the ImGui windows
    MainMenuBar();
    HierarchyView();
    ModeSwitchButtons();
    
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), GUIRenderer::GetRenderer());
}

/**
 * Updates all of the needed aspects of the engine
 * Run once every frame while in editor mode ONLY
 */
void EditorManager::EditorUpdate()
{
    Actors::EditorStartComponents(editor_components_list);
    Actors::EditorUpdateComponents(editor_components_list);
}

/**
* Cleans up the imgui context when the game is closed
*/
void EditorManager::Cleanup()
{
    std::filesystem::path path = docking_layout_file_path.string() + "/" + user_docking_layout_file_name;
    SaveIniSettingsToDisk(path.string()); // save the current docking layout before closing
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

/**
 * Remakes the local scene file with all of our changes
 */
void EditorManager::UpdateSceneLocal()
{
    rapidjson::Document updated_scene;
    updated_scene.SetObject(); // Init the scene file as an object
    
    // Create an allocator (required for memory management in RapidJSON)
    rapidjson::Document::AllocatorType& allocator = updated_scene.GetAllocator();
    
    rapidjson::Value actors;
    actors.SetArray();
    
    for (int actor_id : Scene::GetAllActorsInScene())
    {
        rapidjson::Document actor;
        actor.SetObject(); // Init the actor as an object
        
        // Create an allocator (required for memory management in RapidJSON)
        rapidjson::Document::AllocatorType& a_allocator = actor.GetAllocator();
        
        // Adds the contents of the actor to its json object:
        rapidjson::Value actor_name;
        std::string name = Actors::GetName(actor_id);
        actor_name.SetString(&name[0], name.length());
        actor.AddMember("name", actor_name, a_allocator);
        
        actors.PushBack(actor, allocator);
    }
    
    updated_scene.AddMember("actors", actors, allocator);
    
    EngineUtils::WriteJsonFile(GetScenePath("writing_test"), updated_scene);
}

//-------------------------------------------------------
// Getters/Setters

/**
* If editor mode is on, turn it off, if it is off, turn it on
*/
void EditorManager::ToggleEditorMode()
{
    if (editor_mode) { editor_mode = false; }
    else { editor_mode = true; }
}

/**
* Returns the editor mode
*/
bool EditorManager::GetEditorMode()
{
    return editor_mode;
}

//-------------------------------------------------------
// ImGui Item Displays

/**
 * Displays the given variable in a sol::table in the heirarchy view as a sub-item of that table
 *
 * @param   table    the table that contains the variable we want to display
 * @param   key         the key of the variable that we want to display
 */
void EditorManager::VariableView(sol::table* table, sol::lua_value key)
{
    sol::object value = (*table)[key];
    
    std::string var_name = "???";
    // Converts the key to a string from its lua_type for use in imgui labels
    if (key.is<std::string>()) { var_name = key.as<std::string>(); }
    else if (key.is<bool>()) { var_name = key.as<bool>() ? "true" : "false"; }
    else if (key.is<int>() || key.is<float>() || key.is<double>()) { var_name = to_string(key.as<int>()); }
    else if (key.is<sol::function>()) { var_name = "FUNCTION_KEY"; }
    else if (key.is<sol::table>()) { var_name = "TABLE_KEY"; }
    else { var_name = "UNKNOWN_KEY"; }
    
    
    const char* const_var_name = &var_name[0];
    
    // Skip the variables that exist for engine use: key, actor, type, or any native component values added by Lua
    if (var_name == "key" || var_name == "type" || var_name == "actor" ||
        var_name == "class_cast" || var_name == "REMOVED_FROM_ACTOR" || var_name == "class_check" ||
        var_name == "__type" || var_name == "__name") { return; }
    
    // Skip functions
    if (value.get_type() == sol::type::function) { return; }
    
    // Invisible variable Name
    // ## allows us to have a unique ImGui ID for this item without showing the ID in the UI
    std::string invisible_id = "##" + var_name;
    const char* const_invisible_id = &invisible_id[0];
    
    // Sets the first column to be the name of the variable
    ImGui::TableNextColumn();
    ImGui::Text(const_var_name);
    
    // Moves to the second column to get ready to be the value of the variable
    ImGui::TableNextColumn();
    
    // Variable value
    if (value.get_type() == sol::type::string)
    {
        std::string variable_value = value.as<std::string>();
        char* const_var_value = &variable_value[0];
        if (ImGui::InputText(const_invisible_id, const_var_value, variable_value.size()) && (ImGui::IsItemEdited() && ImGui::IsItemDeactivated()))
        {
            (*table)[key] = const_var_value;
        }
    }
    if (value.get_type() == sol::type::number)
    {
        if (value.is<int>())
        {
            int variable_value = value.as<int>();
            
            // Set the value of this variable if it's changed in the editor
            if (ImGui::InputInt(const_invisible_id, &variable_value))
            {
                (*table)[key] = variable_value;
            }
        }
        else if (value.is<double>())
        {
            double variable_value = value.as<double>();
            
            // Set the value of this variable if it's changed in the editor
            if (ImGui::InputDouble(const_invisible_id, &variable_value))
            {
                (*table)[key] = variable_value;
            }
        }
    }
    if (value.get_type() == sol::type::boolean)
    {
        bool variable_value = value.as<bool>();
        
        // Set the value of this variable if it's changed in the editor
        if (ImGui::Checkbox(const_invisible_id, &variable_value))
        {
            (*table)[key] = variable_value;
        }
    }
    if (value.get_type() == sol::type::table)
    {
        sol::table variable_value = value;
        
        // If this table variable is a Lua component, DO NOT RENDER AS TABLE IN HIERARCHY!
        // This would cause massive problems
        if (!variable_value["actor"].valid())
        {
            // Used to detect the items in our variable that aren't contained in its metatable
            // by storing the ones that the metatable contains.
            sol::table found_items = LuaAPI::GetLuaState()->create_table();
            
            // Table allows us to cleanly format our variables
            ImGui::BeginTable(const_invisible_id, 2);
        
            // Displays all of the items in the table that exist before its initialized, IFF this table is an instance at all
            if (variable_value[sol::metatable_key].valid())
            {
                sol::table metatable = variable_value[sol::metatable_key]["__index"];
                for (auto& item : metatable)
                {
                    sol::lua_value item_key = item.first;
                    found_items[item_key] = 0;
                    // Sets this row of the table to be the variable with the given key
                    VariableView(&variable_value, item_key);
                }
            }
            // Displays all of the items in the table that are created during runtime
            for (auto& item : variable_value)
            {
                sol::lua_value item_key = item.first;
                if (!found_items[item_key].valid())
                {
                    // Sets this row of the table to be the variable with the given key
                    VariableView(&variable_value, item_key);
                }
            }
            ImGui::EndTable();
            
            // Not sure if this is needed, but I want to make sure that Lua knows it can throw away this table since we're
            // done with it.
            found_items = sol::lua_nil;
            
            (*table)[key] = variable_value;
        }
        else
        {
            // If the table variable is a component, write out its actor owner and type instead
            std::string actor_name = Actors::GetName(variable_value["actor"].get<Actor*>()->ID);
            std::string component_type = variable_value["type"];
            std::string placeholder_value =  actor_name + "_" + component_type;
            const char* const_var_value = &placeholder_value[0];
            ImGui::Text(const_var_value);
        }
    }
    
    // Move on to the next row of the table
    ImGui::TableNextRow();
}

/**
 * Creates the mode switching buttons
 */
void EditorManager::ModeSwitchButtons()
{
    // Flags
    bool* display_window = new bool(true);
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoTitleBar;
    
    int window_w = 0;
    int window_h = 0;
    SDL_GetWindowSize(GUIRenderer::GetWindow(), &window_w, &window_h);
    
    // Window Size
    int imgui_window_w = 110.0f;
    int imgui_window_h = 40.0f;
    ImGui::SetNextWindowSize(ImVec2(imgui_window_w, imgui_window_h));
    
    // Window Position
    int imgui_window_x = (window_w / 2) - (imgui_window_w / 2);
    int imgui_window_y = 0.0f;
    ImGui::SetNextWindowPos(ImVec2(imgui_window_x, imgui_window_y));
    
    // Alows developers to activate the play modes and editor modes
    ImGui::Begin("Play/Pause", display_window, flags);
    if (!play_mode)
    {
        if (ImGui::Button("Play"))
        {
            // TOOD: Hot reload all modified scenes and scripts
//            UpdateSceneLocal();
            editor_mode = false;
            play_mode = true;
            PhysicsWorld::ResetWorld();
            Scene::ResetManager();
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause") && play_mode) {trigger_editor_mode_toggle = true;}
    }
    else
    {
        if (ImGui::Button("Stop"))
        {
            // TOOD: Hot reload all modified scenes and scripts
            editor_mode = true;
            play_mode = false;
            PhysicsWorld::ResetWorld();
            Scene::ResetManager();
        }
        ImGui::SameLine();
        if (editor_mode)
        {
            if (ImGui::Button("Unpause")) { trigger_editor_mode_toggle = true; }
        }
        else
        {
            if (ImGui::Button("Pause")) { trigger_editor_mode_toggle = true; }
        }
    }
    ImGui::End();
    delete display_window;
}

/**
 * Creates the actor hierarchy view
 */
void EditorManager::HierarchyView()
{
    if (hierarchy) {
        // window flags
        ImGuiWindowFlags window_flags = 0;

        // Window Size
        int window_w = 0;
        int window_h = 0;
        SDL_GetWindowSize(GUIRenderer::GetWindow(), &window_w, &window_h);

        int imgui_window_w = 300.0f;
        int imgui_window_h = window_h;
        ImGui::SetNextWindowSize(ImVec2(imgui_window_w, imgui_window_h));

        // Window Position
        int imgui_window_x = 0.0f;
        int imgui_window_y = ImGui::GetFrameHeightWithSpacing() - 5.0f;
        ImGui::SetNextWindowPos(ImVec2(imgui_window_x, imgui_window_y), ImGuiCond_Once);

        // Alows developers to click on specifc actors and components to change values
        ImGui::Begin("Hierarchy View", &hierarchy, window_flags);

        // Find the selected actor
        for (int actor_id : Scene::GetAllActorsInScene())
        {
            std::string actor_name = Actors::GetName(actor_id);
            const char* const_name = &actor_name[0];

            bool actor_enabled = Actors::GetActorEnabled(actor_id);

            // If the checkbox is clicked toggle the actor's 'enabled' status
            // The ## hides the id for the item
            std::string checkbox_id = "##" + actor_name;
            const char* const_checkbox_id = &checkbox_id[0];
            if (ImGui::Checkbox(const_checkbox_id, &actor_enabled)) { Actors::SetActorEnabled(actor_id, actor_enabled); }

            ImGui::SameLine();

            // If an actor is clicked display its components
            if (ImGui::Button(const_name)) { selected_actor_id = actor_id; };
        }

        // Display the components of the selected actor
        int number_of_components = Actors::GetNumberOfComponents(selected_actor_id);
        for (int i = 0; i < number_of_components; i++)
        {
            sol::table component = Actors::GetComponentByIndex(selected_actor_id, i);

            if (component.valid())
            {
                std::string component_type = component["type"];
                const char* const_type = &component_type[0];

                // If a component is clicked display its properties
                if (ImGui::CollapsingHeader(const_type))
                {
                    sol::table metatable = component[sol::metatable_key];

                    // If component is native, metatable needs to be indexed at __index
                    if (!ComponentManager::IsComponentTypeNative(component_type)) { metatable = metatable["__index"]; }

                    // Table allows us to cleanly format our variables
                    ImGui::BeginTable(const_type, 2);
                    for (auto& variable : metatable)
                    {
                        sol::lua_value key = variable.first;
                        // Sets this row of the table to be the variable with the given key
                        VariableView(&component, key);
                    }
                    ImGui::EndTable();
                }
            }
        }

        ImGui::End();
    }
}

/**
* Creates main menu for the editor window
*/
void EditorManager::MainMenuBar()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Hierarchy", "Crtl+H", hierarchy)) {
                hierarchy = !hierarchy;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Layout")) {
            if (ImGui::MenuItem("Save Layout As")) {
                save_layout_as = !save_layout_as;
            }
            if (ImGui::MenuItem("Default")) {
                std::filesystem::path path = docking_layout_file_path.string() + "/" + user_docking_layout_file_name;
                LoadDockingLayout(path.string());
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // input text window for saving a docking layout
    if (save_layout_as) {
        char inputText[256] = "";
        ImGui::Begin("Save Layout As");
        if (ImGui::InputText("Enter text", inputText, IM_ARRAYSIZE(inputText)) && (ImGui::IsItemEdited() && ImGui::IsItemDeactivated())) {
            std::filesystem::path path = docking_layout_file_path.string() + "/" + inputText;
            SaveIniSettingsToDisk(path.string());
            save_layout_as = !save_layout_as;
        }
        ImGui::End();
    }
}

/**
* Checks to see if editor shortcuts were pressed
*/
void EditorManager::CheckEditorShortcuts()
{
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)) && ImGui::GetIO().KeyCtrl) {
        hierarchy = !hierarchy;
    }
}

/**
* Handles docking for the main viewport
*/
void EditorManager::ViewportDocking()
{
    // Allows the viewport to be used as a docking space
    ImGui::DockSpaceOverViewport(0U, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    // load the most recent user docking layout
    if (first_frame) {
        std::filesystem::path path = docking_layout_file_path.string() + "/" + user_docking_layout_file_name;
        LoadDockingLayout(path.string());
        first_frame = !first_frame;
    }
}
