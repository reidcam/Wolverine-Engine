//
//  LuaComponent.h
//  Wolverine Engine
//
//  Created by Nicholas Way on 6/4/24
//  Adapted from Rahul Unniyampath
//

#ifndef LuaComponent_h
#define LuaComponent_h

#include "Component.h"

#include <stdio.h>

class LuaComponent : public Component {

    friend class ComponentManager;

public:

    explicit LuaComponent(const LuaComponent &other);

    explicit LuaComponent(const std::string &component_type);

    std::shared_ptr<Component> Clone() const override;

    std::shared_ptr<Component> GetSharedPointer() override;

    void UpdateComponentWithJSON(const rapidjson::Value &component_json) override;

    void SetComponentKey(const std::string &key) override;

    void SetActor(std::weak_ptr<Actor> &actor) override;

    void OnStart() override;

    void OnUpdate() override;

    void OnLateUpdate() override;

    void OnFixedUpdate() override;

    // void OnCollisionEnter(const CollisionData &collision) override;

    // void OnCollisionExit(const CollisionData &collision) override;

    // void OnTriggerEnter(const CollisionData &collision) override;

    // void OnTriggerExit(const CollisionData &collision) override;

    void OnDestroy() override;

    void SetEnabled(bool enabled) override;

    bool IsEnabled() const override;

private:

    explicit LuaComponent();

    bool HasLuaFunction(const std::string &function_name);

    void OnLifecycleGeneric(const std::string &function_name);

    std::unordered_map<std::string, std::unique_ptr<sol::protected_function>> function_refs;

    std::unique_ptr<sol::protected_function> start_ref;

    std::unique_ptr<sol::protected_function> update_ref;

    std::unique_ptr<sol::protected_function> late_update_ref;

    std::unique_ptr<sol::protected_function> fixed_update_ref;

    std::unique_ptr<sol::protected_function> collision_enter_ref;

    std::unique_ptr<sol::protected_function> collision_exit_ref;

    std::unique_ptr<sol::protected_function> trigger_enter_ref;

    std::unique_ptr<sol::protected_function> trigger_exit_ref;

    std::unique_ptr<sol::protected_function> on_destroy_ref;

};


inline std::shared_ptr<Component> LuaComponent::Clone() const { return std::make_shared<LuaComponent>(*this); }


inline std::shared_ptr<Component> LuaComponent::GetSharedPointer() { return shared_from_this(); }


inline void LuaComponent::SetComponentKey(const std::string &key)
{
    Component::SetComponentKey(key);

    component_ref.as<sol::lua_table>()["key"] = key;
}


inline void LuaComponent::SetActor(std::weak_ptr<Actor> &actor)
{
    Component::SetActor(actor);

    if (std::shared_ptr<Actor> actor_sp = this->actor.lock())
        component_ref.as<sol::lua_table>()["actor"] = actor_sp.get();;
}


inline void LuaComponent::OnLifecycleGeneric(const std::string &function_name)
{
    if (function_refs.count(function_name) > 0 && IsEnabled())
    {
        sol::protected_function_result function_result = (*function_refs[function_name])(component_ref);

        if (!function_result.valid())
            ReportError(actor.lock()->name, function_result);
    }
}


inline void LuaComponent::OnStart()
{
    if (HasOnStart() && IsEnabled())
    {
        sol::protected_function_result start_result = (*start_ref)(component_ref);

        if (!start_result.valid())
            ReportError(actor.lock()->name, start_result);
    }
}


inline void LuaComponent::OnUpdate()
{
    if (HasOnUpdate() && IsEnabled())
    {
        sol::protected_function_result update_result = (*update_ref)(component_ref);

        if (!update_result.valid())
            ReportError(actor.lock()->name, update_result);
    }
}


inline void LuaComponent::OnLateUpdate()
{
    if (HasOnLateUpdate() && IsEnabled())
    {
        sol::protected_function_result late_update_result = (*late_update_ref)(component_ref);

        if (!late_update_result.valid())
            ReportError(actor.lock()->name, late_update_result);
    }
}


inline void LuaComponent::OnFixedUpdate()
{
    if (HasOnFixedUpdate() && IsEnabled())
    {
        sol::protected_function_result fixed_update_result = (*fixed_update_ref)(component_ref);

        if (!fixed_update_result.valid())
            ReportError(actor.lock()->name, fixed_update_result);
    }
}


inline void LuaComponent::OnDestroy()
{
    if (HasOnDestroy())
    {
        sol::protected_function_result destroy_result = (*on_destroy_ref)(component_ref);

        if (!destroy_result.valid())
            ReportError(actor.lock()->name, destroy_result);
    }
}


inline void LuaComponent::SetEnabled(bool enabled)                      { component_ref.as<sol::lua_table>()["enabled"] = enabled; }


inline bool LuaComponent::IsEnabled() const                             { return component_ref.as<sol::lua_table>()["enabled"].get<bool>(); }


inline bool LuaComponent::HasLuaFunction(const std::string &function_name)
{
    return component_ref.as<sol::lua_table>()[function_name].is<sol::function>();
}

#endif /* LuaComponent_h */
