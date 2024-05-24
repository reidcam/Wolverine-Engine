//
//  SceneManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Manages the active scene and handles loading new scenes
//

#include <stdio.h>

#include "SceneManager.h"

std::string Scene::current_scene_name; // The name of this scene

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
    // Cleans up all of the dead actors in the actor database
    Actors::Cleanup();
    
    Scene::Instantiate("BouncyBox");
    Actors::DestroyActor(Scene::Instantiate("BouncyBox"));
    Scene::Instantiate("BouncyBox");
    
    // Add all of the new actors to this scene
    for (auto actor : new_actors)
    {
        Actors::Start(actor);
        
        // TODO: Initialize all components added to new actors at runtime.
        
        actors.push_back(actor);
    }
    new_actors.clear();
    
    // Process all of the components added to actors this frame
    for (auto actor : actors)
    {
        Actors::ProcessAddedComponents(actor);
    }
    
    // Update all actors
    for (auto actor : actors)
    {
        Actors::Update(actor);
    }
    
    // Late update all actors
    for (auto actor : actors)
    {
        Actors::LateUpdate(actor);
        
        // TODO: Initialize all components added to new actors at runtime.
    }
    
    // Processes all of the components removed from actors this frame
    for (auto actor : actors)
    {
        Actors::ProcessRemovedComponents(actor);
    }
    
    // Destroys all of the needed actors
    for (auto actor : dead_actors)
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
                            
                            Actors::LoadActorWithJSON(combined_actor);
                        }
                        else
                        {
                            Actors::LoadActorWithJSON(member);
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
    return Actors::LoadActorWithJSON(*GetTemplate(actor_template_name));;
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
