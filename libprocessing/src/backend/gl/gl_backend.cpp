#include "gl_backend.h"
#include <iostream>
#include <glad/glad.h>
#include <string>

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

void GLBackend::enable(int feature){
    if(feature == F_MSAA){
        glEnable(GL_MULTISAMPLE);
    }
}
void GLBackend::disable(int feature){
    if(feature == F_MSAA){
        glDisable(GL_MULTISAMPLE);
    }
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
int GLBackend::linkProgram(std::initializer_list<int> shaders){
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

void GLBackend::bindShader(int program){
    glUseProgram(program);
    activeProgram = program;
}
int GLBackend::getUniformLocation(int program, const std::string& name){
    if(activeProgram != program)
        bindShader(program);
    return glGetUniformLocation(program, name.c_str());
}
void GLBackend::setUniform(int program, const std::string& name, int x){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform1i(loc,x);
}
void GLBackend::setUniform(int program, const std::string& name, int x,int y){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform2i(loc,x,y);
}
void GLBackend::setUniform(int program, const std::string& name, int x,int y,int z){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform3i(loc,x,y,z);
}
void GLBackend::setUniform(int program, const std::string& name, int x,int y,int z,int w){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform4i(loc,x,y,z,w);
}
void GLBackend::setUniform(int program, const std::string& name, float x){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform1f(loc,x);
}
void GLBackend::setUniform(int program, const std::string& name, float x,float y){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform2f(loc,x,y);
}
void GLBackend::setUniform(int program, const std::string& name, float x,float y,float z){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform3f(loc,x,y,z);
}
void GLBackend::setUniform(int program, const std::string& name, float x,float y,float z,float w){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform4f(loc,x,y,z,w);
}
void GLBackend::setUniform(int program, const std::string& name, bool x){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform1i(loc,(int)x);
}
void GLBackend::setUniform(int program, const std::string& name, bool x,bool y){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform2i(loc,(int)x,(int)y);
}
void GLBackend::setUniform(int program, const std::string& name, bool x,bool y,bool z){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform3i(loc,(int)x,(int)y,(int)z);
}
void GLBackend::setUniform(int program, const std::string& name, bool x,bool y,bool z,bool w){
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
    glUniform4i(loc,(int)x,(int)y,(int)z,(int)w);
}
void GLBackend::setUniform(int program, const std::string& name, bool* vec){
    int size = sizeof(vec) / sizeof(bool);
    switch(size){
    case 1:
        setUniform(program,name,vec[0]);
        break;
    case 2:
        setUniform(program,name,vec[0],vec[1]);
        break;
    case 3:
        setUniform(program,name,vec[0],vec[1],vec[2]);
        break;
    case 4:
        setUniform(program,name,vec[0],vec[1],vec[2],vec[3]);
        break;
    }
}
void GLBackend::setUniform(int program, const std::string& name, int* vec){
    int size = sizeof(vec) / sizeof(int);
    switch(size){
    case 1:
        setUniform(program,name,vec[0]);
        break;
    case 2:
        setUniform(program,name,vec[0],vec[1]);
        break;
    case 3:
        setUniform(program,name,vec[0],vec[1],vec[2]);
        break;
    case 4:
        setUniform(program,name,vec[0],vec[1],vec[2],vec[3]);
        break;
    }
}
void GLBackend::setUniform(int program, const std::string& name, float* vec){
    int size = sizeof(vec) / sizeof(float);
    switch(size){
    case 1:
        setUniform(program,name,vec[0]);
        break;
    case 2:
        setUniform(program,name,vec[0],vec[1]);
        break;
    case 3:
        setUniform(program,name,vec[0],vec[1],vec[2]);
        break;
    case 4:
        setUniform(program,name,vec[0],vec[1],vec[2],vec[3]);
        break;
    }
}
void GLBackend::setUniform(int program, const std::string& name, bool* vec,int n){
    switch(n){
    case 1:
        setUniform(program,name,vec[0]);
        break;
    case 2:
        setUniform(program,name,vec[0],vec[1]);
        break;
    case 3:
        setUniform(program,name,vec[0],vec[1],vec[2]);
        break;
    case 4:
        setUniform(program,name,vec[0],vec[1],vec[2],vec[3]);
        break;
    }
}
void GLBackend::setUniform(int program, const std::string& name, int* vec,int n){
    switch(n){
    case 1:
        setUniform(program,name,vec[0]);
        break;
    case 2:
        setUniform(program,name,vec[0],vec[1]);
        break;
    case 3:
        setUniform(program,name,vec[0],vec[1],vec[2]);
        break;
    case 4:
        setUniform(program,name,vec[0],vec[1],vec[2],vec[3]);
        break;
    }
}
void GLBackend::setUniform(int program, const std::string& name, float* vec,int n){
    switch(n){
    case 1:
        setUniform(program,name,vec[0]);
        break;
    case 2:
        setUniform(program,name,vec[0],vec[1]);
        break;
    case 3:
        setUniform(program,name,vec[0],vec[1],vec[2]);
        break;
    case 4:
        setUniform(program,name,vec[0],vec[1],vec[2],vec[3]);
        break;
    }
}

GLBackend::~GLBackend(){

}

