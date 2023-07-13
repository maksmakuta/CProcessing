#ifndef GL_BACKEND_H
#define GL_BACKEND_H

#include "../backend.h"
#include <map>
#include <string>

class GLBackend : public backend{
public:
    GLBackend();

    void init() override;

    void begin() override;
    void clearColor(float r,float g,float b,float a) override;
    void viewport(float w, float h) override;
    void end() override;

    void draw() override;

    int compileShader(const char* src, int type) override;
    int bindProgram(std::initializer_list<int> shaders) override;

    ~GLBackend();
};

#endif
