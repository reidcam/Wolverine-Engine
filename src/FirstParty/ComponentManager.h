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
    
    //-------------------------------------------------------
    // Native Components
    
    /**
     * Returns true if the given type is a C++ based component
     *
     *  @param type                          the type to confirm if its native or not
     *  @returns               true if the given type is a native component, and false otherwise
     */
    static bool IsComponentTypeNative(std::string type);
    
    /**
     * Creates and returns a new native component of the given type
     *
     *  @param component_type   the component type to create a new copy of
     *  @returns               the table that contains our new native component
     */
    static sol::table NewNativeComponent(std::string component_type);
};

#endif /* ComponentManager_h */
