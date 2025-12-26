cmake_minimum_required(VERSION 4.2)
project(untitled27 C)
set(CMAKE_C_STANDARD 11)
add_executable(untitled27 main.c)

find_package(SDL2 REQUIRED)
target_link_libraries(untitled27 ${SDL2_LIBRARIES} m)
