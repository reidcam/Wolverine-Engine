//
//  Component.h
//  Wolverine Engine
//
//  Created by Nicholas Way on 6/4/24
//  Adapted from Rahul Unniyampath
//

#ifndef Component_h
#define Component_h

#include "document.h"
#include "lua.hpp"
#include "sol/sol.hpp"

#include <memory>
#include <string>


class Actor;  // Forward declaration


class Component : public std::enable_shared_from_this<Component> {

    friend class ComponentManager;

public:

    virtual std::shared_ptr<Component> Clone() const = 0;

    virtual std::shared_ptr<Component> GetSharedPointer() = 0;

    virtual void UpdateComponentWithJSON(const rapidjson::Value &component_json) = 0;

    virtual void SetActor(std::weak_ptr<Actor> &actor);

    virtual void OnStart() = 0;

    virtual void OnUpdate() = 0;

    virtual void OnLateUpdate() = 0;

    virtual void OnFixedUpdate() = 0;

    // virtual void OnCollisionEnter(const CollisionData &collision) = 0;

    // virtual void OnCollisionExit(const CollisionData &collision) = 0;

    // virtual void OnTriggerEnter(const CollisionData &collision) = 0;

    // virtual void OnTriggerExit(const CollisionData &collision) = 0;

    virtual void OnDestroy() = 0;

    std::string GetComponentType() const;

    virtual void SetComponentKey(const std::string &key);

    std::string GetComponentKey() const;

    sol::object GetComponentRef() const;

    virtual void SetEnabled(bool enabled) = 0;

    virtual bool IsEnabled() const = 0;

    bool HasOnStart() const;

    bool HasOnUpdate() const;

    bool HasOnLateUpdate() const;

    bool HasOnFixedUpdate() const;

    bool HasOnCollisionEnter() const;

    bool HasOnCollisionExit() const;

    bool HasOnTriggerEnter() const;

    bool HasOnTriggerExit() const;

    bool HasOnDestroy() const;

    bool operator<(const Component &other) const;

    bool operator>(const Component &other) const;

protected:

    explicit Component();

    explicit Component(const Component &other);

    explicit Component(const std::string &type);

    std::string type;

    std::string key;

    sol::object component_ref;

    std::weak_ptr<Actor> actor;

    bool has_on_start = false;

    bool has_on_update = false;

    bool has_on_late_update = false;

    bool has_on_fixed_update = false;

    bool has_on_destroy = false;

    bool has_on_collision_enter = false;

    bool has_on_collision_exit = false;

    bool has_on_trigger_enter = false;

    bool has_on_trigger_exit = false;

};


inline Component::Component() {}


inline Component::Component(const Component &other)
    :   type(other.type),
        key(other.key),
        has_on_start(other.has_on_start),
        has_on_update(other.has_on_update),
        has_on_late_update(other.has_on_late_update),
        has_on_collision_enter(other.has_on_collision_enter),
        has_on_collision_exit(other.has_on_collision_exit),
        has_on_trigger_enter(other.has_on_trigger_enter),
        has_on_trigger_exit(other.has_on_trigger_exit),
        has_on_destroy(other.has_on_destroy) {}


inline Component::Component (const std::string &type) : type(type) {}

/**
* @brief Set the actor that the component is attached to
*/
inline void Component::SetActor(std::weak_ptr<Actor> &actor) { this->actor = actor; }


inline std::string Component::GetComponentType() const          { return type; }


inline void Component::SetComponentKey(const std::string &key)  { this->key = key; }


inline std::string Component::GetComponentKey() const           { return key; }


inline sol::object Component::GetComponentRef() const    { return component_ref; }


inline bool Component::HasOnStart() const                       { return this->has_on_start; }


inline bool Component::HasOnUpdate() const                      { return this->has_on_update; }


inline bool Component::HasOnLateUpdate() const                  { return this->has_on_late_update; }


inline bool Component::HasOnFixedUpdate() const                 { return this->has_on_fixed_update; }


inline bool Component::HasOnCollisionEnter() const                     { return has_on_collision_enter; }


inline bool Component::HasOnCollisionExit() const                      { return has_on_collision_exit; }


inline bool Component::HasOnTriggerEnter() const                       { return has_on_trigger_enter; }


inline bool Component::HasOnTriggerExit() const                        { return has_on_trigger_exit; }


inline bool Component::HasOnDestroy() const                            { return this->has_on_destroy; }


inline bool Component::operator<(const Component &other) const         { return key < other.key; }


inline bool Component::operator>(const Component &other) const         { return key > other.key; }

/**
* @brief comparator for sorting components in ascending order
*/
struct ComponentPtrCompAsc {
    bool operator()(const std::shared_ptr<Component> &lhs, const std::shared_ptr<Component> &rhs) { return *lhs < *rhs; }
};

/**
* @brief comparator for sorting components in descending order
*/
struct ComponentPtrCompDesc {
    bool operator()(const std::shared_ptr<Component> &lhs, const std::shared_ptr<Component> &rhs) { return *lhs > *rhs; }
};

#endif /* Component_h */
