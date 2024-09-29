//
// EngineUtils.h
// wolverine_engine
//
// Nicholas Way, 20 May 2024
// Static utility functions to be used around the engine.
//

#ifndef EngineUtils_h
#define EngineUtils_h

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include <filesystem>
#include <iostream>
#include <thread>
#include <random>

#include "prettywriter.h"
#include "stringbuffer.h"
#include "FileUtils.h"
#include "sol/sol.hpp"

class EngineUtils {
public:
    /**
     * Reads a json file located at `path` into `out_document`
     */
    static void ReadJsonFile(const std::string& path, rapidjson::Document& out_document);
    
    /**
     * Reads 'in_document' into the json file located at 'path'
     */
    static void WriteJsonFile(const std::string& path, rapidjson::Document& in_document);
    
    /**
     * combines two JSON documents into `out_document`
     * if both documents contain a copy of a given member, priority is given to document 1.
     *
     * Written by Jacob Robinson, 5/23/24
     */
    static void CombineJsonDocuments(rapidjson::Document& d1, rapidjson::Document& d2, rapidjson::Document& out_document);
    
    /**
     * Loads the data from JSON into an existing lua value
     * DO NOT USE: This function is for use inside of the scene and actor managers only.
     *
     * @param   value    the lua value that will store the given data
     * @param   data     the JSON that will be processed into the table
     * @param   type     the intended type of the lua value
    */
    static void JsonToLuaObject(sol::lua_value& value, const rapidjson::Value& data, sol::type type);
    
    /**
     * Loads the data from the Lua value and puts it into a json object
     * DO NOT USE: This function is for use inside of the game editor only.
     *
     * @param   value   the JSON that will store the given data
     * @param   data     the lua value that will be processed into the json
     * @param   allocator     the allocator for the highest level of json object that the value is going to be used for
     *
     * @returns       the type of the lua value that is being converted to json as a string
    */
    static std::string LuaObjectToJson(rapidjson::Value& value, const sol::lua_value& data, rapidjson::Document::AllocatorType& allocator);
    
    /**
     * Generates a random number between the max and min values.
     * Precision is the number of decimal places you want to be randomized.
     *
     * @param   min          the lower bound of our random number generation
     * @param   max          the upper bound of our random number generation
     * @param   precision    used to calculate the decimal precision in the random number
     *
     * @returns              the random number between min and max
     */
    static float RandomNumber(float min, float max, int precision);
};

#endif /* EngineUtils_h */
