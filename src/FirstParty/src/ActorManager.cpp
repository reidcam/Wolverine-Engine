//
//  ActorManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages actors
//

#include <stdio.h>

#include "ActorManager.h"
#include "LuaAPI.h"

int Actors::num_total_actors = 0; // The total number of actors created during runtime
int Actors::num_loaded_actors = 0; // The number of actors currently loaded in the game

std::unordered_map<int, int> Actors::id_to_index; // map of an actors id to its current index in the vectors

// The attributes of all the loaded actors
std::vector<std::string> Actors::names;
std::vector<std::string> Actors::templates;
std::vector<int> Actors::IDs;
std::vector<bool> Actors::actor_enabled;

//-------------------------------------------------------
// Lifecycle Functions

/**
 * Cleans all of the dead actors out from the vectors
*/
void Actors::Cleanup()
{
    // Erase the dead actors from every vector
    int i = 0;
    while (i < num_loaded_actors)
    {
        if (IDs[i] == -1)
        {
            // Erase from the vectors
            IDs.erase(IDs.begin() + i);
            names.erase(names.begin() + i);
            templates.erase(templates.begin() + i);
            actor_enabled.erase(actor_enabled.begin() + i);
            components.erase(components.begin() + i);
        }
        else
        {
            i++;
        }
    }
    
    // Update the id_to_index map with new indexes for living ids
    for (int i = 0; i < IDs.size(); i++) 
    {
        id_to_index[IDs[i]] = i;
    }
}

/**
 * Processes all components added to all actors on the previous frame
*/
void Actors::ProcessAddedComponents()
{
    // Stores the components that didn't get processed this frame, so we can re-add
    // them to "components_to_init" when we've finished processing the others
    std::vector<std::shared_ptr<sol::table>> not_processed;
    
    for (auto& component : components_to_init)
    {
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        
        // If this component has been removed, skip it
        if ((*component)["REMOVED_FROM_ACTOR"] == true || (*component)["actor"] == sol::lua_nil)
        {
            continue;
        }
        
        // Skip this component if the actor or component aren't enabled
        if (!actor_enabled[actor_index] || (*component)["enabled"] == false)
        {
            not_processed.push_back(component);
            continue;
        }
        
        // Add to the appropriate lifecycle functions list so that they will start being called by the engine.
        sol::function OnUpdate = (*component)["OnUpdate"];
        if (OnUpdate.valid()) { components_to_update.push_back(component); }
        sol::function OnLateUpdate = (*component)["OnLateUpdate"];
        if (OnLateUpdate.valid()) { components_to_update_late.push_back(component); }
        
        // Call "OnStart" if it exists for this component
        try
        {
            // OnStart is called for each component the frame they are loaded into the game
            sol::function OnStart = (*component)["OnStart"];
            if (OnStart.valid())
            {
                // NOTE TO SELF: ENGINE DOES NOT HANDLE spawning components inside of "OnStart" well
                OnStart(*component);
            }
        }
        catch(const std::exception& e)
        {
            std::string errorMessage = e.what();
#ifdef _WIN32
            std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
#endif
            std::cout << "\033[31m" << names[actor_index] << " : " << errorMessage << "\033[0m" << std::endl;
        }
    }
    
    // Remove all the processed components from the list
    components_to_init.clear();
    components_to_init = not_processed;
}

/**
 * Calls "OnUpdate" for every component that has it
*/
void Actors::Update()
{
    std::vector<std::shared_ptr<sol::table>> living_components; // The "Update" list without any of the dead components
    
    int i = 0;
    for (auto& component : components_to_update)
    {
        // If this component is dead, skip it
        if ((*component)["REMOVED_FROM_ACTOR"] == true || (*component)["actor"] == sol::lua_nil)
        {
            continue;
        }
        
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        
        // The component is alive! add it to living components
        living_components.push_back(component);
    
        // Skip this component if the actor or component aren't enabled
        if (!actor_enabled[actor_index] || (*component)["enabled"] == false)
        {
            continue;
        }
        
        // Call "OnUpdate"
        try
        {
            sol::function OnUpdate = (*component)["OnUpdate"];
            if (OnUpdate.valid())
            {
                OnUpdate(*component);
            }
        }
        catch(const std::exception& e)
        {
            std::string errorMessage = e.what();
#ifdef _WIN32
            std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
#endif
            std::cout << "\033[31m" << names[actor_index] << " : " << errorMessage << "\033[0m" << std::endl;
        }
        i++;
    }
    
    components_to_update.clear();
    components_to_update = living_components;
}

