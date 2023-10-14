cmake_minimum_required(VERSION 3.14)
project(SDL_Handle LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SDL2 REQUIRED)
add_executable(build ../src/main.cpp ../src/SDL_handle.cpp)

include_directories(SDL_Handle ${SDL2_INCLUDE_DIRS})
target_link_libraries(build ${SDL2_LIBRARIES})

