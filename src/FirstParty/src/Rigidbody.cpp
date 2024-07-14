//
//	Rigidbody.cpp
//	wolverine_engine
//
//	Created by Cameron Reid on 7/2/24.
//	Defines the rigidbody component

#include "Rigidbody.h"

/*
* Lifecycle function for initializing a rigidbody
*/
void Rigidbody::OnStart()
{
	PhysicsWorld::InitializeWorld(); // initialize physics world

	b2BodyDef body_def;
	if (body_type == "dynamic")
		body_def.type = b2_dynamicBody;
	else if (body_type == "kinematic")
		body_def.type = b2_kinematicBody;
	else if (body_type == "static")
		body_def.type = b2_staticBody;

	body_def.position = b2Vec2(x, y);
	body_def.bullet = precise;
	body_def.angularDamping = angular_friction;
	body_def.gravityScale = gravity_scale;

	body = PhysicsWorld::world->CreateBody(&body_def);


	// set the shape
	b2Shape* collider_shape = nullptr;
	b2Shape* trigger_shape = nullptr;
	if (collider_type == "box") {
		b2PolygonShape* polygon_shape = new b2PolygonShape();
		polygon_shape->SetAsBox(collider_width * 0.5f, collider_height * 0.5f);
		collider_shape = polygon_shape;
	}
	else if (collider_type == "circle") {
		b2CircleShape* circle_shape = new b2CircleShape();
		circle_shape->m_radius = collider_radius;
		collider_shape = circle_shape;
	}

	if (trigger_type == "box") {
		b2PolygonShape* polygon_shape = new b2PolygonShape();
		polygon_shape->SetAsBox(trigger_width * 0.5f, trigger_height * 0.5f);
		trigger_shape = polygon_shape;
	}
	else if (trigger_type == "circle") {
		b2CircleShape* circle_shape = new b2CircleShape();
		circle_shape->m_radius = trigger_radius;
		trigger_shape = circle_shape;
	}

	// define the fixture
	b2FixtureDef fixure_def;
	fixure_def.shape = collider_shape;
	fixure_def.density = density;
	fixure_def.restitution = bounciness;
	fixure_def.friction = friction;


	// create the fixtures
	if (!has_collider && !has_trigger) {
		fixure_def.userData.pointer = reinterpret_cast<uintptr_t>(nullptr);
		fixure_def.isSensor = true; // phantom is a fake trigger
		body->CreateFixture(&fixure_def);
	}
	if (has_collider) {
		fixure_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixure_def.isSensor = false; // false bc collider
		body->CreateFixture(&fixure_def);
	}
	if (has_trigger) {
		fixure_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixure_def.shape = trigger_shape;
		fixure_def.isSensor = true; // true bc trigger
		body->CreateFixture(&fixure_def);
	}

	// must be called after the body is created
	SetRotation(rotation);

}

/*
* Lifecycle function for destroying the rigidbody
*/
void Rigidbody::OnDestroy()
{
    if (body == nullptr)
        return;
    
	PhysicsWorld::world->DestroyBody(body);
}

/*
* Adds force to the center of the rigidbody
*
* @parameter	force		the force to add to the rb
*/
void Rigidbody::AddForce(const b2Vec2& force)
{
	if (body == nullptr)
		return;

	body->ApplyForceToCenter(force, true);
}

/*
* Sets the velocity of the rigidbody
*
* @parameter	velocity	the new velocity of the rb
*/
void Rigidbody::SetVelocity(const b2Vec2& velocity)
{
	if (body == nullptr)
		return;

	body->SetLinearVelocity(velocity);
}

/*
* Sets the position of the rigidobdy
*
* @parameter	position	the new position of the rb
*/
void Rigidbody::SetPosition(const b2Vec2& position)
{
	if (body == nullptr) { // OnStart hasn't been called yet
		x = position.x;
		y = position.y;
	}
	else {
		float current_rotation = body->GetAngle();
		body->SetTransform(position, current_rotation);
	}
}

/*
* Sets the rotation of the rigidbody
*
* @parameter	degrees_clockwise    the number of degrees clockwise to rotate the rb
*/
void Rigidbody::SetRotation(const float degrees_clockwise)
{
	if (body == nullptr)
		rotation = degrees_clockwise;
	else {
		// convert to radians and apply
		body->SetTransform(body->GetPosition(), static_cast<int>(degrees_clockwise) * (b2_pi / 180.0f));
	}
}

