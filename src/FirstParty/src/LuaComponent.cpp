//
//  LuaComponent.cpp
//  Wolverine Engine
//
//  Created by Nicholas Way on 6/4/24
//  Adapted from Rahul Unniyampath
//

#include "LuaComponent.h"

#include "ComponentManager.h"

/**
 * @brief Construct a new Lua Component object
 */
LuaComponent::LuaComponent()
{
    component_ref = ComponentManager::GetLuaState()->create_table();;

    SetEnabled(true);

    start_ref = std::make_unique<sol::protected_function>();

    update_ref =  std::make_unique<sol::protected_function>();

    late_update_ref =  std::make_unique<sol::protected_function>();

    fixed_update_ref = std::make_unique<sol::protected_function>();

    collision_enter_ref =  std::make_unique<sol::protected_function>();

    collision_exit_ref =  std::make_unique<sol::protected_function>();

    trigger_enter_ref =  std::make_unique<sol::protected_function>();

    trigger_exit_ref =  std::make_unique<sol::protected_function>();

    on_destroy_ref = std::make_unique<sol::protected_function>();
}


/**
* @brief Lua Component Copy Constructor
* @param other (LuaComponent)
*/
LuaComponent::LuaComponent(const LuaComponent &other) : Component(other)
{
    component_ref = ComponentManager::GetLuaState()->create_table();

    sol::table component_table = component_ref;
    sol::table other_component_lua_table = other.component_ref.as<sol::table>();

    ComponentManager::EstablishInheritance(component_table, other_component_lua_table);



    SetEnabled(true);

    if (has_on_start)
        start_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnStart"]);

    if (has_on_update)
        update_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnUpdate"]);

    if (has_on_late_update)
        late_update_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnLateUpdate"]);

    if (has_on_fixed_update)
        fixed_update_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnFixedUpdate"]);

    if (has_on_collision_enter)
        collision_enter_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnCollisionEnter"]);

    if (has_on_collision_exit)
        collision_exit_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnCollisionExit"]);

    if (has_on_trigger_enter)
        trigger_enter_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnTriggerEnter"]);

    if (has_on_trigger_exit)
        trigger_exit_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnTriggerExit"]);

    if (has_on_destroy)
        on_destroy_ref = std::make_unique<sol::protected_function>(component_ref.as<sol::lua_table>()["OnDestroy"]);
}


/**
 * @brief Construct a new Lua Component object
 * @param component_type (string)
 */
LuaComponent::LuaComponent(const std::string &component_type) : Component(component_type)
{
    component_ref = ComponentManager::GetLuaState()->create_table();
    sol::lua_table component_table = component_ref.as<sol::lua_table>();
    ComponentManager::EstablishInheritance(component_table, type);

    SetEnabled(true);

    has_on_start = HasLuaFunction("OnStart");

    has_on_update = HasLuaFunction("OnUpdate");

    has_on_late_update = HasLuaFunction("OnLateUpdate");

    has_on_fixed_update = HasLuaFunction("OnFixedUpdate");

    has_on_collision_enter = HasLuaFunction("OnCollisionEnter");

    has_on_collision_exit = HasLuaFunction("OnCollisionExit");

    has_on_trigger_enter = HasLuaFunction("OnTriggerEnter");

    has_on_trigger_exit = HasLuaFunction("OnTriggerExit");

    has_on_destroy = HasLuaFunction("OnDestroy");

    if (has_on_start)
        start_ref = std::make_unique<sol::protected_function>(component_table["OnStart"]);

    if (has_on_update)
        update_ref = std::make_unique<sol::protected_function>(component_table["OnUpdate"]);

    if (has_on_late_update)
        late_update_ref = std::make_unique<sol::protected_function>(component_table["OnLateUpdate"]);

    if (has_on_fixed_update)
        fixed_update_ref = std::make_unique<sol::protected_function>(component_table["OnFixedUpdate"]);

    if (has_on_collision_enter)
        collision_enter_ref = std::make_unique<sol::protected_function>(component_table["OnCollisionEnter"]);

    if (has_on_collision_exit)
        collision_exit_ref = std::make_unique<sol::protected_function>(component_table["OnCollisionExit"]);

    if (has_on_trigger_enter)
        trigger_enter_ref = std::make_unique<sol::protected_function>(component_table["OnTriggerEnter"]);

    if (has_on_trigger_exit)
        trigger_exit_ref = std::make_unique<sol::protected_function>(component_table["OnTriggerExit"]);

    if (has_on_destroy)
        on_destroy_ref = std::make_unique<sol::protected_function>(component_table["OnDestroy"]);
}
