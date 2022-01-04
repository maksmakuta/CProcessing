#ifndef PGL_H
#define PGL_H

#include <GL/glew.h>
#include <iostream>

const char* vertex   = "#version 330 core"
        "layout (location = 0) in vec3 aPos;"

        "void main(){"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
const char* fragment = "#version 330 core"
        "out vec4 FragColor;"

        "void main(){"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}";

class pShader{
private:
    GLuint program,vert,frag;
public:
    pShader(){
        load(vertex,fragment);
    }

    void load(const char* vertc,const char* fragc){
        this->vert = create(vertc,GL_VERTEX_SHADER);
        this->frag = create(fragc,GL_FRAGMENT_SHADER);
        if(this->frag != 0 && this->vert != 0){
            this->program = createProgram(this->vert,this->frag);
        }
    }
private:
    GLuint create(const char* code, GLuint type){
        unsigned int shader = glCreateShader(type);//GL_VERTEX_SHADER);
        glShaderSource(shader, 1, &code, NULL);
        glCompileShader(shader);

        int  success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "COMPILATION_FAILED: \n" << infoLog << std::endl;
        }else{
            return shader;
        }
        return 0;
    }

    GLuint createProgram(GLuint v,GLuint f){
        GLuint pp = glCreateProgram();
        glAttachShader(pp, v);
        glAttachShader(pp, f);
        glLinkProgram(pp);

        int  success;
        char infoLog[512];
        glGetProgramiv(pp, GL_LINK_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(pp, 512, NULL, infoLog);
            std::cout << "COMPILATION_FAILED: \n" << infoLog << std::endl;
        }else{
            return pp;
        }
        return 0;
    }

    void use(){
        glUseProgram(this->program);
    }

};

enum COMMANDS{
    MOVE    = 1,
    LINE    = 2,
    BEZIER  = 3,
    CLOSE   = 4,
    WIND    = 5
};

class pGL{
private:
    pShader shader;
public:
    pGL(){
        shader = pShader();
    }

    void init(){

    }

    void perform(enum COMMANDS c,float x,float y){

    }
};

#endif