/*
* Sets the angular velocity of the rigidbody
*
* @parameter	degrees_clockwise    the number of degrees clockwise to rotate the rb
*/
void Rigidbody::SetAngularVelocity(const float degrees_clockwise)
{
	if (body == nullptr) // no valid parameter to set before body is initialized
		return;

	// convert to radians
	body->SetAngularVelocity(degrees_clockwise * (b2_pi / 180.0f));
}

/*
* Set the gravity scale of the rigidbody
*
* @parameter	scale		the new gravity scale
*/
void Rigidbody::SetGravityScale(const float scale)
{
	if (body == nullptr)
		gravity_scale = scale;
	else
		body->SetGravityScale(scale);
}

/*
* Sets the upwards direction of the rigidbody
*
* @parameter	direction	the new upwards direction of the rb
*/
void Rigidbody::SetUpDirection(b2Vec2 direction)
{
	direction.Normalize();

	if (body == nullptr) {
		// get radians and then convert to degrees to store
		rotation = glm::atan(direction.x, -direction.y) * (180.0f / b2_pi);
	}
	else {
		float new_angle_radians = glm::atan(direction.x, -direction.y);
		body->SetTransform(body->GetPosition(), new_angle_radians);
	}
}

/*
* Sets the right direction of the rigidbody
*
* @parameter	direction	the new right direction of the rb
*/
void Rigidbody::SetRightDirection(b2Vec2 direction)
{
	direction.Normalize();

	if (body == nullptr) {
		// get radians and then convert to degrees to store
		rotation = (glm::atan(direction.x, -direction.y) - (b2_pi / 2.0f)) * (180.0f / b2_pi);
	}
	else {
		float new_angle_radians = glm::atan(direction.x, -direction.y) - (b2_pi / 2.0f);
		body->SetTransform(body->GetPosition(), new_angle_radians);
	}
}

/*
* Get the (x, y) position of the rigidbody
*
* @returns      a b2Vec2 containing the (x, y) position of the rb
*/
b2Vec2 Rigidbody::GetPosition()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return b2Vec2(x, y);

	return body->GetPosition();
}

/*
* Get the current rotation of the rigidbody
*
* @returns		a float of the current rotation of the rb in clockwise degrees
*/
float Rigidbody::GetRotation()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return rotation;

	// body->GetAngle() returns the value in radians, need to convert to degrees
	return body->GetAngle() * (180.0f / b2_pi);
}

/*
* Returns the current velocity of the rigidbody
*
* @returns		a b2Vec2 containing the current velocity of the rb separated
*				into its x and y components
*/
b2Vec2 Rigidbody::GetVelocity()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return b2Vec2(0.0f, 0.0f); // no velocity before init

	return body->GetLinearVelocity();
}

/*
* Returns the current angular velocity of the rigidbody
*
* @returns		a b2Vec2 containing the current angular velocity of the rb separated
*				into its x and y components
*/
float Rigidbody::GetAngularVelocity()
{
	if (body == nullptr)
		return 0.0f; // no velocity before init

	return body->GetAngularVelocity() * (180.0f / b2_pi);
}

/*
* Returns the current gravity scale of the rigidbody
*
* @returns		a float of the current gravity scale of the rb
*/
float Rigidbody::GetGravityScale()
{
	if (body == nullptr) // OnStart hasn't been called yet
		return gravity_scale;

	return body->GetGravityScale();
}

/*
* Returns the current upwards direction of rigidbody
*
* @returns		a b2Vec2 containing the current upwards direction of the rb
*				split into its (x, y) components
*/
b2Vec2 Rigidbody::GetUpDirection()
{
	b2Vec2 upDirection = b2Vec2(0.0f, 0.0f);

	if (body == nullptr) {
		float rotation_rad = rotation * (b2_pi / 180.0f);
		upDirection = b2Vec2(glm::sin(rotation_rad), -glm::cos(rotation_rad));
	}
	else
		upDirection = b2Vec2(glm::sin(body->GetAngle()), -glm::cos(body->GetAngle()));

	upDirection.Normalize();

	return upDirection;
}

/*
* Returns the current right direction of rigidbody
*
* @returns		a b2Vec2 containing the current right direction of the rb
*				split into its (x, y) components
*/
b2Vec2 Rigidbody::GetRightDirection()
{
	b2Vec2 rightDirection = b2Vec2(0.0f, 0.0f);

	if (body == nullptr) {
		float rotation_rad = rotation * (b2_pi / 180.0f);
		rightDirection = b2Vec2(glm::cos(rotation_rad), glm::sin(rotation_rad));
	}
	else
		rightDirection = b2Vec2(glm::cos(body->GetAngle()), glm::sin(body->GetAngle()));

	rightDirection.Normalize();

	return rightDirection;
}
