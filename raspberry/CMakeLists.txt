cmake_minimum_required(VERSION 3.22)
project(pioneer-raspberry)

include_directories(./src/lib_pioneer/include)

add_subdirectory(src/lib_pioneer)
add_subdirectory(src/app_pioneer)
add_subdirectory(src/app_data)