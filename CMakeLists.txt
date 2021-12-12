cmake_minimum_required(VERSION 3.10)
project(processing++)

set(nvg
    lib/ngl/fontstash.h
    lib/ngl/nanovg.c
    lib/ngl/nanovg.h
    lib/ngl/nanovg_gl.h
    lib/ngl/nanovg_gl_utils.h
    lib/ngl/stb_image.h
    lib/ngl/stb_truetype.h
)

set(processing_api
    lib/processing.h
    lib/PVector.h
    lib/Math.h
    lib/ArrayList.h)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    link_libraries(${OPENGL_LIBRARIES})
endif()

###################################
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(     ${GLFW_LIBRARIES})
link_libraries(     ${GLFW_STATIC_LIBRARIES})
###################################

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(     ${GLEW_LIBRARIES})
endif()

add_executable(processing++ ${nvg} ${processing_api} sketch.cpp)
target_include_directories(processing++ PRIVATE lib)
