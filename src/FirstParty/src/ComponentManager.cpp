//
//  ComponentManager.cpp
//  wolverine_engine
//
//  Created by Jacob Robinson on 6/4/24.
//  Manages Components
//

#include <stdio.h>

#include "ComponentManager.h"

/**
 * Initializes the component manager
*/
void ComponentManager::init()
{
    l_state = new sol::state();
    l_state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
}
