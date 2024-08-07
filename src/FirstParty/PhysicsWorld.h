//
//  PhysicsWorld.h
//  wolverine_engine
//
//  Created by Cameron Reid on 7/2/24.
//  Stores all of the information related to the Box2d b2World
//

#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "box2d/box2d.h"

class CollisionDetector;

class PhysicsWorld
{
public:
	inline static bool world_initialized = false;
	inline static CollisionDetector* collision_detector;
	inline static b2World* world;
	inline static float time_step = 1.0f / 60.0f;

	/**
	* Initializes the b2World if it has not already been initialized
	*/
	inline static void InitializeWorld() {
		if (!PhysicsWorld::world_initialized) { // initialize physics world
			PhysicsWorld::world = new b2World(b2Vec2(0.0f, 9.8f));
			PhysicsWorld::world_initialized = true;
		}
	};

	/**
	* advances the b2World by the specified time step
	*/
	inline static void AdvanceWorld() {
		if (PhysicsWorld::world_initialized)
			PhysicsWorld::world->Step(1.0f / 60.0f, 8, 3);
	};
};

#endif