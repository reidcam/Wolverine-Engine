# cmake build system for wolverine engine
# written by Jacob Robinson <mrjacob@umich.edu>
# 7/16/2024

cmake_minimum_required(VERSION 3.30)

project("wolverine-engine-demo")

# Some quick settings
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED="NO" CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF)

# GLOB all of our files
file(GLOB FIRST_PARTY "src/FirstParty/src/*.cpp" "src/FirstParty/*.h")

# Get all of our games resources
# configs
file(GLOB CONFIG_FILES "resources/*.config")
# images
file(GLOB_RECURSE IMAGE_FILES
        "resources/images/*.png"
        "resources/images/*.jpeg"
)

# Add executable
add_executable("${PROJECT_NAME}" 
        ${FIRST_PARTY}
        ${CONFIG_FILES}
        ${IMAGE_FILES}
)
target_include_directories("${PROJECT_NAME}" PRIVATE "src/FirstParty")

# Add Lua library
file(GLOB LUA_SRC "src/ThirdParty/lua-5.4.6/*.c")
add_library("${PROJECT_NAME}_lua" ${LUA_SRC})
target_include_directories("${PROJECT_NAME}_lua" PUBLIC "src/ThirdParty/lua-5.4.6")

# Add Sol includes (header only library)
target_include_directories("${PROJECT_NAME}" PUBLIC "src/ThirdParty/sol2-3.3.0")

# Add RapidJSON includes (header only library)
target_include_directories("${PROJECT_NAME}" PUBLIC "src/ThirdParty/rapidjson-1.1.0/include/rapidjson")

# Add box2d library
file(GLOB BOX2D_SOURCES
        "src/ThirdParty/box2d-2.4.1/src/collision/*.cpp"
        "src/ThirdParty/box2d-2.4.1/src/common/*.cpp"
        "src/ThirdParty/box2d-2.4.1/src/dynamics/*.cpp"
        "src/ThirdParty/box2d-2.4.1/src/rope/*.cpp"
)
add_library("${PROJECT_NAME}_box2d" ${BOX2D_SOURCES})
target_include_directories("${PROJECT_NAME}_box2d"
        PUBLIC
        "src/ThirdParty/box2d-2.4.1"
        "src/ThirdParty/box2d-2.4.1/src"
        PRIVATE
        "src/ThirdParty/box2d-2.4.1/box2d"
        "src/ThirdParty/box2d-2.4.1/src/dynamics"
)

if(APPLE)
        # # Add all of the game resources to the mac bundle
        # foreach(FILE ${IMAGE_FILES})
        #         file(RELATIVE_PATH NEW_FILE "resources/images" ${FILE})
        #         get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        #         set_source_files_properties(${FILE} PROPERTIES
        #                 MACOSX_PACKAGE_LOCATION "Resources/images/${NEW_FILE_PATH}"
        #         )
        # endforeach()

        # Make target a .app executable
        set_target_properties("${PROJECT_NAME}" PROPERTIES
                MACOSX_BUNDLE true     
                # Bundle resources
                RESOURCE "${IMAGE_FILES}"   
        )
endif()


# Sets the C++ version
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20 c_std_17)
target_compile_features("${PROJECT_NAME}_lua" PRIVATE cxx_std_20 c_std_17)
target_compile_features("${PROJECT_NAME}_box2d" PRIVATE cxx_std_20 c_std_17)

# Add third party libraries
add_subdirectory("src/ThirdParty/glm")
add_subdirectory("src/ThirdParty/SDL2-2.30.5")
add_subdirectory("src/ThirdParty/SDL2_image-2.8.2")
add_subdirectory("src/ThirdParty/SDL2_ttf-2.22.0")
add_subdirectory("src/ThirdParty/SDL2_mixer-2.8.0")

# Link everything together
target_link_libraries("${PROJECT_NAME}"
        PRIVATE
        "${PROJECT_NAME}_box2d"
        "${PROJECT_NAME}_lua"
        glm
        SDL2::SDL2-static
        SDL2_image::SDL2_image-static
        SDL2_ttf::SDL2_ttf-static
        SDL2_mixer::SDL2_mixer-static
)