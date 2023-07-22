#ifndef BACKEND_H
#define BACKEND_H

#include <initializer_list>
#include <string>

#define SHADER_VERTEX 1
#define SHADER_FRAGMENT 2

#define F_MSAA 0x0001

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
    virtual void enable(int feature) = 0;
    virtual void disable(int feature) = 0;

    virtual int compileShader(const char* src, int type) = 0;
    virtual int linkProgram(std::initializer_list<int> shaders) = 0;
    virtual void bindShader(int program) = 0;

    virtual int getUniformLocation(int program, const std::string& name) = 0;
    virtual void setUniform(int program, const std::string& name, int x)  = 0;
    virtual void setUniform(int program, const std::string& name, int x,int y) = 0;
    virtual void setUniform(int program, const std::string& name, int x,int y,int z) = 0;
    virtual void setUniform(int program, const std::string& name, int x,int y,int z,int w) = 0;
    virtual void setUniform(int program, const std::string& name, float x) = 0;
    virtual void setUniform(int program, const std::string& name, float x,float y) = 0;
    virtual void setUniform(int program, const std::string& name, float x,float y,float z) = 0;
    virtual void setUniform(int program, const std::string& name, float x,float y,float z,float w) = 0;
    virtual void setUniform(int program, const std::string& name, bool x) = 0;
    virtual void setUniform(int program, const std::string& name, bool x,bool y) = 0;
    virtual void setUniform(int program, const std::string& name, bool x,bool y,bool z) = 0;
    virtual void setUniform(int program, const std::string& name, bool x,bool y,bool z,bool w) = 0;
    virtual void setUniform(int program, const std::string& name, bool* vec,int n) = 0;
    virtual void setUniform(int program, const std::string& name, int* vec,int n) = 0;
    virtual void setUniform(int program, const std::string& name, float* vec,int n) = 0;

    virtual ~backend(){}
};

#endif