/**
 * Calls "OnLateUpdate" for every component that has it
*/
void Actors::LateUpdate()
{
    std::vector<std::shared_ptr<sol::table>> living_components; // The "LateUpdate" list without any of the dead components
    
    for (auto& component : components_to_update_late)
    {
        // If this component is dead, skip it
        if ((*component)["REMOVED_FROM_ACTOR"] == true || (*component)["actor"] == sol::lua_nil)
        {
            continue;
        }
        
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        
        // The component is alive! add it to living components
        living_components.push_back(component);
        
        // Skip this component if the actor or component aren't enabled
        if (!actor_enabled[actor_index] || (*component)["enabled"] == false)
        {
            continue;
        }
        
        // Call "OnLateUpdate"
        try
        {
            sol::function OnLateUpdate = (*component)["OnLateUpdate"];
            if (OnLateUpdate.valid())
            {
                OnLateUpdate(*component);
            }
        }
        catch(const std::exception& e)
        {
            std::string errorMessage = e.what();
#ifdef _WIN32
            std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
#endif
            std::cout << "\033[31m" << names[actor_index] << " : " << errorMessage << "\033[0m" << std::endl;
        }
    }
    
    components_to_update_late.clear();
    components_to_update_late = living_components;
}

/**
 * Processes all components removed from actors on this frame
*/
void Actors::ProcessRemovedComponents()
{
    while (components_to_delete.size() > 0)
    {
        auto& component = components_to_delete.front();
        
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        bool enabled_actor = actor_enabled[actor_index];
        bool enabled_component = (*component)["enabled"];
        
        // Skip caling "OnDestroy" on this component if the actor or component aren't enabled
        if (!enabled_actor || !enabled_component)
        {
            continue;
        }
        
        // Call "OnDestroy" if this component has it.
        try
        {
            sol::function OnDestroy = (*component)["OnDestroy"];
            if (OnDestroy.valid())
            {
                OnDestroy(*component);
            }
        }
        catch(const std::exception& e)
        {
            std::string errorMessage = e.what();
#ifdef _WIN32
            std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
#endif
            std::cout << "\033[31m" << names[actor_index] << " : " << errorMessage << "\033[0m" << std::endl;
        }
        
        // Deletes the component
        LuaAPI::DeleteLuaTable(component);
        components_to_delete.pop();
    }
}

//-------------------------------------------------------
// Getters/Setters

/**
 * Returns this actors name
 *
 * @param   actor_id    the id of the actor that this function is acting on
 * @returns             the name of the given actor
*/
std::string Actors::GetName(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return "";}
    
    return names[actor_index];
}

/**
 * Sets this actors name
 *
 * @param   actor_id    the id of the actor that this function is acting on
 * @param   new_name    the new name for the given actor
*/
void Actors::SetName(int actor_id, std::string new_name)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return;}
    
    names[actor_index] = new_name;
}

/**
 * Returns this actors ID
 *
 * @param   actor_id    the id of the actor that this function is acting on
 * @returns             the ID of the given actor
*/
int Actors::GetID(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return -1;}
    
    return IDs[actor_index];
}

/**
* Gets wether or not an actor is enabled
*
* @param     actor_id    the id of the actor that this function is acting on
* @return    a bool for whether or not the actor is enabled
*/
bool Actors::GetActorEnabled(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) { return false; }
    
    return actor_enabled[actor_index];
}

