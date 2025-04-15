//
//	ShallowActorClass.h
//	wolverine_engine
//
//	Created by Jacob Robinson on 4/15/25.
//  ACTOR REFLECTION SYSTEM
//	Defines the "ShallowActor" class: an ENGINE-ONLY class used for generating small, temporary versions of an actor class that are detatched from the ActorManager's systems
//  DO NOT EXPOSE TO LUA!! THIS IS FOR ENGINE FUNCTIONALITY ONLY
//  Use cases: comparing values inside 2 actor template types

#ifndef ShallowActorClass_h
#define ShallowActorClass_h

#include <vector>
#include <string>

#include "sol/sol.hpp"

//    Defines the "ShallowActor" class: an ENGINE-ONLY class used for generating small, temporary versions of an actor class that are detatched from the ActorManager's systems
// This class exists purely for extracting data from the ActorManager, do NOT attempt to call any methods from the stored components or undefined befahiour will occur.
class ShallowActor
{
public:
    // The shallow actor's components
    std::vector<std::shared_ptr<sol::table>> components;
    
    /*
     * ShallowActor constructor, pass through the type of the actor to make a copy of
     */
    ShallowActor(std::string type);
    ~ShallowActor();
};


#endif /* ShallowActorClass.h */
