//
//  SceneManager.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages the active scene and handles loading new scenes
//

#ifndef SceneManager_h
#define SceneManager_h

#include "ActorManager.h"
#include "SceneDB.h"

class Scene
{
private:
    static std::string current_scene_name; // The name of this scene
    static int current_scene_lifetime; // The number of frames this scene has been active for
    
    static std::vector<int> actors; // A list of active actors indexes
    static std::vector<int> dead_actors; // A list of actors that need to be deleted this frame
public:
    static std::string new_scene_name; // The name of the new scene we're loading into
    static bool load_new_scene; // True if we want to load a new scene at the end of this frame
    //-------------------------------------------------------
    // Lifecycle Functions
    
    /**
     * Update all of the actors in this scene
    */
    static void UpdateActors();
    
    //-------------------------------------------------------
    // Loaders
    
    /**
     * Loads a new scene with the given name.
     *
     * @param   scene_name  the name of the scene to be loaded
    */
    static void ChangeScene(std::string scene_name);
    
    /**
     * Loads the new scene
     * NOTE: DO NOT EXPOSE TO LUA! This is called in GameLoop ONLY if ChangeScene was called this frame.
    */
    static void LoadNewScene();
    
    /**
     * Creates a new actor from a template and adds it to the current scene, then returns a reference to it
     *
     * @param   actor_template_name the name of the template to make a copy of in the scene
     * @returns                     the index of the newly created actor
    */
    static int Instantiate(std::string actor_template_name);
    
    /**
     * destroys an actor and then removes it from the scene
     *
     * @param   actor    the actor to be destroyed
    */
    static void Destroy(Actor actor);
    
    //-------------------------------------------------------
    // Getters/Setters
    
    /**
     * Gets the name of the current scene
     *
     * @returns             the name of the currently loaded scene
    */
    static std::string GetSceneName();
    
    /**
     * Finds an actor in the current scene that has the provided name
     * If multiple actors have this name this returns the one that was loaded first
     *
     * @param   actor_name  the name of the actor that this function is trying to find
     * @returns            the found actor
    */
    static Actor FindActorWithName(std::string actor_name);
    
    /**
     * Finds all actors in the current scene that have the provided name
     *
     * @param   actor_name  the name of the actors that this function is trying to find
     * @returns             a list of indexes that represent actors with the given name
    */
    static sol::table FindAllActorsWithName(std::string actor_name);
    
    /**
     * Finds an actor with the given ID
     *
     * @param   ID                     the ID of the actor that this function is trying to find
     * @returns             the found actor
    */
    static Actor FindActorByID(int ID);
}; // Scene

#endif /* SceneManager_h */
