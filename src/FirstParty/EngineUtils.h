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
#include <filesystem>
#include <iostream>
#include <thread>
#include <random>

#include "prettywriter.h"
#include "stringbuffer.h"
#include "FileUtils.h"

class EngineUtils {
public:
    /**
     * Reads a json file located at `path` into `out_document`
     */
    static void ReadJsonFile(const std::string& path, rapidjson::Document& out_document) {
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
     * combines two JSON documents into `out_document`
     * if both documents contain a copy of a given member, priority is given to document 1.
     *
     * Written by Jacob Robinson, 5/23/24
     */
    static void CombineJsonDocuments(rapidjson::Document& d1, rapidjson::Document& d2, rapidjson::Document& out_document)
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
     * Generates a random number between the max and min values.
     * Precision is the number of decimal places you want to be randomized.
     *
     * @param   min the lower bound of our random number generation
     * @param   max the upper bound of our random number generation
     * @param   precision   used to calculate the decimal precision in the random number
     *
     * @returns           the random number between min and max
     */
    static float RandomNumber(float min, float max, int precision)
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
};

#endif /* EngineUtils_h */
