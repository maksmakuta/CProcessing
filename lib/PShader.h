#ifndef PSHADER_H
#define PSHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class PShader{
private:
    GLuint programS;
    GLuint vertS,fragS;
public:
    PShader(){}

    void loadFile(const std::string& vertex,const std::string& fragment){
        this->loadCode(load(vertex),load(fragment));
    }

    GLuint programID() const {return this->programS;}

    void mat4(const std::string& name,const glm::mat4& matrix){
        glUniformMatrix4fv(glGetUniformLocation(programID(), name.c_str()),1, GL_FALSE, glm::value_ptr(matrix));
    }

    void loadCode(const std::string& vertex,const std::string& fragment){
        this->vertS = makeShader(GL_VERTEX_SHADER  ,  vertex.c_str());
        this->fragS = makeShader(GL_FRAGMENT_SHADER,fragment.c_str());
        this->programS = makeProgram(vertS,fragS);
    }

    static PShader files(const std::string& vertex,const std::string& fragment){
        PShader s;
        s.loadFile(vertex,fragment);
        return s;
    }

    static PShader code(const std::string& vertex,const std::string& fragment){
        PShader s;
        s.loadCode(vertex,fragment);
        return s;
    }

private:
    bool isShaderOK(GLuint shader){
        int  success; char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "Shader ERROR: \n" << infoLog << std::endl;
        }
        return success;
    }

    bool isProgramOK(GLuint program){
        int  success; char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "Program ERROR: \n" << infoLog << std::endl;
        }
        return success;
    }

    GLuint makeShader(int type,const char* code){
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &code, NULL);
        glCompileShader(shader);
        return isShaderOK(shader) ? shader : 0;
    }

    GLuint makeProgram(GLuint vert,GLuint frag){
        GLuint program = glCreateProgram();
        glAttachShader(program, vert);
        glAttachShader(program, frag);
        glLinkProgram(program);
        return isProgramOK(program) ? program : 0;
    }

    std::string load(const std::string& fname){
        std::stringstream ss;
        std::ifstream f(fname);
        if(f.is_open()){
            ss << f.rdbuf();
            f.close();
        }
        return ss.str();
    }
};

#endif
