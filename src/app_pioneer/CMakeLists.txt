cmake_minimum_required(VERSION 3.5)
project(app_pioneer)

# Example 1: simple
add_executable(pioneer_base ./src/main.c)
target_link_libraries(pioneer_base pioneer ufr)
install(TARGETS pioneer_base DESTINATION bin)

# Service configuration
install(FILES ./etc/vri-pioneer.service DESTINATION boot)