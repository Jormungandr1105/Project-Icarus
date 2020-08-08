cmake_minimum_required(VERSION 3.18)
project(Project_Icarus)

add_library(stllib Model.cpp Plane.h Triangle.cpp Vertex.cpp)
add_executable(stlapp main.cpp)
target_link_libraries(stlapp stllib)
