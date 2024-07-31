//
//  CollisionDectector.h
//  wolverine_engine
//
//  Created by Cameron Reid on 7/16/24.
//  Handles contact between physics bodies

#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "box2d/box2d.h"
#include "ActorManager.h"
#include "Collision.h"

class CollisionDetector : public b2ContactListener
{
public:
	/*
	* Handles physics contacts when the contact first occurs
	* 
	* @parameters    contact    a contact object containing the info about the contact
	*/
	void BeginContact(b2Contact* contact);

	/*
	* Handles physics contacts when the contact ends
	*
	* @parameters    contact    a contact object containing the info about the contact
	*/
	void EndContact(b2Contact* contact);
};

class OnEnter
{
public:
	/*
	* Called when contact begins between two triggers
	* 
	* @parameters    collision    info about the collision
	*/
	static void OnTriggerEnter(const Collision& collision);

	/*
	* Called when contact ends between two triggers
	* 
	* @parameters    collision    info about the collision
	*/
	static void OnTriggerExit(const Collision& collision);

	/*
	* Called when contact begins between two rigidbodies
	* 
	* @parameter    collision    info about the collision
	*/
	static void OnCollisionEnter(const Collision& collision);

	/*
	* Called when contact ends between two rigidbodies
	*
	* @parameter    collision    info about the collision
	*/
	static void OnCollisionExit(const Collision& collision);

	/*
	* Catches and outputs lua errors from sol
	*
	* @parameters    actor_id    the id of the actor that the error came from
	* @parameters    e           the sol error
	*/
	static void ReportError(const int actor_id, const sol::error& e);
};
#endif