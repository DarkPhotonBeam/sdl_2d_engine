cmake_minimum_required(VERSION 3.31)
project(sdl_2d_engine C)

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
        include/2DEngine/vector2d.h
        src/rigidbody2d.c
        include/2DEngine/rigidbody2d.h
        src/game.c
        include/2DEngine/game.h
        src/string.c
        include/2DEngine/string.h
)

add_library(DLeuthold::2DEngine ALIAS 2d_engine)

target_include_directories(2d_engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(2d_engine PUBLIC SDL3::SDL3 PRIVATE m)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_executable(2d_engine_demo src/main.c)

    # Link the internal alias
    target_link_libraries(2d_engine_demo PRIVATE DLeuthold::2DEngine)
endif()