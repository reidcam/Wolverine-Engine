//
//  ActorManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages actors
//

#include <stdio.h>

#include "ActorManager.h"

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
 * Initializes the actor at the given index
 * Called the frame an actor is loaded
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::Start(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    
    // TODO: ???
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
    for (auto& component : components_to_update)
    {
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        
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
}

/**
 * Calls "OnLateUpdate" for every component that has it
*/
void Actors::LateUpdate()
{
    for (auto& component : components_to_update_late)
    {
        int actor_index = GetIndex((*component)["actor"]["ID"]);
        
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
}

/**
 * Processes all components removed from the actor on this frame
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::ProcessRemovedComponents(int actor_id)
{
    int actor_index = GetIndex(actor_id);

    // TODO: Call "OnDestroy" for every component on this actor that has it.
    // What if the dev calls destroy in OnDestroy? The components in the destroyed actor wouldn't be processed correctly
    // Maybe call "OnDestroy" right when the component is destroyed?
}

//-------------------------------------------------------
// Getters/Setters

/**
 * Returns this actors name
 *
 * @param   actor_id        the id of the actor that this function is acting on
 * @returns             the name of the given actor
*/
std::string Actors::GetName(int actor_id)
{
    return names[GetIndex(actor_id)];
}

/**
 * Returns this actors ID
 *
 * @param   actor_id        the id of the actor that this function is acting on
 * @returns             the ID of the given actor
*/
int Actors::GetID(int actor_id)
{
    return IDs[GetIndex(actor_id)];
}

//-------------------------------------------------------
// Misc.

/**
 * Loads the data from JSON into the actor database to create a new actor
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 * In order to create a new actor please use the "'instantiate' function instead
 *
 * @param   actor_data  the JSON that will be processed into a new actor
 * @return             returns the id of the newly created actor
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
            sol::table new_component = ComponentManager::GetLuaState()->create_table();
            
            // The key of this component
            std::string key = itr->name.GetString();
            
            // Establishes inheritance between the new component and its type if specified
            if (itr->value.HasMember("type"))
            {
                std::string type = itr->value["type"].GetString();
                ComponentManager::EstablishInheritance(new_component, *GetComponentType(type));
                
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
            
//            // Sets the component to uninitialized, it will become initialized after onstart has been called.
//            new_component["IsComponentInitialized"] = false;
            
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
 * @param   value                           the lua value that will store the given data
 * @param   data                             the JSON that will be processed into the table
 * @param   type                             the intended type of the lua value
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
        sol::table _table = ComponentManager::GetLuaState()->create_table();
        _table[0] = sol::object(*ComponentManager::GetLuaState());
        
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
}

/**
 * Destroys an actor
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 * In order to destroy an actor please use the "'destroy' function instead
 *
 * @param   actor_id        the id of the actor that this function is acting on
*/
void Actors::DestroyActor(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    
    IDs[actor_index] = -1;
    actor_enabled[actor_index] = false;
    id_to_index.erase(actor_id);
    num_loaded_actors--;
}

/**
 * Gets the vector index of the actor with the given ID
 *
 * @param   actor_id the id of the actor that this function is acting on
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
