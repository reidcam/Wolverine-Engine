//
//  ComponentDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 6/4/24.
//  Provides access to all of the lua component types being used in the game
//

#ifndef ComponentDB_h
#define ComponentDB_h

#include "sol/sol.hpp"

/**
 * Loads all of the components in the resources/component_types directory
*/
void LoadComponentTypes();

/**
 * Get a component type based on its name
 *
 * @param   component_name    the name of the component type to get from the database
 * @returns                   the component type with the specified name
*/
std::shared_ptr<sol::table> GetComponentType(std::string component_name);

#endif /* ComponentDB_h */
