#include "gl_backend.h"
#include <iostream>
#include <glad/glad.h>
#include <string>

GLuint VAO, VBO;

GLBackend::GLBackend() : backend(){

}

void GLBackend::init(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //GLint temp;
    //glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &temp);
    //std::cout << "Max textures: " << temp << "\n";
}

void GLBackend::begin(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void GLBackend::end(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void GLBackend::viewport(float w,float h){
    glViewport(0,0,w,h);
}

void GLBackend::clearColor(float r,float g,float b,float a){
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
}
void GLBackend::draw(std::vector<vert>& points){
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vert), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(7*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(9*sizeof(float)));
    glEnableVertexAttribArray(3);
    glDrawArrays(GL_TRIANGLES, 0, points.size());
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
bool checkShader(int type,int shader){
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        if(type == SHADER_VERTEX){
            std::cout << "VERTEX SHADER\n";
        }else if(type == SHADER_FRAGMENT){
            std::cout << "FRAGMENT SHADER\n";
        }else{
            std::cerr << "Unknown shader type\n";
        }
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
        shader = glCreateShader(GL_VERTEX_SHADER);
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
    bool status = checkShader(type,shader);
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
    if(activeProgram != program)
        bindShader(program);
    int loc = getUniformLocation(program,name);
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
    case 6:
        glUniformMatrix3x2fv(loc,1,false,vec);
        break;
    case 9:
        glUniformMatrix3fv(loc,1,false,vec);
        break;
    case 16:
        glUniformMatrix4fv(loc,1,false,vec);
        break;
    }
}

uint GLBackend::genTexture(unsigned char* data,int w,int h,int type){
    uint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return tex;
}

GLBackend::~GLBackend(){

}

