//
//  ImageDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/21/24.
//  Provides access to all of the images being used in the game.

#ifndef ImageDB_h
#define ImageDB_h

#include <string>

#include "SDL.h"
#include "SDL_image.h"

/**
 * Loads all of the images in the resources/images directory
*/
void LoadImages();

/**
 * Get an image based on the images name
 *
 *`@param   image_name  the name of the image to get from the database
 * @returns             the image with the specified name
*/
SDL_Texture* GetImage(std::string image_name);

#endif /* ImageDB_h */
