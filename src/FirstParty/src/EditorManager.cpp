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

bool EditorManager::editor_mode = true;
bool EditorManager::trigger_editor_mode_toggle = false;

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
    
    bool t = true;
    ImGui::ShowDemoWindow(&t);
    
    ImGui::Begin("Editor Mode");
    if (ImGui::Button("Play"))
    {
        // TOOD: Hot reload all modified scenes and scripts
        editor_mode = false;
        PhysicsWorld::ResetWorld();
        Scene::ResetManager();
    }
    if (ImGui::Button("Pause")) { trigger_editor_mode_toggle = true; }
    if (ImGui::Button("Reset"))
    {
        // TOOD: Hot reload all modified scenes and scripts
        editor_mode = true;
        PhysicsWorld::ResetWorld();
        Scene::ResetManager();
    }
    ImGui::End();
    
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
