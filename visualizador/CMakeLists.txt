project(visualizador)
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

find_package(PkgConfig REQUIRED)
# Find SDL2

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_gfx REQUIRED SDL2_gfx)

# Add global definitions
add_definitions("-Wall")
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_gfx_INCLUDE_DIR})

add_executable(visualizador main.cpp visualizador.cpp)
add_executable(mov_seq mov_seq.cpp)
add_executable(mov_par mov_par.cpp)
SET_TARGET_PROPERTIES(mov_par PROPERTIES COMPILE_FLAGS "-fopenmp"
LINK_FLAGS "-fopenmp")

add_executable(mov_simd mov_seq.cpp)
SET_TARGET_PROPERTIES(mov_simd PROPERTIES COMPILE_FLAGS "-ffast-math -ftree-vectorize -mavx")


target_link_libraries(visualizador ${SDL2_LIBRARIES} ${SDL2_gfx_LIBRARIES})


