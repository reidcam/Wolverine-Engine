//
//  TextDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/21/24.
//  Provides access to all of the text elements being used in the game.

#ifndef TextDB_h
#define TextDB_h

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

/**
 * Loads all of the fonts in the resources/fonts directory
*/
void LoadFonts();

/**
 * Get an font based on its size and name
 *
 *`@param   font_name   the name of the font to get from the database
 * @param   font_size       the size of the font to get from the database
 * @returns             the font with the specified name and size
*/
TTF_Font* GetFont(std::string font_name, int font_size);

#endif /* TextDB_h */
