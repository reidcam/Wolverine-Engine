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

#include "Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

class EditorManager
{
private:
    static bool editor_mode;
public:
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
    * Cleans up the imgui context when the game is closed
    */
    static void Cleanup();
    
    /**
    * If editor mode is on, turn it off, if it is off, turn it on
    */
    static void ToggleEditorMode();
    
    /**
    * Returns the editor mode
    */
    static bool GetEditorMode();
};

#endif /* EditorManager.h */
