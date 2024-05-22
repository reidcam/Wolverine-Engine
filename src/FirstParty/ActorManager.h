//
//  ActorManager.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages actors
//

#ifndef ActorManager_h
#define ActorManager_h

#include <vector>
#include <string>

#include "TemplateDB.h"

using namespace std;

class Actors
{
private:
    static int num_total_actors; // The total number of actors created during runtime
    static int num_loaded_actors; // The number of actors currently loaded in the game
    
    // The attributes of all the loaded actors
    static std::vector<std::string> names;
    static std::vector<int> IDs;
    static std::vector<bool> actor_enabled;
    
public:
    //-------------------------------------------------------
    // Lifecycle Functions
    
    /**
     * Initializes the actor at the given index
     * Called the frame an actor is loaded
     *
     * @param   actor_index the index of the actor that this function is acting on
    */
    static void Start(int actor_index);

    /**
     * Processes all components added to the actor on this frame
     *
     * @param   actor_index the index of the actor that this function is acting on
    */
    static void ProcessAddedComponents(int actor_index);

    /**
     * Calls "OnUpdate" for every component on this actor that has it
     *
     * @param   actor_index the index of the actor that this function is acting on
    */
    static void Update(int actor_index);

    /**
     * Calls "OnLateUpdate" for every component on this actor that has it
     *
     * @param   actor_index the index of the actor that this function is acting on
    */
    static void LateUpdate(int actor_index);

    /**
     * Processes all components removed from the actor on this frame
     *
     * @param   actor_index the index of the actor that this function is acting on
    */
    static void ProcessRemovedComponents(int actor_index);

    //-------------------------------------------------------
    // Getters/Setters
    
    /**
     * Returns this actors name
     *
     * @param   actor_index the index of the actor that this function is acting on
     * @returns             the name of the given actor
    */
    static std::string GetName(int actor_index);

    /**
     * Returns this actors ID
     *
     * @param   actor_index the index of the actor that this function is acting on
     * @returns             the ID of the given actor
    */
    static int GetID(int actor_index);
    
    //-------------------------------------------------------
    // Misc.
    
    /**
     * Loads the data from JSON into the actor database to create a new actor
     *
     * @param   actor_data  the JSON that will be processed into a new actor
    */
    static void LoadActorWithJSON(const rapidjson::Value& actor_data);
}; // Actors

#endif /* ActorManager_h */
