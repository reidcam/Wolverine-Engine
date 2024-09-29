//
//  EngineUtils.cpp
//  wolverine-engine-demo
//
//  Created by Jacob Robinson on 9/28/24.
//  Static utility functions to be used around the engine.
//

#include <stdio.h>

#include "EngineUtils.h"
#include "LuaAPI.h"

/**
 * Reads a json file located at `path` into `out_document`
 */
void EngineUtils::ReadJsonFile(const std::string& path, rapidjson::Document& out_document) {
    FILE* file_pointer = nullptr;
#ifdef _WIN32
    fopen_s(&file_pointer, path.c_str(), "rb");
#else
    file_pointer = fopen(path.c_str(), "rb");
#endif
    char buffer[65536];
    rapidjson::FileReadStream stream(file_pointer, buffer, sizeof(buffer));
    out_document.ParseStream(stream);
    std::fclose(file_pointer);

    if (out_document.HasParseError()) {
        std::cout << "error parsing json at [" << path << "]";
        exit(0);
    }
}

/**
 * Reads 'in_document' into the json file located at 'path'
 */
void EngineUtils::WriteJsonFile(const std::string& path, rapidjson::Document& in_document) {
    FILE* file_pointer = nullptr;
#ifdef _WIN32
    fopen_s(&file_pointer, path.c_str(), "wb");
#else
    file_pointer = fopen(path.c_str(), "wb");
#endif
    char buffer[65536];
    rapidjson::FileWriteStream stream(file_pointer, buffer, sizeof(buffer));
    
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(stream);
    in_document.Accept(writer);
    
    std::fclose(file_pointer);
}

/**
 * combines two JSON documents into `out_document`
 * if both documents contain a copy of a given member, priority is given to document 1.
 *
 * Written by Jacob Robinson, 5/23/24
 */
