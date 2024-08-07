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

/**
 * Calls "OnUpdate" for every component that has it
*/
void Actors::Update()
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
 * Calls "OnLateUpdate" for every component that has it
*/
void Actors::LateUpdate()
{
    std::vector<std::shared_ptr<sol::table>> living_components; // The "LateUpdate" list without any of the dead components
    
    for (auto& component : components_to_update_late)
    {
        // If this component is dead, skip it
        if ((*component)["REMOVED_FROM_ACTOR"] == true)
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
                
                // Allows the component to know its own type
                new_component["type"] = type;
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
            for (rapidjson::Value::ConstMemberIterator itr2 = component_properties.MemberBegin(); itr2 != component_properties.MemberEnd(); itr2++)
            {
                std::string property_name = itr2->name.GetString();
                sol::type property_type = new_component[property_name].get_type();

                sol::lua_value property = "";
                
                JsonToLuaObject(property, itr2->value, property_type);
                
                new_component[property_name] = property;
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
 * Loads the data from JSON into an existing lua value
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 *
 * @param   value    the lua value that will store the given data
 * @param   data     the JSON that will be processed into the table
 * @param   type     the intended type of the lua value
*/
void Actors::JsonToLuaObject(sol::lua_value& value, const rapidjson::Value& data, sol::type type)
{
    // Adds the property to the component
    if (type == sol::type::string) { value = data.GetString(); }
    else if (type == sol::type::boolean) { value = data.GetBool(); }
    else if (type == sol::type::number)
    {
        if (data.IsDouble()) { value = data.GetDouble(); }
        else { value = data.GetInt(); }
    }
    else if (type == sol::type::table)
    {
        sol::table _table = LuaAPI::GetLuaState()->create_table();
        _table[0] = sol::object(*LuaAPI::GetLuaState());
        
        // Add all of the values in the data to our new table.
        int i = 1;
        for (rapidjson::Value::ConstMemberIterator itr = data.MemberBegin(); itr != data.MemberEnd(); itr++)
        {
            const rapidjson::Value& _data = itr->value;
            
            sol::lua_value _value = "";
            
            // Determine the type of value that is being added to the table
            sol::type _type = sol::type::none;
            if (itr->value.IsString()) { _type = sol::type::string; }
            else if (itr->value.IsBool()) { _type = sol::type::boolean; }
            else if (itr->value.IsInt() || itr->value.IsDouble()) { _type = sol::type::number; }
            else if (itr->value.IsObject()) { _type = sol::type::table; }
            
            JsonToLuaObject(_value, _data, _type);
            
            // Add the value to the new table
            _table[i] = _value;
            i++;
        }
        value = _table;
    }
    else
    {
        // If the sol type is not any of the above, determine the object type based off of the json
        sol::type second_type;
        if (data.IsString()) { second_type = sol::type::string; }
        else if (data.IsBool()) { second_type = sol::type::boolean; }
        else if (data.IsNumber()) { second_type = sol::type::number; }
        else if (data.IsObject()) { second_type = sol::type::table; }
        JsonToLuaObject(value, data, second_type);
    }
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
    return components[GetIndex(actor_id)].size();
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
    return *components[GetIndex(actor_id)][component_index];
}

/**
* Gets where or not an actor is enabled
*
* @param     actor_id    the id of the actor that this function is acting on
* @return    a bool for whether or not the actor is enabled
*/
bool Actors::GetActorEnabled(int actor_id)
{
    return actor_enabled[GetIndex(actor_id)];
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
