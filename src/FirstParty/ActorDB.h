//
//  ActorDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Stores all of the actors that are currently loaded in the game
//

#ifndef ActorDB_h
#define ActorDB_h

#include <vector>
#include <string>

using namespace std;

class ActorDB
{
private:
    static int num_loaded_actors; // The total number of actors created during runtime
    
    static int num_actors; // The number of actors currently loaded in the game
    
    // The attributes of all the loaded actors
    static std::vector<std::string> names;
    static std::vector<int> IDs;
    static std::vector<bool> actor_enabled;
    
public:
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

    /**
     * Returns this actors name
     *
     * @param   actor_index the index of the actor that this function is acting on
    */
    static std::string GetName(int actor_index);

    /**
     * Returns this actors ID
     *
     * @param   actor_index the index of the actor that this function is acting on
    */
    static int GetID(int actor_index);
}; // ActorDB

#endif /* ActorDB_h */
