#ifndef BACKEND_H
#define BACKEND_H

#include <initializer_list>

#define SHADER_VERTEX 1
#define SHADER_FRAGMENT 2

struct vertex{
    float x = 0.f,y = 0.f,z = 0.f;
    float r = 0.f,g = 0.f,b = 0.f,a = 0.f;
    float u = 0.f,v = 0.f;
};

class backend{
public:

    backend(){}

    virtual void init() = 0;

    virtual void begin() = 0;
    virtual void clearColor(float r,float g,float b,float a) = 0;
    virtual void viewport(float w, float h) = 0;
    virtual void end() = 0;

    virtual void draw() = 0;

    virtual int compileShader(const char* src, int type) = 0;
    virtual int bindProgram(std::initializer_list<int> shaders) = 0;

    virtual ~backend(){}
};

#endif
