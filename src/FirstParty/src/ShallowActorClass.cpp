//
//    ShallowActorClass.h
//    wolverine_engine
//
//    Created by Jacob Robinson on 4/15/25.
//    Defines the "ShallowActor" class: an ENGINE-ONLY class used for generating small, temporary versions of an actor class that are detatched from the ActorManager's systems
//  DO NOT EXPOSE TO LUA!! THIS IS FOR ENGINE FUNCTIONALITY ONLY
//  Use cases: comparing values inside 2 actor template types

#include "ShallowActorClass.h"
#include "ActorManager.h"
#include "LuaAPI.h"

/*
 * ShallowActor constructor, pass through the type of the actor to make a copy of
 *
 * Basically: Creates new actor of type, steals its components, then deletes it
 */
ShallowActor::ShallowActor(std::string type)
{
    int dummy_id = Actors::LoadActorWithJSON(*GetTemplate(type));
    
    // Don't create a shallow actor of an invalid acotr
    if (Actors::GetID(dummy_id) == -1) { return; }
    
    // Copy the components of the given actor
    int number_of_components = Actors::GetNumberOfComponents(dummy_id);
    for (int i = 0; i < number_of_components; i++)
    {
        components.push_back(std::make_shared<sol::table>(Actors::GetComponentByIndex(dummy_id, i)));
    }
    
    // clear new actor's components and erase it
    Actors::components[Actors::id_to_index[dummy_id]].clear();
    Actors::PrepareActorForDestruction(dummy_id);
    Actors::DestroyActor(dummy_id);
}
ShallowActor::~ShallowActor()
{
    for (auto component : components)
    {
        LuaAPI::DeleteLuaTable(component);
    }
}
