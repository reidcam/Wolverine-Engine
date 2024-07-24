//
//	SpriteRenderer.h
//	wolverine_engine
//
//	Created by Nicholas Way on 7/5/24.
//	Defines the Sprite Renderer component

#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "SDL.h"
#include "SDL_image.h"

#include "Renderer.h"

struct Actor; // forward declation

class SpriteRenderer {
public:
    // image reference
    std::string sprite = "";
    
    // parameters
    float x = 0.0f;
    float y = 0.0f;
    int color[4] = {255, 255, 255, 255};
    float scale_x = 1.0f;
    float scale_y = 1.0f;
    float pivot_x = 0.5f;
    float pivot_y = 0.5f;
    float rotation = 0.0f; // units of degrees
    float sorting_order = 0.0f;


    // component parameters
    std::string type = "SpriteRenderer";
    std::string key = "???";
    Actor* actor = nullptr;
    bool REMOVED_FROM_ACTOR = false;
    bool enabled = true;

    /*
    * Lifecycle function for initializing the sprite renderer
    */
    void OnStart();
    
    /*
    * Lifecycle function for updating the sprite renderer
    */
    void OnUpdate();

    /*
    * Lifecycle function for destroying the sprite renderer
    */
    void OnDestroy();
};

#endif /* SpriteRenderer.h */
