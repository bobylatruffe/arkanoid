cmake_minimum_required(VERSION 3.27)
project(arkanoid C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(PkgConfig)

pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
include_directories(${SDL2IMAGE_INCLUDE_DIRS})
link_directories(${SDL2IMAGE_LIBRARY_DIRS})

include_directories(${SDL2_INCLUDE_DIRS})
message(STATUS "SDL2_INCLUDE_DIRS = ${SDL2_INCLUDE_DIRS}")

add_executable(arkanoid
        main.c
        type.h
        init.h
        init.c
        render.c
        render.h
        object.h
        object.c
        update.h
        update.c
        check.h
        check.c)
message(STATUS "SDL2_LIBRARIE" = ${SDL2_LIBRARIES})
target_link_libraries(arkanoid ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

target_compile_definitions(arkanoid PRIVATE DEBUG)

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})