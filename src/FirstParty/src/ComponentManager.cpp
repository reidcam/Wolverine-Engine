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
#include "LuaAPI.h"

/**
 * Establishes inheritance between two tables by setting one to be the metatable of the other
 *
 *  @param instance_table   the table to be inheriting from the parent table (passed by reference)
 *  @param parent_table        the table that is being inherited from (passed by reference)
 */
void ComponentManager::EstablishInheritance(sol::table& instance_table, sol::table& parent_table)
{
    /* We must create a metatable to establish inheritance in Lua */
    sol::table new_metatable = LuaAPI::GetLuaState()->create_table();
    new_metatable["__index"] = parent_table;
    
    /* We must use the raw lua C-API (lua stack) to preform a "setmetatable" operation */
    instance_table[sol::metatable_key] = new_metatable;
}
