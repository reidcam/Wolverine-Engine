//
//  ComponentManager.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 6/4/24.
//  Manages Components
//

#ifndef ComponentManager_h
#define ComponentManager_h

#include "sol/sol.hpp"

#include "ComponentDB.h"

class ComponentManager
{
private:
    static inline sol::state* l_state;
public:
    /**
     * Initializes the component manager
    */
    static void init();
    
    /**
     * Get the Lua state
    */
    static inline sol::state* GetLuaState() {return l_state;}
};

#endif /* ComponentManager_h */
