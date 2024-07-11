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

#include "Rigidbody.h"

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

//-------------------------------------------------------
// Native Components

/**
 * Returns true if the given type is a C++ based component
 *
 *  @param type                          the type to confirm if its native or not
 *  @returns               true if the given type is a native component, and false otherwise
 */
bool ComponentManager::IsComponentTypeNative(std::string type)
{
    if (type == "Rigidbody") {return true;}
    
    return false;
}

/**
 * Creates and returns a new native component of the given type
 *
 *  @param component_type   the component type to create a new copy of
 *  @returns               the table that contains our new native component
 */
sol::table ComponentManager::NewNativeComponent(std::string component_type)
{
    sol::table new_component;
    
    if (component_type == "Rigidbody")
    {
        Rigidbody* rigidbody = new Rigidbody();
        new_component = LuaAPI::GetLuaState()->create_table_with(rigidbody);
        return new_component;
    }
    
    return new_component;
}

/**
 * Makes a new copy of the given native component and returns it
 *
 *  @param native_component   the component that is being compied
 *  @returns                the table that contains our new native component
 */
sol::table ComponentManager::CopyNativeComponent(std::shared_ptr<sol::table> native_component)
{
    sol::table new_copy;
    return new_copy;
}
