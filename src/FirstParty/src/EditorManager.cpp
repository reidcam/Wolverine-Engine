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

bool EditorManager::editor_mode = true; // True when the game is paused and edits can be made
bool EditorManager::play_mode = false; // True after the play button is pressed until the stop button is pressed. No edits can be made in this mode.
bool EditorManager::trigger_editor_mode_toggle = false;
int EditorManager::selected_actor_id = -1; // The actor ID of the selected actor in the hierarchy view.

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
    
    // Set up imgui style
    ImGui::StyleColorsDark();
    
    // Set up platfomr/renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(RendererData::GetWindow(), RendererData::GetRenderer());
    ImGui_ImplSDLRenderer2_Init(RendererData::GetRenderer());
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
    
    // Create all of the ImGui windows
    ModeSwitchButtons();
    HierarchyView();
    
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
}

/**
* Cleans up the imgui context when the game is closed
*/
void EditorManager::Cleanup()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

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
    ImGui::Begin("Editor Mode", display_window, flags);
    if (!play_mode)
    {
        if (ImGui::Button("Play"))
        {
            // TOOD: Hot reload all modified scenes and scripts
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
    // Flags
    bool* display_window = NULL;
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    
    int window_w = 0;
    int window_h = 0;
    SDL_GetWindowSize(RendererData::GetWindow(), &window_w, &window_h);
    
    // Window Size
    int imgui_window_w = 300.0f;
    int imgui_window_h = window_h;
    ImGui::SetNextWindowSize(ImVec2(imgui_window_w, imgui_window_h));
    
    // Window Position
    int imgui_window_x = 0.0f;
    int imgui_window_y = 0.0f;
    ImGui::SetNextWindowPos(ImVec2(imgui_window_x, imgui_window_y));
    
    // Alows developers to click on specifc actors and components to change values
    ImGui::Begin("Hierarchy View", display_window, flags);
    
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
    
    int number_of_components = Actors::GetNumberOfComponents(selected_actor_id);
    for (int i = 0; i < number_of_components; i++)
    {
        sol::table component = Actors::GetComponentByIndex(selected_actor_id, i);
        
        if (component.valid())
        {
            std::string component_type = (string)component["type"];
            const char* const_type = &component_type[0];
            
            // If a component is clicked display its properties
            if (ImGui::CollapsingHeader(const_type)) 
            {
                // Table allows us to cleanly format our parameters
                ImGui::BeginTable(const_type, 2);
                for (auto& parameter : component)
                {
                    sol::object key = parameter.first;
                    sol::object value = parameter.second;
                    
                    // Parameter Name
                    std::string parameter_name = key.as<std::string>();
                    const char* const_param_name = &parameter_name[0];
                    
                    // Invisible Parameter Name
                    // ## allows us to have a unique ImGui ID for this item without showing the ID in the UI
                    std::string invisible_id = "##" + key.as<std::string>();
                    const char* const_invisible_id = &invisible_id[0];
                    
                    ImGui::TableNextColumn();
                    ImGui::Text(const_param_name);
                    ImGui::TableNextColumn();
                    
                    // Parameter Value
                    if (value.get_type() == sol::type::string)
                    {
                        std::string parameter_value = value.as<std::string>();
                        const char* const_param_value = &parameter_value[0];
                        ImGui::Text(const_param_value);
                    }
                    if (value.get_type() == sol::type::number)
                    {
                        // TODO: FLOATS
                        int parameter_value = value.as<int>();
                        
                        // Set the value of this parameter if it's changed in the editor
                        if (ImGui::DragInt(const_invisible_id, &parameter_value))
                        {
                            component[const_param_name] = parameter_value;
                        }
                    }
                    if (value.get_type() == sol::type::boolean)
                    {
                        bool parameter_value = value.as<bool>();
                        
                        // Set the value of this parameter if it's changed in the editor
                        if (ImGui::Checkbox(const_invisible_id, &parameter_value))
                        {
                            component[const_param_name] = parameter_value;
                        }
                    }
                    // TODO: TABLES
                    
                    ImGui::TableNextRow();
                }
                ImGui::EndTable();
            }
        }
    }
    
    ImGui::End();
    delete display_window;
}
