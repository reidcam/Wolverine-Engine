//
//  CollisionDectector.cpp
//  wolverine_engine
//
//  Created by Cameron Reid on 7/16/24.
//  Handles contact between physics bodies

#include "CollisionDetector.h"

/*
* Handles physics contacts when the contact first occurs
*
* @parameters    contact    a contact object containing the info about the contact
*/
void CollisionDetector::BeginContact(b2Contact* contact)
{
	b2Fixture* fixture_a = contact->GetFixtureA();
	b2Fixture* fixture_b = contact->GetFixtureB();
	uintptr_t userDataA = fixture_a->GetUserData().pointer;
	Actor* actor_a = reinterpret_cast<Actor*>(userDataA);
	uintptr_t userDataB = fixture_b->GetUserData().pointer;
	Actor* actor_b = reinterpret_cast<Actor*>(userDataB);

	// one of the colliders is a phantom fixture
	if (actor_a == nullptr || actor_b == nullptr)
		return;

	b2WorldManifold	world_manifold;
	contact->GetWorldManifold(&world_manifold);

	Collision collision;
	collision.point = world_manifold.points[0]; // get the first point of collision

	b2Vec2 relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();
	collision.relative_velocity = relative_velocity;

	collision.normal = world_manifold.normal;

	collision.other = actor_b;

	// determine if we have 2 triggers or 2 colliders
	if (fixture_a->IsSensor() && fixture_b->IsSensor()) {
		collision.point = b2Vec2(-999.0f, -999.0f); // not valid for trigger
		collision.normal = b2Vec2(-999.0f, -999.0f); // not valid for trigger
		actor_a->OnTriggerEnter(collision);

		collision.other = actor_a;

		actor_b->OnTriggerEnter(collision);
	}
	else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
		actor_a->OnCollisionEnter(collision);

		collision.other = actor_a;

		actor_b->OnCollisionEnter(collision);
	}
}

/*
* Handles physics contacts when the contact ends
*
* @parameters    contact    a contact object containing the info about the contact
*/
void CollisionDetector::EndContact(b2Contact* contact)
{
	b2Fixture* fixture_a = contact->GetFixtureA();
	b2Fixture* fixture_b = contact->GetFixtureB();
	uintptr_t userDataA = fixture_a->GetUserData().pointer;
	Actor* actor_a = reinterpret_cast<Actor*>(userDataA);
	uintptr_t userDataB = fixture_b->GetUserData().pointer;
	Actor* actor_b = reinterpret_cast<Actor*>(userDataB);

	// one of the colliders is a phantom fixture
	if (actor_a == nullptr || actor_b == nullptr)
		return;

	Collision collision;
	collision.point = b2Vec2(-999.0f, -999.0f); // not valid for exit

	b2Vec2 relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();
	collision.relative_velocity = relative_velocity;

	collision.normal = b2Vec2(-999.0f, -999.0f); // not valid for exit

	collision.other = actor_b;

	// determine if we have 2 triggers or 2 colliders
	if (fixture_a->IsSensor() && fixture_b->IsSensor()) {
		actor_a->OnTriggerExit(collision);

		collision.other = actor_a;

		actor_b->OnTriggerExit(collision);
	}
	else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
		actor_a->OnCollisionExit(collision);

		collision.other = actor_a;

		actor_b->OnCollisionExit(collision);
	}
}

/*
* Called when contact begins begins between two triggers
*
* @parameters    collision    info about the collision
*/
void OnEnter::OnTriggerEnter(const Collision& collision)
{
	std::vector<std::pair<std::string, Component>> name_and_component;

	// find all components on the actor that have OnTriggerEnter functions
	for (auto& component : components) {
		if ((*component.second.componentRef)["OnTriggerEnter"].isFunction())
			name_and_component.push_back(std::pair<std::string, Component>(component.first, component.second));
	}

	sort(name_and_component.begin(), name_and_component.end(), SortComponentsByKey());

	// call all of the OnTriggerEnter functions in order by key
	for (auto& component : name_and_component) {
		try {
			luabridge::LuaRef enabled = (*component.second.componentRef)["enabled"];

			if (enabled.cast<bool>() == true) {
				(*component.second.componentRef)["OnTriggerEnter"](*component.second.componentRef, collision);
			}
		}
		catch (const luabridge::LuaException& e) {
			ReportError(actor_name, e);
		}
	}
}

/*
* Catches and outputs lua errors from sol
* 
* @parameters    actor_id    the id of the actor that the error came from
* @parameters    e           the sol error
*/
void OnEnter::ReportError(const int actor_id, const sol::error& e)
{
	std::string error_message = e.what();

	// Normalize file paths across platforms
	std::replace(error_message.begin(), error_message.end(), '\\', '/');

	// Display (with color codes)
	std::cout << "\033[31m" << Actors::GetName(actor_id) << " : " << error_message << "\033[0m" << std::endl;
}