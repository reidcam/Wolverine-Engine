# cmake build system for wolverine engine
# written by Jacob Robinson <mrjacob@umich.edu>
# 7/16/2024

cmake_minimum_required(VERSION 3.30)

project("wolverine-engine-demo" VERSION 1.0)

# Some quick settings
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED="NO" CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_JSON_SOURCE_FILE_EXTENSIONS json;JSON;config;template;scene)

# GLOB all of our files
file(GLOB FIRST_PARTY "src/FirstParty/src/*.cpp" "src/FirstParty/*.h")

# Get all of our games resources
file(GLOB IMAGE_FILES
        "${CMAKE_SOURCE_DIR}/resources/images/*.png"
        "${CMAKE_SOURCE_DIR}/resources/images/*.jpeg"
)
file(GLOB AUDIO_FILES 
        "${CMAKE_SOURCE_DIR}/resources/audio/*.mp3"
        "${CMAKE_SOURCE_DIR}/resources/audio/*.wav"
)
file(GLOB FONT_FILES "${CMAKE_SOURCE_DIR}/resources/fonts/*.ttf")
file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/resources/*.config")
file(GLOB ACTOR_TEMPLATES "${CMAKE_SOURCE_DIR}/resources/actor_templates/*.template")
file(GLOB COMPONENT_TYPES "${CMAKE_SOURCE_DIR}/resources/component_types/*.lua")
file(GLOB SCENE_FILES "${CMAKE_SOURCE_DIR}/resources/scenes/*.scene")

# Add executable
add_executable("${PROJECT_NAME}" 
        ${FIRST_PARTY}
        # bundle resources
        ${IMAGE_FILES}
        ${AUDIO_FILES}
        ${FONT_FILES}
        ${CONFIG_FILES}
        ${ACTOR_TEMPLATES}
        ${COMPONENT_TYPES}
        ${SCENE_FILES}
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
        # Add all of the game resources to the mac bundle
        foreach(FILE ${IMAGE_FILES})
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources/images"
                )
        endforeach()
        foreach(FILE ${AUDIO_FILES})
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources/audio"
                )
        endforeach()
        foreach(FILE ${FONT_FILES})
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources/fonts"
                )
        endforeach()
        foreach(FILE ${CONFIG_FILES})
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources"
                )
        endforeach()
        foreach(FILE ${ACTOR_TEMPLATES})
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources/actor_templates"
                )
        endforeach()
        foreach(FILE ${COMPONENT_TYPES})
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources/component_types"
                )
        endforeach()
        foreach(FILE ${SCENE_FILES})
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources/scenes"
                )
        endforeach()

        # Make target a .app executable
        set_target_properties("${PROJECT_NAME}" PROPERTIES
                MACOSX_BUNDLE true
                MACOSX_BUNDLE_NAME "${PROJECT_NAME}"
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
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