void EngineUtils::CombineJsonDocuments(rapidjson::Document& d1, rapidjson::Document& d2, rapidjson::Document& out_document)
{
    // I know this function is kinda scuffed, don't worry about it. Fix only if causing performance issues.
    // Ensures that the out_document is a blank json object
    out_document.SetObject();
    
    // Parse and copy the values from d2 to out_document:
    for (rapidjson::Value::ConstMemberIterator itr = d2.MemberBegin(); itr != d2.MemberEnd(); itr++)
    {
        std::string member_name = itr->name.GetString();
        rapidjson::Value json_member_name(member_name.c_str(), (int)member_name.size(), out_document.GetAllocator());
        // If the value is an array check to combine the arrays, otherwise just copy the values.
        if (itr->value.IsObject())
        {
            // If d1 also has this array, combine the values
            if (d1.HasMember(member_name.c_str()))
            {
                // Double check to make sure the member is actually an array, then recursively combine the arrays
                if (d1[member_name.c_str()].IsObject())
                {
                    rapidjson::Document array;
                    
                    rapidjson::Document lhs;
                    lhs.CopyFrom(d1[member_name.c_str()], d1.GetAllocator());
                    rapidjson::Document rhs;
                    rhs.CopyFrom(d2[member_name.c_str()], d2.GetAllocator());
                    
                    CombineJsonDocuments(lhs, rhs, array);
                    
                    out_document.AddMember(json_member_name, array, array.GetAllocator());
                }
            }
            else
            {
                // Otherwise, just add the d2 array
                rapidjson::Document array;
                array.CopyFrom(d2[member_name.c_str()], d2.GetAllocator());
                out_document.AddMember(json_member_name, array, array.GetAllocator());
            }
        }
        else if (itr->value.IsBool())
        {
            // If d1 has a value of the same name and type than use it instead of d2's value
            bool b = itr->value.GetBool();
            if (d1.HasMember(member_name.c_str()) && d1[member_name.c_str()].IsBool())
            {
                b = d1[member_name.c_str()].GetBool();
            }
            
            out_document.AddMember(json_member_name, b, out_document.GetAllocator());
        }
        else if (itr->value.IsString())
        {
            // If d1 has a value of the same name and type than use it instead of d2's value
            std::string s = itr->value.GetString();
            if (d1.HasMember(member_name.c_str()) && d1[member_name.c_str()].IsString())
            {
                s = d1[member_name.c_str()].GetString();
            }
            
            rapidjson::Value json_value(s.c_str(), (int)s.size(), out_document.GetAllocator());
            out_document.AddMember(json_member_name, json_value, out_document.GetAllocator());
        }
        else if (itr->value.IsNumber())
        {
            // If d1 has a value of the same name and type than use it instead of d2's value
            double num = itr->value.GetDouble();
            if (d1.HasMember(member_name.c_str()) && d1[member_name.c_str()].IsNumber())
            {
                num = d1[member_name.c_str()].GetDouble();
            }
            
            // Truncates the value if needed
            if (std::abs(static_cast<int>(num) - num) < 1e-10)
            {
                out_document.AddMember(json_member_name, static_cast<int>(num), out_document.GetAllocator());
            }
            else
            {
                out_document.AddMember(json_member_name, num, out_document.GetAllocator());
            }
        }
    }
    
    // Parse and copy the values from d1 to out_document:
    // This part is much simpler since we know all duplicates are already in the out_document
    for (rapidjson::Value::ConstMemberIterator itr = d1.MemberBegin(); itr != d1.MemberEnd(); itr++)
    {
        std::string member_name = itr->name.GetString();
        rapidjson::Value json_member_name(member_name.c_str(), (int)member_name.size(), out_document.GetAllocator());
        
        // Checks if the member is already in out_document, and skips it if so.
        if (out_document.HasMember(member_name.c_str()))
        {
            continue;
        }
        
        // Add each unique object from d1 into out_document
        if (itr->value.IsObject())
        {
            rapidjson::Document array;
            array.CopyFrom(d1[member_name.c_str()], d1.GetAllocator());
            out_document.AddMember(json_member_name, array, array.GetAllocator());
        }
        else if (itr->value.IsBool())
        {
            bool b = itr->value.GetBool();
            out_document.AddMember(json_member_name, b, out_document.GetAllocator());
        }
        else if (itr->value.IsString())
        {
            std::string s = itr->value.GetString();
            rapidjson::Value json_value(s.c_str(), (int)s.size(), out_document.GetAllocator());
            out_document.AddMember(json_member_name, json_value, out_document.GetAllocator());
        }
        else if (itr->value.IsNumber())
        {
            double num = itr->value.GetDouble();
            
            // Truncates the value if needed
            if (std::abs(static_cast<int>(num) - num) < 1e-10)
            {
                out_document.AddMember(json_member_name, static_cast<int>(num), out_document.GetAllocator());
            }
            else
            {
                out_document.AddMember(json_member_name, num, out_document.GetAllocator());
            }
        }
    }
}

