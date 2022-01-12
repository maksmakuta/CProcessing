cmake_minimum_required(VERSION 3.10)
project(sketch VERSION 1.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)
add_executable(sketch sketch.cpp)
target_link_libraries(sketch cprocessing)
target_include_directories(sketch PUBLIC lib)
