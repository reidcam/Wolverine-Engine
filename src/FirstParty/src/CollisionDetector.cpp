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

	collision.other_actor_id = actor_b->ID;
	collision.this_actor_id = actor_a->ID;

	// determine if we have 2 triggers or 2 colliders
	if (fixture_a->IsSensor() && fixture_b->IsSensor()) {
		collision.point = b2Vec2(-999.0f, -999.0f); // not valid for trigger
		collision.normal = b2Vec2(-999.0f, -999.0f); // not valid for trigger
		OnEnter::OnTriggerEnter(collision);

		collision.other_actor_id = actor_a->ID;
		collision.this_actor_id = actor_b->ID;

		OnEnter::OnTriggerEnter(collision);
	}
	else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
		OnEnter::OnCollisionEnter(collision);

		collision.other_actor_id = actor_a->ID;
		collision.this_actor_id = actor_b->ID;

		OnEnter::OnCollisionEnter(collision);
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

	collision.other_actor_id = actor_b->ID;
	collision.this_actor_id = actor_a->ID;

	// determine if we have 2 triggers or 2 colliders
	if (fixture_a->IsSensor() && fixture_b->IsSensor()) {
		OnEnter::OnTriggerExit(collision);

		collision.other_actor_id = actor_a->ID;
		collision.this_actor_id = actor_b->ID;

		OnEnter::OnTriggerExit(collision);
	}
	else if (!fixture_a->IsSensor() && !fixture_b->IsSensor()) {
		OnEnter::OnCollisionExit(collision);

		collision.other_actor_id = actor_a->ID;
		collision.this_actor_id = actor_b->ID;

		OnEnter::OnCollisionExit(collision);
	}
}

/*
* Called when contact begins begins between two triggers
*
* @parameters    collision    info about the collision
*/
void OnEnter::OnTriggerEnter(const Collision& collision)
{
	size_t number_of_components = Actors::GetNumberOfComponents(collision.this_actor_id);

	for (size_t i = 0; i < number_of_components; i++) {
		try {
			sol::table component = Actors::GetComponentByIndex(collision.this_actor_id, i);
			sol::function OnTriggerEnter = component["OnTriggerEnter"];
            
            // If this component is dead, skip it
            if ((component)["REMOVED_FROM_ACTOR"] == true)
            {
                continue;
            }

			// Skip this component if the actor or component aren't enabled
			if (!Actors::GetActorEnabled(collision.this_actor_id) || (component)["enabled"] == false)
			{
				continue;
			}

			if (OnTriggerEnter.valid())
			{
				OnTriggerEnter(component, collision);
			}
		}
		catch (const sol::error& e) {
			std::cerr << "Caught Lua runtime error: " << e.what() << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Caught C++ exception: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Caught unknown exception." << std::endl;
		}
	}
}

/*
* Called when contact ends between two triggers
*
* @parameters    collision    info about the collision
*/
void OnEnter::OnTriggerExit(const Collision& collision)
{
	size_t number_of_components = Actors::GetNumberOfComponents(collision.this_actor_id);

	for (size_t i = 0; i < number_of_components; i++) {
		try {
			sol::table component = Actors::GetComponentByIndex(collision.this_actor_id, i);
			sol::function OnTriggerExit = component["OnTriggerExit"];
            
            // If this component is dead, skip it
            if ((component)["REMOVED_FROM_ACTOR"] == true)
            {
                continue;
            }

			// Skip this component if the actor or component aren't enabled
			if (!Actors::GetActorEnabled(collision.this_actor_id) || (component)["enabled"] == false)
			{
				continue;
			}

			if (OnTriggerExit.valid())
			{
				OnTriggerExit(component, collision);
			}
		}
		catch (const sol::error& e) {
			std::cerr << "Caught Lua runtime error: " << e.what() << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Caught C++ exception: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Caught unknown exception." << std::endl;
		}
	}
}

/*
* Called when contact begins between two rigidbodies
*
* @parameter    collision    info about the collision
*/
void OnEnter::OnCollisionEnter(const Collision& collision)
{
	size_t number_of_components = Actors::GetNumberOfComponents(collision.this_actor_id);

	for (size_t i = 0; i < number_of_components; i++) {
		try {
			sol::table component = Actors::GetComponentByIndex(collision.this_actor_id, i);
			sol::function OnCollisionEnter = component["OnCollisionEnter"];
            
            // If this component is dead, skip it
            if ((component)["REMOVED_FROM_ACTOR"] == true)
            {
                continue;
            }

			// Skip this component if the actor or component aren't enabled
			if (!Actors::GetActorEnabled(collision.this_actor_id) || (component)["enabled"] == false)
			{
				continue;
			}

			if (OnCollisionEnter.valid())
			{
				OnCollisionEnter(component, collision);
			}
		}
		catch (const sol::error& e) {
			std::cerr << "Caught Lua runtime error: " << e.what() << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Caught C++ exception: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Caught unknown exception." << std::endl;
		}
	}
}

/*
* Called when contact ends between two rigidbodies
*
* @parameter    collision    info about the collision
*/
void OnEnter::OnCollisionExit(const Collision& collision)
{
	size_t number_of_components = Actors::GetNumberOfComponents(collision.this_actor_id);

	for (size_t i = 0; i < number_of_components; i++) {
		try {
			sol::table component = Actors::GetComponentByIndex(collision.this_actor_id, i);
			sol::function OnCollisionExit = component["OnCollisionExit"];
            
            // If this component is dead, skip it
            if ((component)["REMOVED_FROM_ACTOR"] == true)
            {
                continue;
            }

			// Skip this component if the actor or component aren't enabled
			if (!Actors::GetActorEnabled(collision.this_actor_id) || (component)["enabled"] == false)
			{
				continue;
			}

			if (OnCollisionExit.valid())
			{
				OnCollisionExit(component, collision);
			}
		}
		catch (const sol::error& e) {
			std::cerr << "Caught Lua runtime error: " << e.what() << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Caught C++ exception: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Caught unknown exception." << std::endl;
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