/**
* Sets wether or not an actor is enabled
*
* @param     actor_id    the id of the actor that this function is acting on
* @param    is_enabled  the new enabled status of the actor
*/
void Actors::SetActorEnabled(int actor_id, bool is_enabled)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) { return; }
    
    actor_enabled[actor_index] = is_enabled;
    
    // TODO: Enable or disable the Rigidbody Component to stop the collider from moving even while disabled
    // Same for when the rigidbody component is manually disabled.
}

/**
 * Returns this actors template name
 *
 * @param   actor_id    the id of the actor that this function is acting on
 * @returns             the name of the template of the given actor, bank string if it does not have a template
*/
std::string Actors::GetTemplateName(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return "";}
    
    return templates[actor_index];
}

//-------------------------------------------------------
// Misc.

/**
 * Loads the data from JSON into the actor database to create a new actor
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 * In order to create a new actor please use the "'instantiate' function instead
 *
 * @param   actor_data    the JSON that will be processed into a new actor
 * @return                returns the id of the newly created actor
*/
int Actors::LoadActorWithJSON(const rapidjson::Value& actor_data)
{
    // Gives actor their ID
    IDs.push_back(num_total_actors);
    int index = (int)IDs.size() - 1;
    id_to_index[num_total_actors] = index;
    
    // Assigns the values to the new actor
    if (actor_data.HasMember("name"))
    {
        names.push_back(actor_data["name"].GetString());
    }
    else
    {
        // Gives this actor a default name if none is specified for it
        names.push_back("ActorName");
    }
    
    if (actor_data.HasMember("template"))
    {
        templates.push_back(actor_data["template"].GetString());
    }
    else
    {
        // Gives this actor a blank template if none is specified for it
        templates.push_back("");
    }
    
    if (actor_data.HasMember("enabled"))
    {
        actor_enabled.push_back(actor_data["enabled"].GetBool());
    }
    else
    {
        // Sets the actor to be enabled by default
        actor_enabled.push_back(true);
    }
    
    //-------------------------------------------------------
    // Components
    
    // Stores all of the created components
    std::vector<std::shared_ptr<sol::table>> new_components_list;
    if (actor_data.HasMember("components"))
    {
        const rapidjson::Value& actor_components = actor_data["components"];
        
        // Iterate over each component
        for (rapidjson::Value::ConstMemberIterator itr = actor_components.MemberBegin(); itr != actor_components.MemberEnd(); itr++)
        {
            // Creates and gets a reference to a new table on the Lua stack
            sol::table new_component = LuaAPI::GetLuaState()->create_table();
            
            // Gets the key_value type pairs for this component
            const rapidjson::Value& holder = itr->value;
            const rapidjson::Value& key_value_type_pairs = holder["__type_pairs"];
            
            // The key of this component
            std::string key = itr->name.GetString();
            
            // Establishes inheritance between the new component and its type if specified
            if (itr->value.HasMember("type"))
            {
                std::string type = itr->value["type"].GetString();
                
                // Establishes our new component according to its type
                if (ComponentManager::IsComponentTypeNative(type))
                {
                    new_component = ComponentManager::NewNativeComponent(type);
                }
                else
                {
                    ComponentManager::EstablishInheritance(new_component, *GetComponentType(type));
                }
                
                // Gives the component its key
                new_component["key"] = key;
                // Sets the component to be enabled by default
                new_component["enabled"] = true;
            }
            // If the type for this component is not specified anywhere, throw an error
            else
            {
                std::cout << "error: component type unspecified for " << key << " on " << names[names.size() - 1];
                exit(0);
            }
            
            //-------------------------------------------------------
            // Preform required overrides on component properties
            // Sets component properties to specified values
            const rapidjson::Value& component_properties = itr->value;
            int i = 0;
            for (rapidjson::Value::ConstMemberIterator itr2 = component_properties.MemberBegin(); itr2 != component_properties.MemberEnd(); itr2++)
            {
                std::string property_name = itr2->name.GetString();
                
                // Skip this loop if we're looking at the __type_pairs object since it doesn't need to be translated into Lua.
                if (property_name == "__type_pairs") { continue; }
                
                std::string pair = key_value_type_pairs.FindMember(to_string(i).c_str())->value.GetString();
                std::size_t splitter = pair.find('_');
                
                // Error output if the key_value pair was formatted incorrectly
                if (splitter >= pair.size()) { std::cout << "error: Incorrect key_value pair formatting in json"; }
                else
                {
                    std::string property_type = pair.substr(splitter + 1);
                    sol::lua_value property = "";
                    
                    EngineUtils::JsonToLuaObject(property, itr2->value, property_type);
                    
                    if (property.value().valid())
                    {
                        new_component[property_name] = property;
                    }
                }
                i++;
            }

            //-------------------------------------------------------
            // Injects the new component with a reference to its actor
            Actor* _a = new Actor();
            _a->ID = num_total_actors;
            new_component["actor"] = _a;
            
            // Add the new component to the "components_to_init" and "components" vectors
            std::shared_ptr<sol::table> ptr = std::make_shared<sol::table>(new_component);
            components_to_init.push_back(ptr);
            new_components_list.push_back(ptr);
        }
    }
    components.push_back(new_components_list);
    
    // Update the number of loaded actors.
    num_loaded_actors++;
    num_total_actors++;
    
    return num_total_actors - 1;
}

