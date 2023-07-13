#include "gl_backend.h"
#include <iostream>
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
    glClear(GL_COLOR_BUFFER_BIT);
}
void GLBackend::draw(){

}
bool checkShader(int shader){
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return success;
}
bool checkProgram(int program){
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return success;
}
int GLBackend::compileShader(const char* src, int type){
    int shader = -1;
    if(type == SHADER_VERTEX){
        int t = glCreateShader(GL_VERTEX_SHADER);
        shader = t;
    }else if(type == SHADER_FRAGMENT){
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }else{
        shader = -1;
        std::cerr << "Unknown shader type\n";
    }
    if(shader > 0){
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
    }
    bool status = checkShader(shader);
    if(status){
        return shader;
    }else{
        return -1;
    }
}
int GLBackend::bindProgram(std::initializer_list<int> shaders){
    int shaderProgram = glCreateProgram();
    for(int shader : shaders){
        if(shader > 0){
            glAttachShader(shaderProgram, shader);
        }
    }
    glLinkProgram(shaderProgram);
    bool status = checkProgram(shaderProgram);
    if(status){
        for(int shader : shaders){
            if(shader > 0){
                glDeleteShader(shader);
            }
        }
        return shaderProgram;
    }else{
        return -1;
    }
}

GLBackend::~GLBackend(){

}

