//
//	TemplateEditor.h
//	wolverine_engine
//
//	Created by Jacob Robinson on 4/14/2025.
//	Handles the creation and functions of the "Template Editor" window for the game editor
//

#include "TemplateEditor.h"
#include "SceneManager.h"
#include "ComponentManager.h"

/*
 * Saves the changes made to the current template, then update all current instances in the scene
 * NOTE: ONLY Update values if they're the same as the old value
 */
void TemplateEditorWindow::SaveTemplateChanges()
{
    if (!EditorManager::GetEditorMode()) { return; }
    /*
     #1: Create a new instance of the template
     #2: Compare all of the data in the new instance to the changed version, cache the components and variables that are different
     #3: Loop through all existing actors of the template type, update the cached components with the values from the updated version of the template IFF they're different from the instance's values
     #4: Delete the template instance
     */
    
    // Step 1
    int dummy_id = Actors::LoadActorWithJSON(*GetTemplate(selected_template));
    Actors::SetName(dummy_id, selected_template + ":TEMPLATE");
    
    // Step 2
    std::unordered_map<int, std::vector<sol::object>> modified_values; // Map of compoent index to the changed values within those components
    int number_of_components = template_rep->components.size();
    for (int i = 0; i < number_of_components; i++)
    {
        sol::table component_temp = *template_rep->components[i];
        sol::table component_inst = Actors::GetComponentByIndex(dummy_id, i);
        
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
                // Skip userdata (TODO: when custom datatypes are allowed remove this line)
                if (component_temp[variable.first].get_type() == sol::type::userdata) { continue; }

                if (!EngineUtils::LuaEquals(component_inst[variable.first], component_temp[variable.first]))
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
                sol::table component_temp = *template_rep->components[pair.first];
                sol::table component_inst = Actors::GetComponentByIndex(dummy_id, pair.first);
                
                for (auto variable_key : pair.second)
                {
                    if (component[variable_key].valid())
                    {
                        std::string var_name = variable_key.as<std::string>();
                        // If the variable is equal to the old value, set it to the new one
                        if (EngineUtils::LuaEquals(component[variable_key], component_inst[variable_key]))
                        {
                            // TODO: copy by value instead of reference
                            component[variable_key] = component_temp[variable_key];
                            component_inst[variable_key] = component_temp[variable_key];
                        }
                    }
                }
            }
        }
    }
    
    // Update the template
    EditorManager::CreateNewTemplate(dummy_id, selected_template);
    LoadTemplates();
    
    // #4
    Actors::PrepareActorForDestruction(dummy_id);
    Actors::DestroyActor(dummy_id);
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
    // Delete the old template rep if it exists
    if (template_rep != nullptr)
    {
        template_rep = nullptr;
    }
    
    selected_template = to_edit;
    if (to_edit != "")
    {
        // Gets the template reference
        template_rep = GetReferenceTemplate(to_edit);
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
        
        if (template_rep != nullptr) // Don't display editor if template is invalid
        {
            for (auto component : template_rep->components)
            {
                std::string component_type = (*component)["type"].get<std::string>();
                std::shared_ptr<sol::table> og_comp;
                if (ComponentManager::IsComponentTypeNative(component_type))
                {
                    og_comp  = std::make_shared<sol::table>(ComponentManager::NewNativeComponent(component_type));
                }
                else
                {
                    og_comp = GetComponentType(component_type);
                }
                EditorManager::DisplayComponent(*component, og_comp);
            }
        }
        
        ImGui::End();
    }
}
