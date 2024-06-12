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
#include <unordered_map>

#include "TemplateDB.h"
#include "ComponentManager.h"

using namespace std;

// This is a dummy class to be exposed to Lua so that developers can program in an object-oriented way.
class Actor
{
public:
    int ID = 0;
};

class Actors
{
private:
    static int num_total_actors; // The total number of actors created during runtime
    static int num_loaded_actors; // The number of actors currently loaded in the game
    
    static std::unordered_map<int, int> id_to_index; // map of an actors id to its current index in the vectors
    
    /**
     * Gets the vector index of the actor with the given ID
     *
     * @param   actor_id the id of the actor that this function is acting on
    */
    static int GetIndex(int actor_id);
    
    // The attributes of all the loaded actors
    static std::vector<std::string> names;
    static std::vector<int> IDs;
    static std::vector<bool> actor_enabled;
    
    // The components of all the loaded actors
    // Layer 1: the index of the actor
    // Layer 2: the index of the component on the actor
    inline static std::vector< std::vector< std::shared_ptr<sol::table>>> components;
    
    // All components that need to be initialized this frame
    inline static std::vector<std::shared_ptr<sol::table>> components_to_init;
    //static std::vector<std::string> componentsToRemove;
    
    // The components across all actors with lifecycle functions
    inline static std::vector<std::shared_ptr<sol::table>> components_to_update;
    inline static std::vector<std::shared_ptr<sol::table>> components_to_update_late;
    
public:
    //-------------------------------------------------------
    // Lifecycle Functions
    
    /**
     * Cleans all of the dead actors out from the vectors
    */
    static void Cleanup();
    
    /**
     * Initializes the given actor
     * Called the frame an actor is loaded
     *
     * @param   actor_id the id of the actor that this function is acting on
    */
    static void Start(int actor_id);

    /**
     * Processes all components added to the actor on the previous frame
    */
    static void ProcessAddedComponents();

    /**
     * Calls "OnUpdate" for every component that has it
    */
    static void Update();

    /**
     * Calls "OnLateUpdate" for every component that has it
    */
    static void LateUpdate();

    /**
     * Processes all components removed from the actor on this frame
     *
     * @param   actor_id the id of the actor that this function is acting on
    */
    static void ProcessRemovedComponents(int actor_id);

    //-------------------------------------------------------
    // Getters/Setters
    
    /**
     * Returns this actors name
     *
     * @param   actor_id        the id of the actor that this function is acting on
     * @returns             the name of the given actor
    */
    static std::string GetName(int actor_id);

    /**
     * Returns this actors ID
     *
     * @param   actor_id        the id of the actor that this function is acting on
     * @returns             the ID of the given actor
    */
    static int GetID(int actor_id);
    
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
    static int LoadActorWithJSON(const rapidjson::Value& actor_data);
    
    /**
     * Loads the data from JSON into an existing lua value
     * DO NOT USE: This function is for use inside of the scene and actor managers only.
     *
     * @param   value                           the lua value that will store the given data
     * @param   data                             the JSON that will be processed into the table
     * @param   type                             the intended type of the lua value
    */
    static void JsonToLuaObject(sol::lua_value& value, const rapidjson::Value& data, sol::type type);
    
    /**
     * Destroys an actor
     * DO NOT USE: This function is for use inside of the scene and actor managers only.
     * In order to destroy an actor please use the "'destroy' function instead
     *
     * @param   actor_id        the id of the actor that this function is acting on
    */
    static void DestroyActor(int actor_id);
}; // Actors

#endif /* ActorManager_h */
