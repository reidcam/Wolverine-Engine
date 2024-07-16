#ifndef COLLISION_H
#define COLLISION_H

#include "box2d/box2d.h"

class Actor; // forward declaration

class Collision
{
public:
	Actor* other;
	b2Vec2 point;
	b2Vec2 relative_velocity;
	b2Vec2 normal;
};
#endif