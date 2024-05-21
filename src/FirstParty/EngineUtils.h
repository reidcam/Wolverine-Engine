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

class EngineUtils {
public:
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
    
    static bool DirectoryExists(const std::string& path) {
        return std::filesystem::exists(std::filesystem::path(path));
    }
};

#endif /* EngineUtils_h */
