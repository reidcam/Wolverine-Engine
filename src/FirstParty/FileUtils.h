//
//  FileUtilities.h
//  wolverine-engine-demo
//
//  Created by Jacob Robinson on 7/23/24.
//  Handles the different file paths on each system
//

#ifndef FileUtilities_h
#define FileUtilities_h

#include <filesystem>
#include <iostream>
#include <string>

#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif

using namespace std;

class FileUtils
{
private:
    static inline std::string working_directory = ""; // The working directory for the game
public:
    /**
     * Sets the working directory to be where our resources are (based on the current operating system)
    */
    static inline void SetWorkingDirectory()
    {
#if defined(_WIN32) || defined(_WIN64)
        // Windows
        working_directory = std::filesystem::current_path().string();
#elif defined(__APPLE__) && defined(__MACH__)
        // macOS
        // Credit for this solution goes to the people who responded to the thread https://stackoverflow.com/questions/516200/relative-paths-not-working-in-xcode-c
        CFBundleRef app_bundle = CFBundleGetMainBundle();
        CFURLRef resources_url = CFBundleCopyResourcesDirectoryURL(app_bundle);
        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resources_url, true, (UInt8*)path, PATH_MAX))
        {
            // error
        }
        CFRelease(resources_url);
        working_directory = path;
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
        // Linux
        working_directory = std::filesystem::current_path().string();
#else
        std::cerr << "Platform not supported!" << std::endl;
#endif
    }
    
    /**
     * Get the path of a directory or file relative to the current working directory
     *
     * @param   directory_name  the name of the directory or file to get the path of
     * @returns                the path of the given file or directory relative to the current working directory
    */
    static inline std::string GetPath(const std::string& directory_name)
    {
        std::string path = working_directory + "/" + directory_name;
        return path;
    }
    
    /**
     * Get the path of a directory or file relative to the current working directory
     *
     * @param   directory_name  the name of the directory or file to get the path of
     * @returns                the path of the given file or directory relative to the current working directory
    */
    static inline bool DirectoryExists(const std::string& path)
    {
        return std::filesystem::exists(std::filesystem::path(GetPath(path)));
    }
};

#endif /* FileUtilities_h */
