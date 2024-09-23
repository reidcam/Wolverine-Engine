//
//  SceneManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages the active scene and handles loading new scenes
//

#include <stdio.h>

#include "SceneManager.h"

#include "LuaAPI.h"

std::string Scene::initial_scene_name = ""; // The name of the first scene to be loaded in the game
std::string Scene::current_scene_name = ""; // The name of this scene
int Scene::current_scene_lifetime = 0; // The number of frames this scene has been active for

bool Scene::load_new_scene = false; // True if we want to load a new scene at the end of this frame
std::string Scene::new_scene_name = ""; // The name of the new scene we're loading into

std::vector<int> Scene::actors; // A list of active actors indexes
std::vector<int> Scene::dead_actors; // A list of actors that need to be deleted this frame

//-------------------------------------------------------
// Lifecycle Functions

/**
 * Update all of the actors in this scene
*/
void Scene::UpdateActors()
{
    current_scene_lifetime++;
    
    // Cleans up all of the dead actors in the actor database
    if (current_scene_lifetime % 60 == 0)
    {
        Actors::Cleanup();
    }
    
    // Process all of the components added to actors on the previous frame
    Actors::ProcessAddedComponents();
    
    // Update all actors
    Actors::Update();
    
    // Late update all actors
    Actors::LateUpdate();
    
    // Processes all of the components removed from actors this frame
    Actors::ProcessRemovedComponents();
    
    // Destroys all of the dead actors
    // TODO: Take another look at how to remove dead actors from a scene, this feels VERY slow
    for (auto actor : dead_actors)
    {
        // Destroy all of the actors that have been prepped for destruction.
        Actors::DestroyActor(actor);
        
        int index_to_remove = -1;
        // Find the index of the actor within 'actors' and delete it
        for (int i = 0; i < actors.size(); i++)
        {
            if (actor == actors[i])
            {
                index_to_remove = i;
                break;
            }
        }
        // Erase the dead actor
        if (index_to_remove != -1) { actors.erase(actors.begin() + index_to_remove); }
    }
    dead_actors.clear();
}

//-------------------------------------------------------
// Loaders

/**
 * Loads a new scene with the given name.
 *
 * @param   scene_name  the name of the scene to be loaded
*/
void Scene::ChangeScene(std::string scene_name)
{
    // Crash the engine if the new scene doesn't exist
    GetScenePath(scene_name);
    
    new_scene_name = scene_name;
    load_new_scene = true;
    
    // Prepare all of the actors in this scene for deletion
    for (auto& actor_id : actors)
    {
        Actor to_destroy;
        to_destroy.ID = actor_id;
        
        Destroy(to_destroy);
    }
}

/**
 * Loads the new scene
 * NOTE: DO NOT EXPOSE TO LUA! This is called in GameLoop ONLY if ChangeScene was called this frame.
*/
void Scene::LoadNewScene()
{
    load_new_scene = false;
    current_scene_name = new_scene_name;
    current_scene_lifetime = 0;
    
    rapidjson::Document scene_document;
    EngineUtils::ReadJsonFile(GetScenePath(new_scene_name), scene_document);
    
    if (scene_document.HasMember("actors"))
    {
        // Load all of the new actors into the game
        for (auto& member : (scene_document)["actors"].GetArray())
        {
            // If the actor has a value for template, combine the actor with the template before loading it
            if (member.HasMember("template"))
            {
                rapidjson::Document combined_actor;
                
                rapidjson::Document lhs;
                lhs.CopyFrom(member, lhs.GetAllocator());
                rapidjson::Document rhs;
                rhs.CopyFrom(*GetTemplate(member["template"].GetString()), rhs.GetAllocator());

                EngineUtils::CombineJsonDocuments(lhs, rhs, combined_actor);
                
                // FOR TESTS: Output the combined JSON as a string
                // rapidjson::StringBuffer buffer;
                // rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
                // combined_actor.Accept(writer);
                // std::cout << buffer.GetString() << std::endl;
                
                actors.push_back(Actors::LoadActorWithJSON(combined_actor));
            }
            else
            {
                actors.push_back(Actors::LoadActorWithJSON(member));
            }
        }
    }
    // Return after loading the needed scene
    return;
}

