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
#include "ComponentDB.h"
#include "LuaAPI.h"
#include "TextDB.h"

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

    // temp fix for background transparency REMOVE LATER AFTER JAM
    auto& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    const ImVec4 bgColor = ImVec4(0.1, 0.1, 0.1, 0.0);
    colors[ImGuiCol_WindowBg] = bgColor;
    colors[ImGuiCol_ChildBg] = bgColor;
    colors[ImGuiCol_TitleBg] = bgColor;
    /////////////////////////////////

    // Set up platfomr/renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(RendererData::GetWindow(), RendererData::GetRenderer());
    ImGui_ImplSDLRenderer2_Init(RendererData::GetRenderer());

    // init file path
    docking_layout_file_path = std::filesystem::path(FileUtils::GetPath("editor_resources/editor_layouts"));
    
    // set path for file viewer
    current_path = std::filesystem::path(FileUtils::GetPath("resources"));

    // get all of the .ini files in resources/editor_layouts
    editor_layout_files = GetEditorLayouts();
    
    // Get all of the data from the editor.config
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
    DrawImgui::LoadFontsImGUI(); // NOTE: Must be called before ImGui::NewFrame() and after ImGui::Render()
    ImGui::NewFrame();
    
    ViewportDocking();

    // check to see if any windows should be opened/closed this frame
    CheckEditorShortcuts();

    //ImGui::ShowDemoWindow();
    // Create all of the ImGui windows
    MainMenuBar();
    HierarchyView();
    ModeSwitchButtons();

    if (show_file_selector)
        ShowFileSelector();
    
    ViewportWidget();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), RendererData::GetRenderer());
}

/**
 * Updates all of the needed aspects of the engine
 * Run once every frame while in editor mode ONLY
 */
void EditorManager::EditorUpdate()
{
    Actors::EditorStartComponents(editor_components_list);
    Actors::EditorUpdateComponents(editor_components_list);
    // Processes all of the components removed from actors this frame
    Actors::ProcessRemovedComponents();
    Scene::DestroyFinalStep();
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
    rapidjson::Document updated_scene(rapidjson::kObjectType); // Init the scene file as an object
    // Create an allocator (required for memory management in RapidJSON)
    rapidjson::Document::AllocatorType& allocator = updated_scene.GetAllocator();
    
    rapidjson::Value actors(rapidjson::kArrayType); // Init the list of actors as an array
    
    for (int actor_id : Scene::GetAllActorsInScene())
    {
        // Adds the actor to the 'actors' array
        actors.PushBack(Actors::SaveActorToJSON(actor_id, false, allocator), allocator);
        
    }
    
    updated_scene.AddMember("actors", actors, allocator);
    
    EngineUtils::WriteJsonFile(GetScenePath(Scene::GetSceneName()), updated_scene);
}

/**
 * Saves all of the changes made in the editor to the engine directory
 */
