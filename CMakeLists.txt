cmake_minimum_required(VERSION 3.16)
project(SpaceSimulation LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/raylib)

set_source_files_properties(
    external/raylib/src/rcore.c
    external/raylib/src/raudio.c
    external/raylib/src/rmodels.c
    external/raylib/src/rshapes.c
    external/raylib/src/rtext.c
    external/raylib/src/rtextures.c
    external/raylib/src/utils.c
    PROPERTIES LANGUAGE C
)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SRC_FILES src/*.cpp)
add_executable(SpaceSimulation ${SRC_FILES})
target_link_libraries(SpaceSimulation PRIVATE raylib)

if (WIN32)
    target_link_libraries(SpaceSimulation PRIVATE winmm gdi32 user32 shell32)
elseif(APPLE)
    target_link_libraries(SpaceSimulation PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
elseif(UNIX)
    target_link_libraries(SpaceSimulation PRIVATE m pthread dl GL X11)
endif()

set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
add_custom_command(TARGET SpaceSimulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_SOURCE_DIR}
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/assets
)
