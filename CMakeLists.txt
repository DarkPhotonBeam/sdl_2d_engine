cmake_minimum_required(VERSION 3.31)
project(sdl_velocity_verlet C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

if(NOT TARGET SDL3::SDL3)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL/CMakeLists.txt")
        add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
    else()
        find_package(SDL3 REQUIRED)
    endif()
endif()

add_library(2d_engine
        src/vector2d.c
        include/vector2d.h
        src/rigidbody2d.c
        include/rigidbody2d.h
        src/game.c
        include/game.h
        src/string.c
        include/string.h
)

add_library(DLeuthold::SDL2DEngine ALIAS 2d_engine)

target_link_directories(2d_engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(2d_engine PUBLIC SDL3::SDL3 PRIVATE m)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_executable(2d_engine_demo src/main.c)

    # Link the internal alias
    target_link_libraries(2d_engine_demo PRIVATE DLeuthold::SDL2DEngine)
endif()