void EditorManager::SaveChanges()
{
    // Changes cannot be saved in play mode
    if (play_mode) { return; }
    
    // All changes should be locally saved before being saved to the disk
    UpdateSceneLocal();
    
    std::string save_to_path = "";
#ifndef NDEBUG
    save_to_path = REAL_DIR_PATH;
    save_to_path += "/resources";
#endif
    
    const auto copy_options = std::filesystem::copy_options::recursive
                            | std::filesystem::copy_options::update_existing
    ;

    // Save Scenes
    std::filesystem::copy(FileUtils::GetPath("resources/scenes"), save_to_path + "/scenes", copy_options);
    
    // Save Actor_Templates
    std::filesystem::copy(FileUtils::GetPath("resources/actor_templates"), save_to_path + "/actor_templates", copy_options);
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
    else if (key.is<int>()) { var_name = to_string(key.as<int>()); }
    else if (key.is<float>() || key.is<double>()) { var_name = to_string(key.as<float>()); }
    else { return; }
    
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
        if (ImGui::InputText(const_invisible_id, const_var_value, 100) && (ImGui::IsItemEdited() && ImGui::IsItemDeactivated()))
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
            
            // Add a value to the table
            if (ImGui::BeginMenu("Add Item")) {
                if (ImGui::MenuItem("Int")) {
                    variable_value.add(0);
                }
                if (ImGui::MenuItem("float")) {
                    variable_value.add(0.0f);
                }
                if (ImGui::MenuItem("string")) {
                    variable_value.add("string");
                }
                if (ImGui::MenuItem("bool")) {
                    variable_value.add(false);
                }
                if (ImGui::MenuItem("table")) {
                    variable_value.add(LuaAPI::GetLuaState()->create_table());
                }
                ImGui::EndMenu();
            }

            // Table allows us to cleanly format our variables
            ImGui::BeginTable(const_invisible_id, 3);

            ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, 10.0f); // Default to 10.0f
            ImGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthFixed, 10.0f);
            ImGui::TableSetupColumn("three", ImGuiTableColumnFlags_WidthStretch);

            // Keeps track of the table element we're on while iterating, used to create unique display IDs for the
            // delete button of each element.
            int i = 0;

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

                    i++;
                }
            }
            // Displays all of the items in the table that are created during runtime
            for (auto& item : variable_value)
            {
                sol::lua_value item_key = item.first;
                if (!found_items[item_key].valid())
                {
                    ImGui::TableNextColumn();

                    std::string table_element_id = std::string(const_invisible_id) + std::to_string(i);
                    if (ImGui::Button(std::string("-" + table_element_id).c_str()))
                    {
                        std::cout << table_element_id << std::endl;
                        variable_value[item_key] = sol::lua_nil;
                    }
                
                    // Sets this row of the table to be the variable with the given key
                    VariableView(&variable_value, item_key);

                    i++;
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
    
    // variables to store the window size
    int window_w = 0;
    int window_h = 0;
    SDL_GetWindowSize(RendererData::GetWindow(), &window_w, &window_h);
    
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
            // TOOD: Hot reload all modified scripts
            SaveChanges();
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
        SDL_GetWindowSize(RendererData::GetWindow(), &window_w, &window_h);

        int imgui_window_w = 300.0f;
        int imgui_window_h = window_h;
        ImGui::SetNextWindowSize(ImVec2(imgui_window_w, imgui_window_h));

        // Window Position
        int imgui_window_x = window_w - imgui_window_w;
        int imgui_window_y = ImGui::GetFrameHeightWithSpacing() - 5.0f;
        ImGui::SetNextWindowPos(ImVec2(imgui_window_x, imgui_window_y), ImGuiCond_FirstUseEver);

        // Alows developers to click on specifc actors and components to change values
        ImGui::Begin("Hierarchy View", &hierarchy, window_flags);

        // Add new actors
        if (ImGui::BeginMenu("Add Actor..."))
        {
            std::vector<std::string> list = ListAllTemplateTypes();
            
            std::sort(list.begin(), list.end());
            
            for (std::string name : list)
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    Scene::Instantiate(name);
                }
            }
            ImGui::EndMenu();
        }
        
        // Find the selected actor
        for (int actor_id : Scene::GetAllActorsInScene())
        {
            std::string actor_name = Actors::GetName(actor_id);
            const char* const_name = &actor_name[0];

            bool actor_enabled = Actors::GetActorEnabled(actor_id);

            // If the checkbox is clicked toggle the actor's 'enabled' status
            // The ## hides the id for the item
            std::string checkbox_id = "##" + actor_name + std::to_string(actor_id);
            const char* const_checkbox_id = &checkbox_id[0];
            if (ImGui::Checkbox(const_checkbox_id, &actor_enabled)) { Actors::SetActorEnabled(actor_id, actor_enabled); }

            ImGui::SameLine();

            // If an actor is clicked display its components
            if (ImGui::Button(const_name)) { selected_actor_id = actor_id; }
            
            ImGui::SameLine();
            
            // If an actor is clicked display its components
            Actor temp;
            temp.ID = actor_id;
            std::string delete_id = "-" + checkbox_id;
            const char* const_delete_id = &(delete_id)[0];
            if (ImGui::Button(const_delete_id)) { Scene::Destroy(temp); };
        }
        
        if (selected_actor_id != -1)
        {
            char* const_var_value = &Actors::GetName(selected_actor_id)[0];
            ImGui::Text("Name: ");
            ImGui::SameLine();
            if (ImGui::InputText("##ActorName", const_var_value, 50) && (ImGui::IsItemEdited() && ImGui::IsItemDeactivated()))
            {
                Actors::SetName(selected_actor_id, const_var_value);
            }
        }

        // Display the components of the selected actor
        int number_of_components = Actors::GetNumberOfComponents(selected_actor_id);
        for (int i = 0; i < number_of_components; i++)
        {
            sol::table component = Actors::GetComponentByIndex(selected_actor_id, i);

            if (component.valid())
            {
                std::string component_type = component["type"];
                std::string label = component_type + "##" + std::to_string(selected_actor_id) + std::to_string(i);
                const char* const_type = &component_type[0];

                // If a component is clicked display its properties
                if (ImGui::CollapsingHeader(label.c_str()))
                {
                    sol::table metatable = component[sol::metatable_key];

                    // If component is native, metatable needs to be indexed at __index
                    if (!ComponentManager::IsComponentTypeNative(component_type)) { metatable = metatable["__index"]; }

                    // Table allows us to cleanly format our variables
                    ImGui::BeginTable(const_type, 2);

                    ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, 100.0f); // Default to 100.0f
                    ImGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthStretch);

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
        
        // Menu for adding new components to actors in the scene
        if (selected_actor_id != -1)
        {
            if (ImGui::BeginMenu("Add Component..."))
            {
                std::vector<std::string> list = ListAllComponentTypes();
                std::vector<std::string> native_list = ComponentManager::ListAllNativeComponentTypes();
                list.insert(list.end(), native_list.begin(), native_list.end());
                
                std::sort(list.begin(), list.end());
                
                for (std::string name : list)
                {
                    if (ImGui::MenuItem(name.c_str()))
                    {
                        Actors::AddComponentToActor(selected_actor_id, name);
                    }
                }
                ImGui::EndMenu();
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
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", "Crtl+S")) {
                SaveChanges();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Hierarchy", "Crtl+H", hierarchy)) {
                hierarchy = !hierarchy;
            }
            if (ImGui::MenuItem("File Selector", "Crtl+F", show_file_selector)) {
                show_file_selector = !show_file_selector;
            }
            if (ImGui::MenuItem("Viewport", "Crtl+V", viewport)) {
                viewport = !viewport;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Layout")) {
            if (ImGui::MenuItem("Windowed Full Screen", "F11", windowed_full_screen)) {
                Uint32 window_flags = SDL_GetWindowFlags(RendererData::GetWindow());
                windowed_full_screen = !windowed_full_screen;
                exlusive_full_screen = false;

                UpdateWindowFullScreenState();
            }

            if (ImGui::MenuItem("Exclusive Full Screen", "Crtl+F11", exlusive_full_screen)) {
                Uint32 window_flags = SDL_GetWindowFlags(RendererData::GetWindow());
                exlusive_full_screen = !exlusive_full_screen;
                windowed_full_screen = false;

                UpdateWindowFullScreenState();
            }

            if (ImGui::MenuItem("Save Layout As")) {
                save_layout_as = !save_layout_as;
            }

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Restart the engine for saved layouts to appear");
                ImGui::EndTooltip();
            }

            if (ImGui::MenuItem("Default")) { // need to create a defualt layout
                std::filesystem::path path = docking_layout_file_path.string() + "/" + user_docking_layout_file_name;
                LoadDockingLayout(path.string());
            }

            // create menu items for each .ini file that exists in resources/editor_layouts
            for (const std::string& file : editor_layout_files) {
                if (file != user_docking_layout_file_name && ImGui::MenuItem(file.c_str())) {
                    std::filesystem::path path = docking_layout_file_path.string() + "/" + file;
                    LoadDockingLayout(path.string());
                }
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Theme")) {
            if (ImGui::MenuItem("Default")) {
                ImGui::StyleColorsDark();
            }
            if (ImGui::MenuItem("Visual Studio")) {
                EditorStyle::VisualStudioStyle();
            }
            if (ImGui::MenuItem("Cherry")) {
                EditorStyle::CherryStyle();
            }
            if (ImGui::MenuItem("Soft Cherry")) {
                EditorStyle::SoftCherryStyle();
            }
            if (ImGui::MenuItem("Green Leaf")) {
                EditorStyle::GreenLeafStyle();
            }
            if (ImGui::MenuItem("Gold")) {
                EditorStyle::GoldStyle();
            }
            if (ImGui::MenuItem("Unreal Engine")) {
                EditorStyle::UnrealEngineStyle();
            }
            if (ImGui::MenuItem("Purple Comfy")) {
                EditorStyle::PurpleComfyStyle();
            }
            if (ImGui::MenuItem("Black Devil")) {
                EditorStyle::BlackDevilStyle();
            }
            if (ImGui::MenuItem("Light")) {
                EditorStyle::LightStyle();
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
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S)) && ImGui::GetIO().KeyCtrl) {
        SaveChanges();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F)) && ImGui::GetIO().KeyCtrl) {
        show_file_selector = !show_file_selector;
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_V)) && ImGui::GetIO().KeyCtrl) {
        viewport = !viewport;
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F11)) && !ImGui::GetIO().KeyCtrl) {
        windowed_full_screen = !windowed_full_screen;
        exlusive_full_screen = false;
        UpdateWindowFullScreenState();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F11)) && ImGui::GetIO().KeyCtrl) {
        exlusive_full_screen = !exlusive_full_screen;
        windowed_full_screen = false;
        UpdateWindowFullScreenState();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
        windowed_full_screen = false;
        exlusive_full_screen = false;
        UpdateWindowFullScreenState();
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

/**
* Gets all of the editor layout file names from resources/editor_layouts
*
* @return    a vector containing strings of the file names of the editor layout files
*/
std::vector<std::string> EditorManager::GetEditorLayouts()
{
    std::vector<std::string> iniFiles;
    if (std::filesystem::exists(docking_layout_file_path)) {
        for (const auto& entry : std::filesystem::directory_iterator(docking_layout_file_path)) {
            if (entry.path().extension() == ".ini") {
                std::string file_name = FileUtils::removeExtension(entry.path().filename().string());
                iniFiles.push_back(file_name);
            }
        }
    }

    return iniFiles;
}

/**
* Creates a imgui widgit for selecting files from the resources folder and stores the selected
* file in the selected_file variable in EditorManager.h
*/
void EditorManager::ShowFileSelector() {
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    ImGui::Begin("File Selector");
    
    // Display current path
    ImGui::Text("Current Path: %s", current_path.string().c_str());

    // Button to go to parent directory
    if (std::filesystem::path(current_path).has_parent_path()) {
        if (ImGui::Button("Go to Parent Directory")) {
            current_path = std::filesystem::path(current_path).parent_path();
        }
    }

    // Display list of files and directories
    for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
        if (entry.is_directory()) {
            if (ImGui::Button((entry.path().filename().string() + "/").c_str())) {
                current_path = entry.path();
            }
        }
        else {
            if (ImGui::Selectable(entry.path().filename().string().c_str())) {
                selected_file = entry.path();
            }
        }
    }

    // Calculate the required width for the text box and button
    float textWidth = ImGui::CalcTextSize(selected_file.filename().string().c_str()).x;
    float buttonWidth = ImGui::CalcTextSize("Confirm").x + ImGui::GetStyle().FramePadding.x * 2;
    constexpr float WINDOW_PADDING = 120;
    constexpr float TEXT_BOX_MIN_WIDTH = 200.0f;
    float totalWidth = TEXT_BOX_MIN_WIDTH + buttonWidth + WINDOW_PADDING;

    // Set a maximum width for the text box
    if (textWidth < TEXT_BOX_MIN_WIDTH) {
        ImGui::PushItemWidth(TEXT_BOX_MIN_WIDTH);
    }
    else {
        ImGui::PushItemWidth(textWidth);
    }

    if (ImGui::GetWindowWidth() < totalWidth) {
        ImGui::SetWindowSize(ImVec2(totalWidth, ImGui::GetWindowHeight()));
    }

    ImGui::Text("Selected File:");
    ImGui::SameLine();
    ImGui::InputText("##SelectedFile", (char*)selected_file.filename().string().c_str(), selected_file.filename().string().size() + 1, ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("Confirm") && !selected_file.empty()) {
        show_file_selector = false; // Close the file selector window
    }

    ImGui::End();
}

