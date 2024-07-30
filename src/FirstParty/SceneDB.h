//
//  SceneDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 7/10/24.
//  Provides access to all of the scene paths
//

#ifndef SceneDB_h
#define SceneDB_h

#include <string>

/**
 * Loads all of the scene paths in the resources/scenes directory
*/
void LoadScenePaths();

/**
 * Get a scene path based on the name of the scene
 *
 *`@param   scene_name      the name of the scene path to get from the database
 * @returns                 the scene path with the specified name
*/
std::string GetScenePath(std::string scene_name);

#endif /* SceneDB_h */