/**
 * Converts an actor into a json file so that the actor can be saved/instantiated easily
 *
 * @param   actor_id             the ID of the actor to be templatized
 * @param   as_template      true if this actor should be saved in the form of a template
 * @param   document_allocator      the allocator for the document this actor is going into
 * @return                returns json representing the actor as a template
*/
rapidjson::Value Actors::SaveActorToJSON(int actor_id, bool as_template, rapidjson::Document::AllocatorType& document_allocator)
{
    rapidjson::Value actor(rapidjson::kObjectType); // Init the actor as an object
    
    // Adds the contents of the actor to its json object:
    // Get the 'name' value
    rapidjson::Value actor_name;
    std::string name = Actors::GetName(actor_id);
    actor_name.SetString(name.c_str(), document_allocator);
    
    // Add the 'name' value to the actor
    actor.AddMember("name", actor_name, document_allocator);
    
    // Get the template of the actor
    std::string template_name = Actors::GetTemplateName(actor_id);
    if (!as_template) // This engine does not support nested templates (yet... :( )
    {
        if (template_name != "")
        {
            rapidjson::Value actor_template_name;
            actor_template_name.SetString(template_name.c_str(), document_allocator);
            actor.AddMember("template", actor_template_name, document_allocator);
        }
    }
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
                
                // Skip if the value is the same as it is in the metatable (except for type, which needs to always be displayed in the json), UNLESS THIS IS A TEMPLATE
                if (!as_template)
                {
                    if (component[variable.first] == variable.second && var_name != "type") { continue; }
                    
                    // If the component is native, default values need to be checked in a special way
                    if (ComponentManager::IsComponentTypeNative(component_type) && var_name != "type")
                    {
                        if (ComponentManager::IsDefaultValue(component_type, variable.first, component[variable.first]))
                        {
                            continue;
                        }
                    }
                }
                
                if (variable.second.get_type() == sol::type::table)
                {
                    // Skip if the value is a table and its empty
                    if (variable.second.as<sol::table>().empty()) { continue; }
                }
                
                rapidjson::Value json_var;
                // Gets the value of the lua value and stores it in a json value.
                std::string key_value_pair = "string_";
                key_value_pair += EngineUtils::LuaObjectToJson(json_var, component[variable.first], document_allocator);
                
                // Adds the key_value_pair to the 'key_value_type_pairs' object
                rapidjson::Value pair;
                pair.SetString(key_value_pair.c_str(), document_allocator);
                rapidjson::Value keykey; // The key to the key, idk man...
                keykey.SetString(to_string(j).c_str(), document_allocator);
                key_value_type_pairs.AddMember(keykey, pair, document_allocator);
                
                // Gets the name of this variable as a string
                rapidjson::Value variable_name;
                variable_name.SetString(var_name.c_str(), document_allocator);
                
                // Adds the variable to the current component
                json_comp.AddMember(variable_name, json_var, document_allocator);
                
                j++;
            }
            
            if (template_name == "")
            {
                // Adds the 'key_value_type_pairs' object to the component
                json_comp.AddMember("__type_pairs", key_value_type_pairs, document_allocator);
            }
            
            if (!components.IsObject())
            {
                components.SetObject();
            }
            // Add this component to the 'components' list
            rapidjson::Value component_id;
            component_id.SetString(to_string(i).c_str(), document_allocator);
            components.AddMember(component_id, json_comp, document_allocator);
        }
    }
    // Add the 'components' value to the actor
    actor.AddMember("components", components, document_allocator);
    
    return actor;
}

