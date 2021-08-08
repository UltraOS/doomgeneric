cmake_minimum_required(VERSION 3.16)

project(DOOM C)

FILE(GLOB C_FILES "${PROJECT_SOURCE_DIR}/doomgeneric/*c" "${PROJECT_SOURCE_DIR}/doomgeneric/*h")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
add_port_executable(DOOM.EXE ${C_FILES})
add_extra_files("${PROJECT_SOURCE_DIR}/DOOM1.WAD")
