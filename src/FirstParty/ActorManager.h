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
#include <unordered_set>
#include <queue>

#include "TemplateDB.h"
#include "ComponentManager.h"

using namespace std;

/**
 * This is a dummy class to be exposed to Lua so that developers can program in an object-oriented way.
 */
class Actor
{
public:
    int ID = -1;
};

/**
 * The real Actor class storing all of the Actor information
 */
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
    static std::vector<std::string> templates;
    static std::vector<int> IDs;
    static std::vector<bool> actor_enabled;
    
    // The components of all the loaded actors
    // Layer 1: the index of the actor
    // Layer 2: the index of the component on the actor
    inline static std::vector< std::vector< std::shared_ptr<sol::table>>> components;
    
    // All components that need to be initialized this frame
    inline static std::vector<std::shared_ptr<sol::table>> components_to_init;
    
    // All components that need to be deleted this frame
    inline static std::queue<std::shared_ptr<sol::table>> components_to_delete;
    
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
     * Processes all components removed from actors on this frame
    */
    static void ProcessRemovedComponents();

    //-------------------------------------------------------
    // Getters/Setters
    
    /**
     * Returns this actors name
     *
     * @param   actor_id    the id of the actor that this function is acting on
     * @returns             the name of the given actor
    */
    static std::string GetName(int actor_id);

    /**
     * Returns this actors ID
     *
     * @param   actor_id    the id of the actor that this function is acting on
     * @returns             the ID of the given actor
    */
    static int GetID(int actor_id);
    
    /**
    * Gets wether or not an actor is enabled
    *
    * @param     actor_id    the id of the actor that this function is acting on
    * @return                a bool for whether or not the actor is enabled
    */
    static bool GetActorEnabled(int actor_id);
    
    /**
    * Sets wether or not an actor is enabled
    *
    * @param     actor_id    the id of the actor that this function is acting on
    * @param    is_enabled  the new enabled status of the actor
    */
    static void SetActorEnabled(int actor_id, bool is_enabled);
    
    /**
     * Returns this actors template name
     *
     * @param   actor_id    the id of the actor that this function is acting on
     * @returns             the name of the template of the given actor, bank string if it does not have a template
    */
    static std::string GetTemplateName(int actor_id);

    
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
    static int LoadActorWithJSON(const rapidjson::Value& actor_data);
    
    /**
     * Prepares an actor for destruction later this frame
     * DO NOT USE: This function is for use inside of the scene and actor managers only.
     * In order to destroy an actor please use the "'destroy' function instead. This ensures that actors are properly prepared for destruction.
     *
     * @param   actor_id    the id of the actor that this function is acting on
    */
    static void PrepareActorForDestruction(int actor_id);
    
    /**
     * Destroys an actor
     * DO NOT USE: This function is for use inside of the scene and actor managers only.
     * In order to destroy an actor please use the "'destroy' function instead. This ensures that actors are properly prepared for destruction.
     *
     * @param   actor_id    the id of the actor that this function is acting on
    */
    static void DestroyActor(int actor_id);
    
    //-------------------------------------------------------
    // Components
    
    /**
     * Removes a component from an actor and marks it for deletion
     *
     * @param   actor_id     the id of the actor that this function is acting on
     * @param   component    the component to be removed
    */
    static void RemoveComponentFromActor(int actor_id, sol::table component);
    
    /**
     * Gets the first component on the given actor with the given type if it exists.
     *
     * @param   actor_id    the id of the actor that this function is acting on
     * @param   type        the type of component we're searching for
     * @return              the first component on the given actor with the given type, if none are found returns null
    */
    static sol::table GetComponentByType(int actor_id, std::string type);

    /**
    * Gets the number of components that an actor has
    * 
    * @param     actor_id    the id of the actor that this function is acting on 
    * @return                the number of components that the actor currently has
    */
    static int GetNumberOfComponents(int actor_id);
    
    /**
    * Gets a component by index
    * 
    * @param     actor_id           the id of the actor that this function is acting on
    * @param     component_index    the index of the component
    * @return                       a sol table containing the component
    */
    static sol::table GetComponentByIndex(int actor_id, int component_index);

    /**
     * Gets all of the components on the given actor with the given type if they exist.
     *
     * @param   actor_id    the id of the actor that this function is acting on
     * @param   type        the type of component we're searching for
     * @return              a list of all the components with the given type, if none are found returns null
    */
    static sol::table GetComponentsByType(int actor_id, std::string type);
    
    /**
     * Gets the component on the given actor with the given key if it exists.
     *
     * @param   actor_id    the id of the actor that this function is acting on
     * @param   key         the key of the component we're searching for
     * @return              the component on the given actor with the given key, if none are found returns null
    */
    static sol::table GetComponentByKey(int actor_id, std::string key);
    
    //-------------------------------------------------------
    // Editor Tools
    
    /**
     * Clears all of the components and actors from this manager
     * Used to do a hard reset of invincible actors and components before loading a new scene
    */
    static void ResetManager();
    
    /**
     * Loops through all the components and ONLY runs onupdate if its type is needed for the editor.
     * This is primarily used to trigger SpriteRenderers and other visual components for the EDITOR in editor mode.
     *
     * Not very DRY I know...
     *
     * @param    editor_components   a list of all the components that are needed for editor mode to function
     */
    static void EditorUpdateComponents(std::unordered_set<std::string> editor_components);
    
    /**
     * Loops through all the components and ONLY runs onstart if its type is needed for the editor.
     * This is primarily used to prepare SpriteRenderers and other visual components for the EDITOR in editor mode.
     *
     * Not very DRY I know...
     *
     * @param    editor_components   a list of all the components that are needed for editor mode to function
     */
    static void EditorStartComponents(std::unordered_set<std::string> editor_components);
    
}; // Actors

#endif /* ActorManager_h */
