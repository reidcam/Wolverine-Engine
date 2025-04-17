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
#include "ShallowActorClass.h"

/**
 * Loads all of the templates in the resources/actor_templates directory
*/
void LoadTemplates();

/**
 * Get a template based on the templates name
 *
 * @param   template_name   the name of the template to get from the database
 * @returns                 the template with the specified name
*/
rapidjson::Document* GetTemplate(std::string template_name);

/**
 * Gets a shallow actor of the given template
 * @param   template_name   the name of the template to get from the database
 * @returns                A shallow actor of the specified template
 */
std::shared_ptr<ShallowActor> GetReferenceTemplate(std::string template_name);

/**
 * Get a list of all the names of all the possible templates
 *
 * @returns                   the list of template names
*/
std::vector<std::string> ListAllTemplateTypes();

#endif /* TemplateDB_h */
