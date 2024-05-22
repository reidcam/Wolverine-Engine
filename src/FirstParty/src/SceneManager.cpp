//
//  SceneManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages the active scene and handles loading new scenes
//

#include <stdio.h>

#include "SceneManager.h"

std::string Scene::scene_name; // The name of this scene

std::vector<int> Scene::actors; // A list of active actors indexes
std::vector<int> Scene::new_actors; // A list of actors that need to be initialized this frame
std::vector<int> Scene::dead_actors; // A list of actors that need to be deleted this frame

//-------------------------------------------------------
// Lifecycle Functions

/**
 * Update all of the actors in this scene
*/
void Scene::UpdateActors()
{
    // Add all of the new actors to this scene
    for (auto actor_index : new_actors)
    {
        Actors::Start(actor_index);
        
        // TODO: Initialize all components added to new actors at runtime.
        
        actors.push_back(actor_index);
    }
    new_actors.clear();
    
    // Process all of the components added to actors this frame
    for (auto actor_index : actors)
    {
        Actors::ProcessAddedComponents(actor_index);
    }
    
    // Update all actors
    for (auto actor_index : actors)
    {
        Actors::Update(actor_index);
    }
    
    // Late update all actors
    for (auto actor_index : actors)
    {
        Actors::LateUpdate(actor_index);
        
        // TODO: Initialize all components added to new actors at runtime.
    }
    
    // Processes all of the components removed from actors this frame
    for (auto actor_index : actors)
    {
        Actors::ProcessRemovedComponents(actor_index);
    }
    
    // Destroys all of the needed actors
    for (auto actor_index : dead_actors)
    {
        //delete actor;
    }
    dead_actors.clear();
}

//-------------------------------------------------------
// Loaders

/**
 * Loads a new scene
 *
 * @param   scene_name  the name of the scene to be loaded
*/
void Scene::LoadScene(std::string scene_name)
{
    
}

/**
 * Creates a new actor from a template and adds it to the current scene, then returns a reference to it
 *
 * @param   actor_template_name the name of the template to make a copy of in the scene
 * @returns                     the index of the newly created actor
*/
int Scene::Instantiate(std::string actor_template_name)
{
    Actors::LoadActorWithJSON(*GetTemplate(actor_template_name));
    
    // TODO: FInd a way to return the index of the created actor.
    return 0;
}

//-------------------------------------------------------
// Getters/Setters

/**
 * Gets the name of the current scene
 *
 * @returns             the name of the currently loaded scene
*/
std::string Scene::GetSceneName()
{
    return scene_name;
}

/**
 * Finds an actor in the current scene that has the provided name
 * If multiple actors have this name this returns the one that was loaded first
 *
 * @param   actor_name  the name of the actor that this function is trying to find
 * @returns             the index of the found actor
*/
int Scene::FindActorWithName(std::string actor_name)
{
    // TODO: This function
    return 0;
}

/**
 * Finds all actors in the current scene that have the provided name
 *
 * @param   actor_name  the name of the actors that this function is trying to find
 * @returns             a list of indexes that represent actors with the given name
*/
std::vector<int> Scene::FindAllActorsWithName(std::string actor_name)
{
    // TODO: This function
    return vector<int>(0);
}

/**
 * Finds an actor with the given ID
 *
 * @param   ID                     the ID of the actor that this function is trying to find
 * @returns             the index of the found actor
*/
int Scene::FindActorByID(int ID)
{
    return 0;
}