/**
 * Loads the data from JSON into an existing lua value
 * DO NOT USE: This function is for use inside of the scene and actor managers only.
 *
 * @param   value    the lua value that will store the given data
 * @param   data     the JSON that will be processed into the table
 * @param   type     the intended type of the lua value
*/
void EngineUtils::JsonToLuaObject(sol::lua_value& value, const rapidjson::Value& data, sol::type type)
{
    // Adds the property to the component
    if (type == sol::type::string) { value = data.GetString(); }
    else if (type == sol::type::boolean) { value = data.GetBool(); }
    else if (type == sol::type::number)
    {
        if (data.IsDouble()) { value = data.GetDouble(); }
        else { value = data.GetInt(); }
    }
    else if (type == sol::type::table)
    {
        sol::table _table = LuaAPI::GetLuaState()->create_table();
        _table[0] = sol::object(*LuaAPI::GetLuaState());
        
        // Add all of the values in the data to our new table.
        int i = 1;
        for (rapidjson::Value::ConstMemberIterator itr = data.MemberBegin(); itr != data.MemberEnd(); itr++)
        {
            const rapidjson::Value& _data = itr->value;
            
            sol::lua_value _value = "";
            
            // Determine the type of value that is being added to the table
            sol::type _type = sol::type::none;
            if (itr->value.IsString()) { _type = sol::type::string; }
            else if (itr->value.IsBool()) { _type = sol::type::boolean; }
            else if (itr->value.IsInt() || itr->value.IsDouble()) { _type = sol::type::number; }
            else if (itr->value.IsObject()) { _type = sol::type::table; }
            
            JsonToLuaObject(_value, _data, _type);
            
            // Add the value to the new table
            _table[i] = _value;
            i++;
        }
        value = _table;
    }
    else
    {
        // If the sol type is not any of the above, determine the object type based off of the json
        sol::type second_type;
        if (data.IsString()) { second_type = sol::type::string; }
        else if (data.IsBool()) { second_type = sol::type::boolean; }
        else if (data.IsNumber()) { second_type = sol::type::number; }
        else if (data.IsObject()) { second_type = sol::type::table; }
        JsonToLuaObject(value, data, second_type);
    }
}

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
std::string EngineUtils::LuaObjectToJson(rapidjson::Value& value, const sol::lua_value& data, rapidjson::Document::AllocatorType& allocator)
{
    // Adds the property to the component
    if (data.is<std::string>()) { value.SetString(data.as<std::string>().c_str(), allocator); }
    else if (data.is<bool>()) { value.SetBool(data.as<bool>()); }
    else if (data.is<int>()) { value.SetInt(data.as<int>()); }
    else if (data.is<float>()) { value.SetFloat(data.as<float>()); }
    else if (data.is<double>()) { value.SetFloat(data.as<double>()); }
    else if (data.value().get_type() == sol::type::table)
    {
        value.SetObject(); // Set our value as an object
        
        // If the table is an instance then we need to iterate through that to get access to all of the values, since Lua doesnt show us metatable values unless they have been accessed recently, which we can't assume.
        sol::table table = data.as<sol::table>();
        sol::table metatable = table;
        if (data.as<sol::table>()[sol::metatable_key].valid())
        {
            metatable = data.as<sol::table>()[sol::metatable_key]["__index"];
        }
        
        for (auto& itr : metatable)
        {
            // The key that corresponds to the itr in the table
            sol::lua_value key = itr.first;
            
            // Stores the key as a converted string
            std::string key_string = "???";
            
            // Converts the key to a string from its lua_type for use as a json object key
            if (key.is<std::string>()) { key_string = key.as<std::string>(); }
            else if (key.is<bool>()) { key_string = key.as<bool>() ? "true" : "false"; }
            else if (key.is<int>() || key.is<float>() || key.is<double>()) { key_string = to_string(key.as<int>()); }
            else if (key.is<sol::function>()) { key_string = "FUNCTION_KEY"; }
            else if (key.is<sol::table>()) { key_string = "TABLE_KEY"; }
            else { key_string = "UNKNOWN_KEY"; }
            
            
            // Add all of the values in the table to 'value'
            rapidjson::Value json_value;
            LuaObjectToJson(json_value, table[key], allocator);
            rapidjson::Value json_key;
            json_key.SetString(key_string.c_str(), allocator);
            
            value.AddMember(json_key, json_value, allocator);
        }
    }
    
    return "UNKNOWN_TYPE";
}

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
float EngineUtils::RandomNumber(float min, float max, int precision)
{
    // Random number engine
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int r_min = min * precision;
    int r_max = max * precision;
    
    // Define a uniform distribution
    std::uniform_int_distribution<> dis(r_min, r_max);
    
    // Generate and return the random number
    float num = dis(gen) / (float)precision;
    return num;
}
