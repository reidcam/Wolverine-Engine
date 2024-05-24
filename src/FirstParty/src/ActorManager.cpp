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
    // TODO: This function
}

/**
 * Calls "OnUpdate" for every component on this actor that has it
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::Update(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    // TODO: This function
}

/**
 * Calls "OnLateUpdate" for every component on this actor that has it
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::LateUpdate(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    // TODO: This function
}

/**
 * Processes all components removed from the actor on this frame
 *
 * @param   actor_id the id of the actor that this function is acting on
*/
void Actors::ProcessRemovedComponents(int actor_id)
{
    int actor_index = GetIndex(actor_id);
    // TODO: This function
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
    
    // Update the number of loaded actors.
    num_loaded_actors++;
    num_total_actors++;
    
    return num_total_actors - 1;
}

/**
 * Destroys an actor
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
    if (id_to_index.contains(actor_id) && id_to_index[actor_id] != -1)
    {
        return id_to_index[actor_id];
    }
    
    std::cout << "error: attempt to access a nonexistant actor with ID: " << actor_id << std::endl;
    return -1;
}