/**
 * Creates a new actor from a template and adds it to the current scene, then returns a reference to it
 *
 * @param   actor_template_name    the name of the template to make a copy of in the scene
 * @returns                        the id of the newly created actor
*/
int Scene::Instantiate(std::string actor_template_name)
{
    // Creates the actor
    int new_actor_id = Actors::LoadActorWithJSON(*GetTemplate(actor_template_name));
    // Adds the actor to the scene
    actors.push_back(new_actor_id);
    
    return new_actor_id;
}

/**
 * destroys an actor and then removes it from the scene
 *
 * @param   actor    the actor to be destroyed
*/
void Scene::Destroy(Actor actor)
{
    dead_actors.push_back(actor.ID);
    
    Actors::PrepareActorForDestruction(actor.ID);
}

//-------------------------------------------------------
// Getters/Setters

/**
 * Gets the name of the current scene
 *
 * @returns    the name of the currently loaded scene
*/
std::string Scene::GetSceneName()
{
    return current_scene_name;
}

/**
 * Finds an actor in the current scene that has the provided name
 * If multiple actors have this name this returns the one that was loaded first
 *
 * @param   actor_name    the name of the actor that this function is trying to find
 * @returns               the found actor
*/
Actor Scene::FindActorWithName(std::string actor_name)
{
    // Create a new actor that will be assigned with the value of our actor with the given name if one is found
    Actor found_actor;
    found_actor.ID = -1;
    
    // Find the actor
    for (auto& actor_id : actors)
    {
        if (Actors::GetName(actor_id) == actor_name)
        {
            found_actor.ID = actor_id;
            return found_actor;
        }
    }
    
    return found_actor;
}

/**
 * Finds all actors in the current scene that have the provided name
 *
 * @param   actor_name    the name of the actors that this function is trying to find
 * @returns               a list of indexes that represent actors with the given name
*/
sol::table Scene::FindAllActorsWithName(std::string actor_name)
{
    // Create a new empty lua table to fill with our actors, and make it 1 indexed so it can be looped through in lua
    sol::table actors_with_name = LuaAPI::GetLuaState()->create_table();
    actors_with_name[0] = sol::object(*LuaAPI::GetLuaState());
    
    // Fill the table
    // Keys must be increasing integers so the table is ipairs compatable
    int i = 1;
    for (auto& actor_id : actors)
    {
        if (Actors::GetName(actor_id) == actor_name)
        {
            // Add our actor with the given name to the list
            Actor found_actor;
            found_actor.ID = actor_id;
            actors_with_name[i] = found_actor;
            i++;
        }
    }
    
    return actors_with_name;
}

/**
 * Finds an actor with the given ID
 *
 * @param   ID    the ID of the actor that this function is trying to find
 * @returns       the found actor
*/
Actor Scene::FindActorByID(int ID)
{
    Actor found_actor;
    found_actor.ID = ID;
    
    return found_actor;
}

/**
 * Clears all of the data from this manager
 * NOTE: DO NOT EXPOSE TO LUA! This function is primarily meant to reset the game for the editor
 * Could cause unintended behavior if used improperly
*/
void Scene::ResetManager()
{
    Actors::ResetManager();
    
    actors.clear();
    dead_actors.clear();
    
    new_scene_name = initial_scene_name;
    LoadNewScene();
}

/**
 * Returns a copy of the 'actors' vector
 * NOTE: DO NOT EXPOSE TO LUA! This function is primarily meant to pass actor data to the editor
 * Could cause unintended behavior if used improperly
*/
std::vector<int> Scene::GetAllActorsInScene()
{
    return actors;
}
