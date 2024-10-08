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
    //LoadFontsImGUI(); // NOTE: Must be called before ImGui::NewFrame() and after ImGui::Render()
    ImGui::NewFrame();
    
    ViewportDocking();

    // check to see if any windows should be opened/closed this frame
    CheckEditorShortcuts();

    ImGui::ShowDemoWindow();
    // Create all of the ImGui windows
    MainMenuBar();
    HierarchyView();
    ModeSwitchButtons();

    if (show_file_selector)
        ShowFileSelector();
    
    ViewportWidget();

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
    rapidjson::Document updated_scene(rapidjson::kObjectType); // Init the scene file as an object
    // Create an allocator (required for memory management in RapidJSON)
    rapidjson::Document::AllocatorType& allocator = updated_scene.GetAllocator();
    
    rapidjson::Value actors(rapidjson::kArrayType); // Init the list of actors as an array
    
    for (int actor_id : Scene::GetAllActorsInScene())
    {
        rapidjson::Value actor(rapidjson::kObjectType); // Init the actor as an object
        
        // Adds the contents of the actor to its json object:
        // Get the 'name' value
        rapidjson::Value actor_name;
        std::string name = Actors::GetName(actor_id);
        actor_name.SetString(name.c_str(), allocator);
        
        // Add the 'name' value to the actor
        actor.AddMember("name", actor_name, allocator);
        
        // Get the 'components' value
        rapidjson::Value components(rapidjson::kObjectType);
        
        // Add all of the components to the 'components' value
        int number_of_components = Actors::GetNumberOfComponents(actor_id);
        for (int i = 0; i < number_of_components; i++)
        {
            sol::table component = Actors::GetComponentByIndex(actor_id, i);
            
            if (component.valid())
            {
                rapidjson::Value json_comp(rapidjson::kObjectType); // Init the component as an object
                
                /* 
                 Used to store the types of the keys and values for the variables in this component
                 so that the engine can properly translate the values back from json into lua when the scene is loaded
                 */
                rapidjson::Value key_value_type_pairs(rapidjson::kObjectType);
                
                std::string component_type = component["type"];
                
                sol::table metatable = component[sol::metatable_key];
                
                // If component is not native, metatable needs to be indexed at __index
                if (!ComponentManager::IsComponentTypeNative(component_type)) { metatable = metatable["__index"]; }
                
                // Loop through the varaiables and add them to our json component
                int j = 0;
                for (auto& variable : metatable)
                {
                    std::string var_name = variable.first.as<std::string>();
                    
                    // Skip the variables that exist for engine use: key, actor, type, or any native component values added by Lua
                    if (var_name == "key" || var_name == "actor" ||
                        var_name == "class_cast" || var_name == "REMOVED_FROM_ACTOR" || var_name == "class_check" ||
                        var_name == "__type" || var_name == "__name") { continue; }
                    
                    // Skip functions
                    if (component[variable.first].get_type() == sol::type::function) { continue; }
                    
                    // Skip if the value is the same as it is in the metatable (except for type, which needs to always be displayed in the json)
                    if (component[variable.first] == variable.second && var_name != "type") { continue; }
                    
                    if (variable.second.get_type() == sol::type::table)
                    {
                        // Skip if the value is a table and its empty
                        if (variable.second.as<sol::table>().empty()) { continue; }
                    }
                    
                    rapidjson::Value json_var;
                    // Gets the value of the lua value and stores it in a json value.
                    std::string key_value_pair = "string_";
                    key_value_pair += EngineUtils::LuaObjectToJson(json_var, component[variable.first], allocator);
                    
                    // Adds the key_value_pair to the 'key_value_type_pairs' object
                    rapidjson::Value pair;
                    pair.SetString(key_value_pair.c_str(), allocator);
                    rapidjson::Value keykey; // The key to the key, idk man...
                    keykey.SetString(to_string(j).c_str(), allocator);
                    key_value_type_pairs.AddMember(keykey, pair, allocator);
                    
                    // Gets the name of this variable as a string
                    rapidjson::Value variable_name;
                    variable_name.SetString(var_name.c_str(), allocator);
                    
                    // Adds the variable to the current component
                    json_comp.AddMember(variable_name, json_var, allocator);
                    
                    j++;
                }
                
                // Adds the 'key_value_type_pairs' object to the component
                json_comp.AddMember("__type_pairs", key_value_type_pairs, allocator);
                
                // Add this component to the 'components' list
                rapidjson::Value component_id;
                component_id.SetString(to_string(i).c_str(), allocator);
                components.AddMember(component_id, json_comp, allocator);
            }
        }
        
        // Add the 'components' value to the actor
        actor.AddMember("components", components, allocator);
        
        // Adds the actor to the 'actors' array
        actors.PushBack(actor, allocator);
        
    }
    
    updated_scene.AddMember("actors", actors, allocator);
    
    EngineUtils::WriteJsonFile(GetScenePath(Scene::GetSceneName()), updated_scene);
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
            UpdateSceneLocal();
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
        ImGui::SetNextWindowPos(ImVec2(imgui_window_x, imgui_window_y), ImGuiCond_FirstUseEver);

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
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", "Crtl+S")) {
                
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
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Layout")) {
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
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F)) && ImGui::GetIO().KeyCtrl) {
        show_file_selector = !show_file_selector;
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
    if (FileUtils::DirectoryExists(docking_layout_file_path.string())) {
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
    ImGui::Begin("Viewport");
    ImageToImGUI();
    TextToImGUI();
    UIToImGUI();
    PixelToImGUI();
    ImGui::End();

    ImGui::Begin("test");
    ImGui::Text("Hello World!");
    ImGui::End();


    ImGui::Begin("My Window");

    ImVec2 window_pos = ImGui::GetWindowPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Adjust coordinates by adding window position
    ImVec2 start_pos = ImVec2(10 + window_pos.x, 10 + window_pos.y);
    ImVec2 end_pos = ImVec2(100 + window_pos.x, 100 + window_pos.y);
    draw_list->AddLine(start_pos, end_pos, IM_COL32(255, 0, 0, 255), 2.0f);

    ImGui::End();
}

/**
* Renders all image draw requests in the image_draw_request_queue to a ImGui widget
*/
void EditorManager::ImageToImGUI()
{
    std::stable_sort(RendererData::GetImageDrawRequestQueue()->begin(), RendererData::GetImageDrawRequestQueue()->end(), CompareImageRequests());

    for (auto& request : *RendererData::GetImageDrawRequestQueue()) {
        glm::vec2 final_rendering_position = glm::vec2(request.x, request.y) - RendererData::GetCameraPosition();

        SDL_Texture* tex = GetImage(request.image_name);
        int tex_w = 0; 
        int tex_h = 0;
        SDL_QueryTexture(tex, NULL, NULL, &tex_w, &tex_h);

        // Apply scale
        float x_scale = std::abs(request.scale_x);
        float y_scale = std::abs(request.scale_y);

        ImVec2 tex_size = ImVec2(tex_w * x_scale, tex_h * y_scale);

        // Calculate pivot point
        ImVec2 pivot_point = ImVec2(request.pivot_x * tex_size.x, request.pivot_y * tex_size.y);

        //ImVec2 window_size = ImGui::GetContentRegionAvail();
        ImVec2 window_size = ImGui::GetWindowSize();

        // center the image
        float center_offset = 0.5f;

        // account for current zoom factor
        float zoom = (1.0f / RendererData::GetCameraZoom());

        // Calculate final rendering position
        ImVec2 final_pos = ImVec2(final_rendering_position.x * RendererData::PIXELS_PER_METER + window_size.x * center_offset * zoom - pivot_point.x,
                                  final_rendering_position.y * RendererData::PIXELS_PER_METER + window_size.y * center_offset * zoom - pivot_point.y);

        // Apply tint / alpha to texture
        SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
        SDL_SetTextureAlphaMod(tex, request.a);

        // Render using ImGui
        ImGui::SetCursorPos(final_pos);
        ImGui::Image((void*)tex, tex_size);

        // Remove tint / alpha from texture
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
    }

    RendererData::GetImageDrawRequestQueue()->clear();
}

/**
* Renders all text draw requests in the to to imgui
* 
* TODO: Currently Broken. Text doesn't render
*/
void EditorManager::TextToImGUI()
{
    std::deque<TextRenderRequest>* text_requests = RendererData::GetTextDrawRequestQueue();
    for (auto& request : *text_requests) {
        /*ImFont* font = GetImGuiFont(request.font, request.size);*/
        ImFont* font = nullptr;
        if (font) {
            ImGui::PushFont(font);
        }

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // set position
        ImVec2 position;
        position.x = request.x;
        position.y = request.y;

        // color
        ImU32 col = IM_COL32(request.r, request.g, request.b, request.a);

        draw_list->AddText(position, col, request.text.c_str());

        // return to the previous font, if used
        if (font)
            ImGui::PopFont();
    }

    text_requests->clear();
}

/**
* Renders all ui draw requests in the to to imgui
*/
void EditorManager::UIToImGUI()
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
* 
* TODO: Pixels render on the SDL renderer and not the imgui widget even though there is no call to sdl...
*/
void EditorManager::PixelToImGUI()
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    for (auto& request : *RendererData::GetPixelDrawRequestQueue()) {
        draw_list->AddRectFilled(ImVec2(request.x, request.y), ImVec2(request.x + 10, request.y + 10), IM_COL32(request.r, request.g, request.b, request.a));
    }

    RendererData::GetPixelDrawRequestQueue()->clear();
}

/**
* Loads the fonts that are needed for text requests this frame, if not already loaded
* 
* NOTE: Must be called before ImGui::NewFrame() and after ImGui::Render()
*/
void EditorManager::LoadFontsImGUI()
{
    std::deque<TextRenderRequest>* text_requests = RendererData::GetTextDrawRequestQueue();
    for (auto& request : *text_requests) {
        bool font_found = false;
        
        // check for the font needed and that it is the correct size
        for (auto& font : imgui_fonts[request.font]) {
            if (font->FontSize == request.size) {
                font_found = true;
                break;
            }
        }

        // the font was not found with the size needed, load it
        if (!font_found) {
            const std::string path = "resources/fonts/" + request.font + ".ttf";

            if (FileUtils::DirectoryExists(path))
            {
                ImGuiIO& io = ImGui::GetIO();
                ImFontConfig fontConfig;
                fontConfig.FontDataOwnedByAtlas = false; // Set to false if loading from memory
                ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), request.size, &fontConfig);

                if (font == nullptr) {
                    std::cerr << "Failed to load font!" << std::endl;
                }

                io.Fonts->Build();

                imgui_fonts[request.font].push_back(font); // save the font for later
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
ImFont* EditorManager::GetImGuiFont(const std::string& name, const float size)
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
