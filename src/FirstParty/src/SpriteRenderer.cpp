//
//	SpriteRenderer.cpp
//	wolverine_engine
//
//	Created by Nicholas Way on 7/10/24.
//	Defines the Sprite Renderer component

#include "SpriteRenderer.h"

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
    RendererData::DrawImageEx(sprite, x, y, rotation, scale_x, scale_y, pivot_x, pivot_y, color[0], color[1], color[2], color[3], sorting_order);
}

/*
* Lifecycle function for destroying the sprite renderer
*/
void SpriteRenderer::OnDestroy()
{
}
