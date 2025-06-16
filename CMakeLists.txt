cmake_minimum_required(VERSION 3.16)
project(SpaceSimulation)

set(CMAKE_CXX_STANDARD 17)

# Add raylib as a submodule or external dependency
add_subdirectory(external/raylib)

# Include directories for your headers
include_directories(include)

# Collect all source files manually
add_executable(SpaceSimulation
    src/main.cpp
    src/CelestialBody.cpp
    src/Planet.cpp
    src/Star.cpp
    src/Moon.cpp
    src/Collision.cpp
    src/Dust.cpp
    src/Meteor.cpp
    src/Camera_Control.cpp
    src/String.cpp
)

# Link against raylib
target_link_libraries(SpaceSimulation raylib)
