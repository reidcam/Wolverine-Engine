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
public:
    /**
     * Establishes inheritance between two tables by setting one to be the metatable of the other
     *
     *  @param instance_table   the table to be inheriting from the parent table (passed by reference)
     *  @param parent_table        the table that is being inherited from (passed by reference)
     */
    static void EstablishInheritance(sol::table& instance_table, sol::table& parent_table);
};

#endif /* ComponentManager_h */
