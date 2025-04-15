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
    /*
     #1: Create a new instance of the template
     #2: Compare all of the data in the new instance to the changed version, cache the components and variables that are different
     #3: Loop through all existing actors of the template type, update the cached components with the values from the updated version of the template
     */
    
    // Step 1
    int new_dummy_id = Actors::LoadActorWithJSON(*GetTemplate(selected_template));
    Actors::SetName(new_dummy_id, selected_template + ":TEMPLATE_TEMP");
    
    // Step 2
    std::unordered_map<int, std::vector<sol::object>> modified_values; // Map of compoent index to the changed values within those components
    int number_of_components = Actors::GetNumberOfComponents(dummy_id);
    for (int i = 0; i < number_of_components; i++)
    {
        sol::table component_temp = Actors::GetComponentByIndex(dummy_id, i);
        sol::table component_inst = Actors::GetComponentByIndex(new_dummy_id, i);
        
        if (component_temp.valid() && component_inst.valid())
        {
            std::string component_type = component_temp["type"];
            
            sol::table metatable = component_temp[sol::metatable_key];
            
            // If component is not native, metatable needs to be indexed at __index
            if (!ComponentManager::IsComponentTypeNative(component_type)) { metatable = metatable["__index"]; }
            
            // Loop through the varaiables and compare the two components
            for (auto& variable : metatable)
            {
                std::string var_name = variable.first.as<std::string>();
                
                // Skip the variables that exist for engine use: key, actor, type, or any native component values added by Lua
                if (var_name == "key" || var_name == "actor" ||
                    var_name == "class_cast" || var_name == "REMOVED_FROM_ACTOR" || var_name == "class_check" ||
                    var_name == "__type" || var_name == "__name") { continue; }
                
                // Skip functions
                if (component_temp[variable.first].get_type() == sol::type::function) { continue; }
                
                if (component_inst[variable.first].get<sol::object>() != component_temp[variable.first].get<sol::object>())
                {
                    modified_values[i].push_back(variable.first);
                }
            }
        }
    }
    
    // #3
    for (int actor : Scene::GetAllActorsInScene())
    {
        if (Actors::GetTemplateName(actor) == selected_template)
        {
            for (auto pair : modified_values)
            {
                sol::table component =  Actors::GetComponentByIndex(actor, pair.first);
                sol::table dummy_component = Actors::GetComponentByIndex(dummy_id, pair.first);
                
                for (auto variable_key : pair.second)
                {
//                    component[variable_key] = dummy_component[variable_key];
                }
            }
        }
    }
    
    // Update the template
    EditorManager::CreateNewTemplate(dummy_id, selected_template);
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
        
        // Add new actors
        if (ImGui::BeginMenu("Change Template"))
        {
            std::vector<std::string> list = ListAllTemplateTypes();
            
            std::sort(list.begin(), list.end());
            
            for (std::string name : list)
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    ChangeTemplate(name);
                }
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::Button("SAVE"))
        {
            SaveTemplateChanges();
        }
        
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