/**
 * Prepares an actor for destruction later this frame
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 * In order to destroy an actor please use the "'destroy' function instead. This ensures that actors are properly prepared for destruction.
 *
 * @param   actor_id    the id of the actor that this function is acting on
*/
void Actors::PrepareActorForDestruction(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return;}
    
    // Queues all of the components on the given actor for deletion
    std::vector<std::shared_ptr<sol::table>> components_to_remove = components[actor_index];
    for (auto& component : components_to_remove)
    {
        RemoveComponentFromActor(actor_id, *component);
    }
}

/**
 * Destroys all actors queued up for destruction
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 * In order to destroy an actor please use the "'destroy' function instead. This ensures that actors are properly prepared for destruction.
 *
 * @param   actor_id    the id of the actor that this function is acting on
*/
void Actors::DestroyActor(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return;}
    
    IDs[actor_index] = -1;
    actor_enabled[actor_index] = false;
    id_to_index.erase(actor_id);
    num_loaded_actors--;
}

/**
 * Gets the vector index of the actor with the given ID
 *
 * @param   actor_id    the id of the actor that this function is acting on
*/
int Actors::GetIndex(int actor_id)
{
    // Check if its an actor that exists
    if (id_to_index.find(actor_id) != id_to_index.end() && id_to_index[actor_id] != -1)
    {
        return id_to_index[actor_id];
    }
    
    //std::cout << "error: attempt to access a nonexistant actor with ID: " << actor_id << std::endl;
    return -1;
}

//-------------------------------------------------------
// Components.

/**
 * Removes a component from an actor and marks it for deletion
 *
 * @param   actor_id     the id of the actor that this function is acting on
 * @param   component    the component to be removed
*/
void Actors::RemoveComponentFromActor(int actor_id, sol::table component)
{
    if (!component.valid())
    {
        return;
    }
    
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return;}
    
    // Removes this component from the actor
    int component_index = -1;
    for (int i = 0; i < components[actor_index].size(); i++) // Find the index of this component
    {
        if ((*components[actor_index][i]).pointer() == component.pointer())
        {
            (component)["REMOVED_FROM_ACTOR"] = true;
            components_to_delete.push(std::make_shared<sol::table>(component));
            component_index = i;
            break;
        }
    }
    if (component_index != -1) {
        components[actor_index].erase(components[actor_index].begin() + component_index);
    } // remove the component from the actor
}

/**
 * Adds a new component to an actor
 *
 * @param   actor_id     the id of the actor that this function is acting on
 * @param   component_type    the type of component to be added
*/
void Actors::AddComponentToActor(int actor_id, std::string component_type)
{
    if (Actors::GetTemplateName(actor_id) != "")
    {
        std::cout << "TODO: Allow adding components to template instances!" << std::endl;
        return;
    }
    // Creates and gets a reference to a new table on the Lua stack
    sol::table new_component = LuaAPI::GetLuaState()->create_table();

    // The key of this component
    std::string key = std::to_string(components[Actors::GetIndex(actor_id)].size());

    // Establishes inheritance between the new component and its type if specified
    if (!component_type.empty())
    {
        std::string type = component_type;

        // Establishes our new component according to its type
        if (ComponentManager::IsComponentTypeNative(type))
        {
            new_component = ComponentManager::NewNativeComponent(type);
        }
        else
        {
            ComponentManager::EstablishInheritance(new_component, *GetComponentType(type));
        }

        // Gives the component its key
        new_component["key"] = key;
        // Sets the component to be enabled by default
        new_component["enabled"] = true;
    }
    // If the type for this component is not specified anywhere, throw an error
    else
    {
        std::cout << "error: component type unspecified for " << key << " on " << names[Actors::GetIndex(actor_id)];
        exit(0);
    }

    //-------------------------------------------------------
    // Injects the new component with a reference to its actor
    Actor* _a = new Actor();
    _a->ID = actor_id;
    new_component["actor"] = _a;

    // Add the new component to the "components_to_init" and "components" vectors
    std::shared_ptr<sol::table> ptr = std::make_shared<sol::table>(new_component);
    components_to_init.push_back(ptr);
    components[Actors::GetIndex(actor_id)].push_back(ptr);
}

