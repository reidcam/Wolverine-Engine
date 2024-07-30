//
//	SpriteRenderer.cpp
//	wolverine_engine
//
//	Created by Nicholas Way on 7/10/24.
//	Defines the Sprite Renderer component

#include "SpriteRenderer.h"
#include "ActorManager.h"
#include "PhysicsWorld.h"

/*
* Lifecycle function for initializing a sprite renderer
*/
void SpriteRenderer::OnStart()
{
}

/*
* Lifecycle function for updating the sprite renderer
*/
void SpriteRenderer::OnUpdate()
{
    // Sprite Renderer should render at the rigidbody's location if it exists
    sol::table rb = Actors::GetComponentByType(actor->ID, "Rigidbody");
    if (rb.valid())
    {
        sol::function GetPosition = (rb)["GetPosition"];
        b2Vec2 pos = GetPosition(rb);
        x = pos.x;
        y = pos.y;
    }
    
    RendererData::DrawImageEx(sprite, x, y, rotation, scale_x, scale_y, pivot_x, pivot_y, color[0], color[1], color[2], color[3], sorting_order);
}

/*
* Lifecycle function for destroying the sprite renderer
*/
void SpriteRenderer::OnDestroy()
{
}
