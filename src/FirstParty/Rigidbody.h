//
//	Rigidbody.h
//	wolverine_engine
//
//	Created by Cameron Reid on 7/2/24.
//	Defines the rigidbody component

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <string>

#include "glm/glm.hpp"
#include "box2d/box2d.h"
#include "PhysicsWorld.h"

class Actor; // forward declation

class Rigidbody
{
public:
	b2Body* body = nullptr;

	// Rigidbody parameters
	float x = 0.0f;
	float y = 0.0f;
	std::string body_type = "dynamic";
	bool precise = true;
	float gravity_scale = 1.0f;
	float density = 1.0f;
	float angular_friction = 0.3f;
	float rotation = 0.0f; // units of degrees (not radians), will need to convert to radians for box2D

	// collider/trigger properties
	bool has_collider = true;
	bool has_trigger = true;
	std::string collider_type = "box";
	std::string trigger_type = "box";
	float collider_width = 1.0f;
	float collider_height = 1.0f;
	float collider_radius = 0.5f;
	float trigger_width = 1.0f;
	float trigger_height = 1.0f;
	float trigger_radius = 0.5f;
	float friction = 0.3f;
	float bounciness = 0.3f;


	// component parameters
	std::string type = "Rigidbody";
	std::string key = "???";
	Actor* actor = nullptr;
    bool REMOVED_FROM_ACTOR = false;
	bool enabled = true;

	/*
	* Lifecycle function for initializing the rigidbody
	*/
	void OnStart();

	/*
	* Lifecycle function for destroying the rigidbody
	*/
	void OnDestroy();

	// Setter functions

	/*
	* Adds force to the center of the rigidbody
	*
	* @parameter	force		the force to add to the rb
	*/
	void AddForce(const b2Vec2& force);

	/*
	* Sets the velocity of the rigidbody
	*
	* @parameter	velocity	the new velocity of the rb
	*/
	void SetVelocity(const b2Vec2& velocity);

	/*
	* Sets the position of the rigidobdy
	*
	* @parameter	position	the new position of the rb
	*/
	void SetPosition(const b2Vec2& position);

	/*
	* Sets the rotation of the rigidbody
	*
	* @parameter	degrees_clockwise    the number of degrees clockwise to rotate the rb
	*/
	void SetRotation(const float degrees_clockwise);

	/*
	* Sets the angular velocity of the rigidbody
	*
	* @parameter	degrees_clockwise    the number of degrees clockwise to rotate the rb
	*/
	void SetAngularVelocity(const float degrees_clockwise);

	/*
	* Set the gravity scale of the rigidbody
	*
	* @parameter	scale		the new gravity scale
	*/
	void SetGravityScale(const float scale);

	/*
	* Sets the upwards direction of the rigidbody
	*
	* @parameter	direction	the new upwards direction of the rb
	*/
	void SetUpDirection(b2Vec2 direction);

	/*
	* Sets the right direction of the rigidbody
	*
	* @parameter	direction	the new right direction of the rb
	*/
	void SetRightDirection(b2Vec2 direction);

	// Getter functions

	/*
	* Get the (x, y) position of the rigidbody
	*
	* @returns      a b2Vec2 containing the (x, y) position of the rb
	*/
	b2Vec2 GetPosition();

	/*
	* Get the current rotation of the rigidbody
	*
	* @returns		a float of the current rotation of the rb in clockwise degrees
	*/
	float GetRotation();

	/*
	* Returns the current velocity of the rigidbody
	*
	* @returns		a b2Vec2 containing the current velocity of the rb separated
	*				into its x and y components
	*/
	b2Vec2 GetVelocity();

	/*
	* Returns the current angular velocity of the rigidbody
	*
	* @returns		a b2Vec2 containing the current angular velocity of the rb separated
	*				into its x and y components
	*/
	float GetAngularVelocity();

	/*
	* Returns the current gravity scale of the rigidbody
	*
	* @returns		a float of the current gravity scale of the rb
	*/
	float GetGravityScale();

	/*
	* Returns the current upwards direction of rigidbody
	*
	* @returns		a b2Vec2 containing the current upwards direction of the rb
	*				split into its (x, y) components
	*/
	b2Vec2 GetUpDirection();

	/*
	* Returns the current right direction of rigidbody
	*
	* @returns		a b2Vec2 containing the current right direction of the rb
	*				split into its (x, y) components
	*/
	b2Vec2 GetRightDirection();
};

#endif /* Rigidbody.h */
