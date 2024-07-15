cmake_minimum_required(VERSION 3.30)

project("wolverine-engine-demo")

# GLOB all of our files
file(GLOB FIRST_PARTY "src/FirstParty/src/*.cpp" "src/FirstParty/*.h")

# Add executable
add_executable("${PROJECT_NAME}" ${FIRST_PARTY})

# Sets the C++ version
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_17)

# Add third party libraries
add_subdirectory("src/ThirdParty/glm")
add_subdirectory("src/ThirdParty/rapidjson-1.1.0")

# Link everything together
target_link_libraries("${PROJECT_NAME}"
        PRIVATE
        glm
        rapidjson-1.1.0
)