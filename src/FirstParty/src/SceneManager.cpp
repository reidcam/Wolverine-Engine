//
//  SceneManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages the active scene and handles loading new scenes
//

#include <stdio.h>

#include "SceneManager.h"

std::string Scene::current_scene_name = ""; // The name of this scene
int Scene::current_scene_lifetime = 0; // The number of frames this scene has been active for

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
    current_scene_lifetime++;
    
    // Cleans up all of the dead actors in the actor database
    if (current_scene_lifetime % 60 == 0)
    {
        Actors::Cleanup();
    }
    
    // Add all of the new actors to this scene
    for (auto actor : new_actors)
    {
        Actors::Start(actor);
        actors.push_back(actor);
    }
    new_actors.clear();
    
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
        
//        int index_to_remove = -1;
//        // Find the index of the actor within 'actors' and delete it
//        for (int i = 0; i < actors.size(); i++)
//        {
//            if (actor == actors[i])
//            {
//                index_to_remove = i;
//                break;
//            }
//        }
//        
//        // Erase the dead actor
//        if (index_to_remove != -1) { actors.erase(actors.begin() + index_to_remove); }
//        else
//        {
//            // If the actor isn't in 'actors' check 'new_actors'
//            // Find the index of the actor within 'new_actors' and delete it
//            for (int i = 0; i < new_actors.size(); i++)
//            {
//                if (actor == new_actors[i])
//                {
//                    index_to_remove = i;
//                    break;
//                }
//            }
//            
//            if (index_to_remove != -1) { new_actors.erase(new_actors.begin() + index_to_remove); }
//        }
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
    // TODO: Scene path database so that this code doesn't have to search for the scene path every time.
    /* Load files from this path */
    const std::string path = "resources/scenes";
    
    // Fills up the database if the path exists
    if (std::filesystem::exists(path))
    {
        for (const auto& file : std::filesystem::directory_iterator(path))
        {
            // If this scene matches the name of the one to be loaded
            if (file.path().filename().stem().stem().string() == scene_name)
            {
                current_scene_name = scene_name;
                current_scene_lifetime = 0;
                
                rapidjson::Document scene_document;
                EngineUtils::ReadJsonFile(file.path().string(), scene_document);
                
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
//                            rapidjson::StringBuffer buffer;
//                            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
//                            combined_actor.Accept(writer);
//                            std::cout << buffer.GetString() << std::endl;
                            
                            new_actors.push_back(Actors::LoadActorWithJSON(combined_actor));
                        }
                        else
                        {
                            new_actors.push_back(Actors::LoadActorWithJSON(member));
                        }
                    }
                }
                // Return after loading the needed scene
                return;
            }
        }
    }
}

/**
 * Creates a new actor from a template and adds it to the current scene, then returns a reference to it
 *
 * @param   actor_template_name the name of the template to make a copy of in the scene
 * @returns                     the id of the newly created actor
*/
int Scene::Instantiate(std::string actor_template_name)
{
    // Creates the actor
    int new_actor_id = Actors::LoadActorWithJSON(*GetTemplate(actor_template_name));
    // Adds the actor to the scene
    new_actors.push_back(new_actor_id);
    
    return new_actor_id;
}

/**
 * destroys an actor and then removes it from the scene
 *
 * @param   actor_id    the id of the actor to be destroyed
*/
void Scene::Destroy(int actor_id)
{
    dead_actors.push_back(actor_id);
    
    Actors::PrepareActorForDestruction(actor_id);
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
    return current_scene_name;
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