/**
* Creates the widget for the viewport and handles rendering
*/
void EditorManager::ViewportWidget()
{
    if (viewport) {
        const int DRAG_SENSE = 100;
        ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
        if (ImGui::IsWindowHovered())
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && editor_mode)
            {
                float x = RendererData::GetCameraPosition().x + (-ImGui::GetMouseDragDelta(ImGuiMouseButton_Right).x / DRAG_SENSE);
                float y = RendererData::GetCameraPosition().y + (-ImGui::GetMouseDragDelta(ImGuiMouseButton_Right).y / DRAG_SENSE);
                ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
                RendererData::SetCameraPosition(x, y);
            }
            
            float scroll_wheel = ImGui::GetIO().MouseWheel;
            if (scroll_wheel != 0 && editor_mode)
            {
                float old_zoom = RendererData::GetCameraZoom();
                RendererData::SetCameraZoom(old_zoom + (scroll_wheel / 10));
                std::cout << RendererData::GetCameraZoom() << std::endl;
            }
        }

        RendererData::RenderAndClearAllImageRequests();
        RendererData::RenderAndClearAllTextRequests();
        RendererData::RenderAndClearAllUI();
        RendererData::RenderAndClearAllPixels();
        RendererData::RenderAndClearAllLines();

        //ImageToImGUI();
        //TextToImGUI();
        //UIToImGUI();
        //PixelToImGUI();
        //LineToImGUI();
        ImGui::End();
    }
}

/**
* Updates the current SDL window fullscreen flag to be consistent with the
* windowed_fullscreen and exclusive_fullscreen variables
*/
void EditorManager::UpdateWindowFullScreenState()
{
    if (windowed_full_screen) {
        RendererData::SetWindowFullscreen(SDL_WINDOW_FULLSCREEN_DESKTOP);
        exlusive_full_screen = false;
    }
    else if (exlusive_full_screen) {
        RendererData::SetWindowFullscreen(SDL_WINDOW_FULLSCREEN);
        windowed_full_screen = false;
    }
    else {
        RendererData::SetWindowFullscreen(0);
        exlusive_full_screen = false;
        windowed_full_screen = false;
    }
}
