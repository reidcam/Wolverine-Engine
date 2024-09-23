# cmake build system for wolverine engine
# written by Jacob Robinson <mrjacob@umich.edu>
# 7/16/2024

cmake_minimum_required(VERSION 3.30)

project("wolverine-engine-demo" VERSION 1.0)

# Some quick settings
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED="NO" CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_JSON_SOURCE_FILE_EXTENSIONS json;JSON;config;template;scene)
set(SDL2TTF_VENDORED TRUE)
set(SDL2MIXER_VENDORED TRUE)

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
file(GLOB ALL_RESOURCES 
        ${IMAGE_FILES}
        ${AUDIO_FILES}
        ${FONT_FILES}
        ${CONFIG_FILES}
        ${ACTOR_TEMPLATES}
        ${COMPONENT_TYPES}
        ${SCENE_FILES}
)

# Add executable
add_executable("${PROJECT_NAME}" 
        ${FIRST_PARTY}
        # bundle resources
        ${ALL_RESOURCES}
)
target_include_directories("${PROJECT_NAME}" PRIVATE "src/FirstParty")

if (MSVC)
        # Set Visual Studio Startup Project
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

        # Needed for Visual Studio due to the size of LuaAPI.cpp
        target_compile_options("${PROJECT_NAME}" PRIVATE /bigobj)

        # Set the output directory for Debug and Release to the root directory so that the resources folder can be properly found
        set_target_properties("${PROJECT_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
        set_target_properties("${PROJECT_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
endif()

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

# Add imgui library
file(GLOB IMGUI_SOURCES
        "src/ThirdParty/imgui-1.91.2-docking/*.cpp"
        "src/ThirdParty/imgui-1.91.2-docking/backends/imgui_impl_sdl2.cpp"
        "src/ThirdParty/imgui-1.91.2-docking/backends/imgui_impl_sdlrenderer2.cpp"
)
add_library("${PROJECT_NAME}_imgui" ${IMGUI_SOURCES})
target_include_directories("${PROJECT_NAME}_imgui"
        PUBLIC
        "src/ThirdParty/imgui-1.91.2-docking"
        "src/ThirdParty/imgui-1.91.2-docking/backends"
)

if(APPLE)
        # Add all of the game resources to the mac bundle
        foreach(FILE ${ALL_RESOURCES})
                file(RELATIVE_PATH NEW_FILE "${CMAKE_SOURCE_DIR}/resources" ${FILE})
                get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
                set_source_files_properties(${FILE} PROPERTIES
                        MACOSX_PACKAGE_LOCATION "Resources/resources/${NEW_FILE_PATH}"
                )
        endforeach()

        # Make target a .app executable
        set_target_properties("${PROJECT_NAME}" PROPERTIES
                MACOSX_BUNDLE true
                MACOSX_BUNDLE_NAME "${PROJECT_NAME}"
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        )
else()
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${CMAKE_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
        )
endif()


# Sets the C++ version
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20 c_std_17)
target_compile_features("${PROJECT_NAME}_lua" PRIVATE cxx_std_20 c_std_17)
target_compile_features("${PROJECT_NAME}_box2d" PRIVATE cxx_std_20 c_std_17)
target_compile_features("${PROJECT_NAME}_imgui" PRIVATE cxx_std_20 c_std_17)

# Add third party libraries
add_subdirectory("src/ThirdParty/glm")
add_subdirectory("src/ThirdParty/SDL2-2.30.5")
add_subdirectory("src/ThirdParty/SDL2_image-2.8.2")
add_subdirectory("src/ThirdParty/SDL2_ttf-2.22.0")
add_subdirectory("src/ThirdParty/SDL2_mixer-2.8.0")

# Link SDL2 to imgui so it can use it
target_link_libraries("${PROJECT_NAME}_imgui"
        PRIVATE
        SDL2::SDL2-static
        SDL2_image::SDL2_image-static
        SDL2_ttf::SDL2_ttf-static
        SDL2_mixer::SDL2_mixer-static
	SDL2main
)

# Link everything together
target_link_libraries("${PROJECT_NAME}"
        PRIVATE
        "${PROJECT_NAME}_box2d"
        "${PROJECT_NAME}_lua"
        "${PROJECT_NAME}_imgui"
        glm
        SDL2::SDL2-static
        SDL2_image::SDL2_image-static
        SDL2_ttf::SDL2_ttf-static
        SDL2_mixer::SDL2_mixer-static
	SDL2main
)