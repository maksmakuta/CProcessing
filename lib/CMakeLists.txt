cmake_minimum_required(VERSION 3.20)
project(cprocessing VERSION 1.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

#########################################################
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    link_libraries(${OPENGL_LIBRARIES})
endif()

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(     ${GLFW_LIBRARIES})
link_libraries(     ${GLFW_STATIC_LIBRARIES})

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(     ${GLEW_LIBRARIES})
endif()

#############################################################

add_library(cprocessing SHARED
    PColor.h
    PFont.h
    PGL.h
    PImage.h
    PMath.h
    PShader.h
    PShape.h
    PStroker.h
    PTime.h
    PVector.h
    processing.h)

set_target_properties(cprocessing PROPERTIES LINKER_LANGUAGE CXX)
