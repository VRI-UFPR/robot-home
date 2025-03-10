cmake_minimum_required(VERSION 3.0)
project(ufr-app-filesystem)

# Server
find_package(OpenCV)
add_executable(ufr_data ./src/main.cpp)
target_link_libraries(ufr_data ufr ${OpenCV_LIBS})
install(TARGETS ufr_data DESTINATION bin)

# Shell Client
# add_executable(ufr_shell ./src/shell.c)
# target_link_libraries(ufr_shell ufr cwalk)
# install(TARGETS ufr_shell DESTINATION bin)