/**
 * Gets the first component on the given actor with the given type if it exists.
 *
 * @param   actor_id    the id of the actor that this function is acting on
 * @param   type        the type of component we're searching for
 * @return              the first component on the given actor with the given type, if none are found returns null
*/
sol::table Actors::GetComponentByType(int actor_id, std::string type)
{
    sol::table null; // An empty table, to be returned if the component(s) cannot be found
    
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return null;}
    
    for (int i = 0; i < components[actor_index].size(); i++)
    {
        if ((*components[actor_index][i])["type"] == type)
        {
            return *components[actor_index][i];
        }
    }
    
    // Return null if the component cannot be found.
    return null;
}

/**
* Gets the number of components that an actor has
*
* @param     actor_id    the id of the actor that this function is acting on
* @return    the number of components that the actor currently has
*/
int Actors::GetNumberOfComponents(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) { return 0; }
    
    return components[actor_index].size();
}

/**
* Gets a component by index
*
* @param     actor_id           the id of the actor that this function is acting on
* @param     component_index    the index of the component
* @return    a sol table containing the component
*/
sol::table Actors::GetComponentByIndex(int actor_id, int component_index)
{
    sol::table null; // An empty table, to be returned if the component(s) cannot be found
    
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) { return null; }
    if (component_index < 0 || component_index >= GetNumberOfComponents(actor_id)) { return null; }
    
    return *components[actor_index][component_index];
}

/**
 * Gets all of the components on the given actor with the given type if they exist.
 *
 * @param   actor_id    the id of the actor that this function is acting on
 * @param   type        the type of component we're searching for
 * @return              a list of all the components with the given type, if none are found returns null
*/
sol::table Actors::GetComponentsByType(int actor_id, std::string type)
{
    sol::table null; // An empty table, to be returned if the component(s) cannot be found
    
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return null;}
    
    sol::table components_of_type = LuaAPI::GetLuaState()->create_table();
    components_of_type[0] = null;
    int index = 1;
    
    for (int i = 0; i < components[actor_index].size(); i++)
    {
        if ((*components[actor_index][i])["type"] == type)
        {
            components_of_type[index] = *components[actor_index][i];
        }
    }
    
    return components_of_type;
}

/**
 * Gets the component on the given actor with the given key if it exists.
 *
 * @param   actor_id    the id of the actor that this function is acting on
 * @param   key         the key of the component we're searching for
 * @return              the component on the given actor with the given key, if none are found returns null
*/
sol::table Actors::GetComponentByKey(int actor_id, std::string key)
{
    sol::table null; // An empty table, to be returned if the component(s) cannot be found
    
    int actor_index = GetIndex(actor_id);
    if (actor_index == -1) {return null;}
    
    for (int i = 0; i < components[actor_index].size(); i++)
    {
        if ((*components[actor_index][i])["key"] == key)
        {
            return *components[actor_index][i];
        }
    }
    
    // Return null if the component cannot be found.
    return null;
}

//-------------------------------------------------------
// Editor Tools

