//
//  TemplateDB.h
//  wolverine_engine
//
//  Created by Jacob Robinson on 5/22/24.
//  Provides access to all of the templates being used in the game.
//

#ifndef TemplateDB_h
#define TemplateDB_h

#include "EngineUtils.h"

/**
 * Loads all of the templates in the resources/actor_templates directory
*/
void LoadTemplates();

/**
 * Get a template based on the templates name
 *
 *`@param   template_name   the name of the template to get from the database
 * @returns                 the template with the specified name
*/
rapidjson::Document* GetTemplate(std::string template_name);

#endif /* TemplateDB_h */
