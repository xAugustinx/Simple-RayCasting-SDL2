cmake_minimum_required(VERSION 4.2)
project(untitled27 C)
set(CMAKE_C_STANDARD 11)
add_executable(untitled27 main.c)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_include_directories(untitled27 PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(untitled27 ${SDL2_LIBRARIES} SDL2_image m)
