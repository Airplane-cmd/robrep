cmake_minimum_required(VERSION 3.14)
project(joystickTest LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SDL2 REQUIRED)
add_executable(bin ../src/main.cpp)

include_directories(joystickTest ${SDL2_INCLUDE_DIRS})
target_link_libraries(bin ${SDL2_LIBRARIES})

