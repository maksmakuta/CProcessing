#ifndef GL_BACKEND_H
#define GL_BACKEND_H

#include "../backend.h"
#include <map>
#include <string>
#define SHGL

class GLBackend : public backend{
private:
    int activeProgram = -1;
public:
    GLBackend();

    void init() override;

    void begin() override;
    void clearColor(float r,float g,float b,float a) override;
    void viewport(float w, float h) override;
    void end() override;

    void enable(int feature) override;
    void disable(int feature) override;
    void draw(std::vector<vert>& vert) override;

    int compileShader(const char* src, int type) override;
    int linkProgram(std::initializer_list<int> shaders) override;
    void bindShader(int program) override;

    int getUniformLocation(int program, const std::string& name) override;
    void setUniform(int program, const std::string& name, int x)  override;
    void setUniform(int program, const std::string& name, int x,int y) override;
    void setUniform(int program, const std::string& name, int x,int y,int z) override;
    void setUniform(int program, const std::string& name, int x,int y,int z,int w) override;
    void setUniform(int program, const std::string& name, float x) override;
    void setUniform(int program, const std::string& name, float x,float y) override;
    void setUniform(int program, const std::string& name, float x,float y,float z) override;
    void setUniform(int program, const std::string& name, float x,float y,float z,float w) override;
    void setUniform(int program, const std::string& name, bool x) override;
    void setUniform(int program, const std::string& name, bool x,bool y) override;
    void setUniform(int program, const std::string& name, bool x,bool y,bool z) override;
    void setUniform(int program, const std::string& name, bool x,bool y,bool z,bool w) override;
    void setUniform(int program, const std::string& name, bool* vec,int n) override;
    void setUniform(int program, const std::string& name, int* vec,int n) override;
    void setUniform(int program, const std::string& name, float* vec,int n) override;

    uint genTexture(unsigned char* data,int w,int h,int type) override;

    ~GLBackend();
};

#endif
