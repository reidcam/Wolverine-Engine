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
#include "Renderer.h"

class CollisionDetector;

// Used to draw the rigidbody colliders for debugging
// No comments because none of this needs to be exposed to devs using the engine
class DebugDraw : public b2Draw
{
public:
    inline void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
    {
        for (int i = 0; i < vertexCount; i++)
        {
            b2Vec2 vertex1 = *(vertices + i);
            for (int j = 0; j < vertexCount; j++)
            {
                b2Vec2 vertex2 = *(vertices + j);
                RendererData::DrawLine(vertex1.x, vertex1.y, vertex2.x, vertex2.y, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
            }
        }
    };
    
    // Not used so no need to fill function(s)
    inline void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) { DrawPolygon(vertices, vertexCount, color); };
    
    inline void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) 
    {
        for (int i = 1; i < 360; i++)
        {
            b2Vec2 vertex1(cos(i - 1) * radius, sin(i - 1) * radius); // the coordinates of the previous point on the circle
            b2Vec2 vertex2(cos(i) * radius, sin(i) * radius); // the coordinates of the current point on the circle
            
            // the points are now centered on the center of the circle
            vertex1 = vertex1 + center;
            vertex2 = vertex2 + center;
            
            RendererData::DrawLine(vertex1.x, vertex1.y, vertex2.x, vertex2.y, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
        }
    };
    
    // Not used so no need to fill function(s)
    inline void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) { DrawCircle(center, radius, color); };
    inline void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {};
    inline void DrawTransform(const b2Transform& xf) {};
    inline void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {};
};

class PhysicsWorld
{
public:
	inline static bool world_initialized = false;
	inline static CollisionDetector* collision_detector;
	inline static b2World* world;
	inline static float time_step = 1.0f / 60.0f;
    inline static DebugDraw ddraw;

	/**
	* Initializes the b2World if it has not already been initialized
	*/
	inline static void InitializeWorld() {
		if (!PhysicsWorld::world_initialized) { // initialize physics world
			PhysicsWorld::world = new b2World(b2Vec2(0.0f, 9.8f));
			PhysicsWorld::world_initialized = true;
            
            // Sets the debug draw for the physics world IF draw_debug is true in the renderer
            if (RendererData::GetDebugDraw())
            {
                ddraw.SetFlags(b2Draw::e_shapeBit); // Tells the debug drawer to draw shapes
                PhysicsWorld::world->SetDebugDraw(&ddraw);
            }
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