/**
 * Clears all of the components and actors from this manager
 * Used to do a hard reset of invincible actors and components before loading a new scene
 * NOTE: DO NOT EXPOSE TO LUA! This function is primarily meant to reset the game for the editor
*/
void Actors::ResetManager()
{
    num_total_actors = 0;
    num_loaded_actors = 0;
    
    id_to_index.clear();

    names.clear();
    templates.clear();
    IDs.clear();
    actor_enabled.clear();
    
    components.clear();
    components_to_init.clear();
    components_to_delete = {};
    components_to_update.clear();
    components_to_update_late.clear();
}

/**
 * Loops through all the components and ONLY runs onupdate if its type is needed for the editor.
 * This is primarily used to trigger SpriteRenderers and other visual components for the EDITOR in editor mode.
 *
 * Not very DRY I know...
 *
 * @param    editor_components   a list of all the components that are needed for editor mode to function
 */
void Actors::EditorUpdateComponents(std::unordered_set<std::string> editor_components)
{
    std::vector<std::shared_ptr<sol::table>> living_components; // The "Update" list without any of the dead components
    
    for (auto& component : components_to_update)
    {
        // If this component is dead, skip it
        if ((*component)["REMOVED_FROM_ACTOR"] == true)
        {
            continue;
        }
        
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        
        // The component is alive! add it to living components
        living_components.push_back(component);
        
        // Skip this component if it isn't needed in editor mode
        if (editor_components.find((*component)["type"]) == editor_components.end()) { continue; }
    
        // Skip this component if the actor or component aren't enabled
        if (!actor_enabled[actor_index] || (*component)["enabled"] == false)
        {
            continue;
        }
        
        // Call "OnUpdate"
        try
        {
            sol::function OnUpdate = (*component)["OnUpdate"];
            if (OnUpdate.valid())
            {
                OnUpdate(*component);
            }
        }
        catch(const std::exception& e)
        {
            std::string errorMessage = e.what();
#ifdef _WIN32
            std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
#endif
            std::cout << "\033[31m" << names[actor_index] << " : " << errorMessage << "\033[0m" << std::endl;
        }
    }
    
    components_to_update.clear();
    components_to_update = living_components;
}

/**
 * Loops through all the components and ONLY runs onstart if its type is needed for the editor.
 * This is primarily used to prepare SpriteRenderers and other visual components for the EDITOR in editor mode.
 *
 * Not very DRY I know...
 *
 * @param    editor_components   a list of all the components that are needed for editor mode to function
 */
void Actors::EditorStartComponents(std::unordered_set<std::string> editor_components)
{
    // Stores the components that didn't get processed this frame, so we can re-add
    // them to "components_to_init" when we've finished processing the others
    std::vector<std::shared_ptr<sol::table>> not_processed;
    
    for (auto& component : components_to_init)
    {
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        
        // If this component has been removed, skip it
        if ((*component)["REMOVED_FROM_ACTOR"] == true)
        {
            continue;
        }
        
        // Skip this component if the actor or component aren't enabled
        if (!actor_enabled[actor_index] || (*component)["enabled"] == false)
        {
            not_processed.push_back(component);
            continue;
        }

        // Skip this component if it isn't needed in editor mode
        if (editor_components.find((*component)["type"]) == editor_components.end())
        {
            not_processed.push_back(component);
            continue;
        }
        
        // Add to the appropriate lifecycle functions list so that they will start being called by the engine.
        sol::function OnUpdate = (*component)["OnUpdate"];
        if (OnUpdate.valid()) { components_to_update.push_back(component); }
        sol::function OnLateUpdate = (*component)["OnLateUpdate"];
        if (OnLateUpdate.valid()) { components_to_update_late.push_back(component); }
        
        // Call "OnStart" if it exists for this component
        try
        {
            // OnStart is called for each component the frame they are loaded into the game
            sol::function OnStart = (*component)["OnStart"];
            if (OnStart.valid())
            {
                OnStart(*component);
            }
        }
        catch(const std::exception& e)
        {
            std::string errorMessage = e.what();
#ifdef _WIN32
            std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
#endif
            std::cout << "\033[31m" << names[actor_index] << " : " << errorMessage << "\033[0m" << std::endl;
        }
    }
    
    // Remove all the processed components from the list
    components_to_init.clear();
    components_to_init = not_processed;
}
