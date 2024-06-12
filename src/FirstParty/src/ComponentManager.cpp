//
//  ComponentManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 6/4/24.
//  Manages Components
//

#include <stdio.h>

#include "ComponentManager.h"
#include "ActorManager.h"

/**
 * Initializes the component manager
*/
void ComponentManager::init()
{
    // Initialize the state
    l_state = new sol::state();
    l_state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
    
    // Initialize the C++ classes and functions to get exposed to Lua
    ExposeCPP();
    
    // Load all of the component types
    LoadComponentTypes();
}

/**
 * Exposes C++ classes and functions to lua so that they can be used.
 */
void ComponentManager::ExposeCPP()
{
    // "Actor" class
    sol::usertype<Actor> actor_type = l_state->new_usertype<Actor>("Actor");
    actor_type["ID"] = &Actor::ID;
    actor_type["GetName"] = Actors::GetName;
}

/**
 * Establishes inheritance between two tables by setting one to be the metatable of the other
 *
 *  @param instance_table   the table to be inheriting from the parent table (passed by reference)
 *  @param parent_table        the table that is being inherited from (passed by reference)
 */
void ComponentManager::EstablishInheritance(sol::table& instance_table, sol::table& parent_table)
{
    /* We must create a metatable to establish inheritance in Lua */
    sol::table new_metatable = l_state->create_table();
    new_metatable["__index"] = parent_table;
    
    /* We must use the raw lua C-API (lua stack) to preform a "setmetatable" operation */
    instance_table[sol::metatable_key] = new_metatable;
}
