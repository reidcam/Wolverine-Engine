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
 * Processes all components added to the actor on this frame
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::ProcessAddedComponents(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    
    // Skip this function if the actor isn't enabled
    if (!actor_enabled[actor_index])
    {
        return;
    }
    // TODO: Call "OnStart" for every component on this actor that has it.
}

/**
 * Calls "OnUpdate" for every component on this actor that has it
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::Update(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    
    // Skip this function if the actor isn't enabled
    if (!actor_enabled[actor_index])
    {
        return;
    }
    
    //std::cout << GetID(actor_id) << std::endl;
    // TODO: Call "OnUpdate" for every component on this actor that has it.
}

/**
 * Calls "OnLateUpdate" for every component on this actor that has it
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::LateUpdate(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    
    // Skip this function if the actor isn't enabled
    if (!actor_enabled[actor_index])
    {
        return;
    }
    // TODO: Call "OnLateUpdate" for every component on this actor that has it.
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
    id_to_index[num_total_actors] = (int)IDs.size() - 1;
    
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
            LoadJSONIntoLuaTable(new_component, component_properties);

            //-------------------------------------------------------
            // Injects the new component with a reference to its actor
            Actor* _a = new Actor();
            _a->ID = num_total_actors;
            new_component["actor"] = _a;
        
            new_component["OnStart"](new_component);
            
            // Add the new component to the "components_to_add" vector
            components_to_add.push_back(std::make_shared<sol::table>(new_component));
        }
    }
    
    // Update the number of loaded actors.
    num_loaded_actors++;
    num_total_actors++;
    
    return num_total_actors - 1;
}

/**
 * Loads the data from JSON into an existing lua table
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 *
 * @param   out_table    the lua table that will store the given data
 * @param   data               the JSON that will be processed into the table
*/
void Actors::LoadJSONIntoLuaTable(sol::table& out_table, const rapidjson::Value& data)
{
    for (rapidjson::Value::ConstMemberIterator itr = data.MemberBegin(); itr != data.MemberEnd(); itr++)
    {
        std::string property_name = itr->name.GetString();
        sol::object property = out_table[itr->name.GetString()];
        sol::type property_type = property.get_type();
        
        // Adds the property to the component
        if (property_type == sol::type::string) { out_table[property_name] = itr->value.GetString(); }
        else if (property_type == sol::type::boolean) { out_table[property_name] = itr->value.GetBool(); }
        else if (property_type == sol::type::number)
        {
            if (itr->value.IsDouble()) { out_table[property_name] = itr->value.GetDouble(); }
            else { out_table[property_name] = itr->value.GetInt(); }
        }
        else if (property_type == sol::type::table)
        {
            sol::table new_table = ComponentManager::GetLuaState()->create_table();
            const rapidjson::Value& table_data = itr->value;
            
            // Iterate through the whole table until we fill each spot
            LoadJSONIntoLuaTable(new_table, table_data);
            
            out_table[property_name] = new_table;
        }
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
