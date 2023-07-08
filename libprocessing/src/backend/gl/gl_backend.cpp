#include "gl_backend.h"
#include <glad/glad.h>

GLBackend::GLBackend() : backend(){

}

void GLBackend::init(){

}

void GLBackend::begin(){

}

void GLBackend::end(){

}

void GLBackend::viewport(float w,float h){
    glViewport(0,0,w,h);
}

void GLBackend::clearColor(float r,float g,float b,float a){
    glClearColor(r,g,b,a);
}

GLBackend::~GLBackend(){

}

