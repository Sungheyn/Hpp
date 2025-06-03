cmake_minimum_required(VERSION 3.12)

project(Compiler)

file(GLOB_RECURSE PROJECT_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh"
)

add_executable(Compiler ${PROJECT_SOURCES})