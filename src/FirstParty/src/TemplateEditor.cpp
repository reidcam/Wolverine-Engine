//
//	TemplateEditor.h
//	wolverine_engine
//
//	Created by Jacob Robinson on 4/14/2025.
//	Handles the creation and functions of the "Template Editor" window for the game editor
//

#include "TemplateEditor.h"
#include "SceneManager.h"

/*
 * Saves the changes made to the current template, then update all current instances in the scene
 * NOTE: ONLY Update values if they're the same as the old value
 */
void TemplateEditorWindow::SaveTemplateChanges()
{
    
}

/*
 * Toggle the window being displayed
 */
void TemplateEditorWindow::ToggleWindow()
{

}

/*
 * Save the changes to the current template, then change the template being edited
 * @param   to_edit the name of the new template to be edited
 */
void TemplateEditorWindow::ChangeTemplate(std::string to_edit)
{
    // If there is a previous template, save its changes before moving on
    if (selected_template != "" && dummy_id != -1)
    {
        SaveTemplateChanges();
        Actors::PrepareActorForDestruction(dummy_id);
        Actors::DestroyActor(dummy_id);
    }
    
    selected_template = to_edit;
    if (to_edit != "")
    {
        // Creates the actor
        dummy_id = Actors::LoadActorWithJSON(*GetTemplate(to_edit));
        Actors::SetName(dummy_id, to_edit + ":TEMPLATE");
    }
    else // Basically set the window to edit nothing
    {
        dummy_id = -1;
    }
}

/*
 * Display the template editor window
 */
void TemplateEditorWindow::TemplateEditor()
{
    if (show_template_editor)
    {
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("Template Editor");
        if (selected_template != "") // Don't display editor if template is invalid
        {
            if (dummy_id != -1) // don't display actor if ID is invalid
            {
                EditorManager::DisplayActor(dummy_id);
            }
            
        }
        
        ImGui::End();
    }
}
