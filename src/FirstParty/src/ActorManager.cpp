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

// The attributes of all the loaded actors
std::vector<std::string> Actors::names;
std::vector<int> Actors::IDs;
std::vector<bool> Actors::actor_enabled;

//-------------------------------------------------------
// Lifecycle Functions

/**
 * Initializes the actor at the given index
 * Called the frame an actor is loaded
 *
 * @param   actor_index the index of the actor that this function is acting on
*/
void Actors::Start(int actor_index)
{
    // Gives actor their ID
    IDs[actor_index] = num_total_actors;
    num_total_actors += 1;
}

/**
 * Processes all components added to the actor on this frame
 *
 * @param   actor_index the index of the actor that this function is acting on
*/
void Actors::ProcessAddedComponents(int actor_index)
{
    // TODO: This function
}

/**
 * Calls "OnUpdate" for every component on this actor that has it
 *
 * @param   actor_index the index of the actor that this function is acting on
*/
void Actors::Update(int actor_index)
{
    // TODO: This function
}

/**
 * Calls "OnLateUpdate" for every component on this actor that has it
 *
 * @param   actor_index the index of the actor that this function is acting on
*/
void Actors::LateUpdate(int actor_index)
{
    // TODO: This function
}

/**
 * Processes all components removed from the actor on this frame
 *
 * @param   actor_index the index of the actor that this function is acting on
*/
void Actors::ProcessRemovedComponents(int actor_index)
{
    // TODO: This function
}

//-------------------------------------------------------
// Getters/Setters

/**
 * Returns this actors name
 *
 * @param   actor_index the index of the actor that this function is acting on
 * @returns             the name of the given actor
*/
std::string Actors::GetName(int actor_index)
{
    return names[actor_index];
}

/**
 * Returns this actors ID
 *
 * @param   actor_index the index of the actor that this function is acting on
 * @returns             the ID of the given actor
*/
int Actors::GetID(int actor_index)
{
    return IDs[actor_index];
}

//-------------------------------------------------------
// Misc.

/**
 * Loads the data from JSON into the actor database to create a new actor
 *
 * @param   actor_data  the JSON that will be processed into a new actor
*/
void Actors::LoadActorWithJSON(const rapidjson::Value& actor_data)
{
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
}
