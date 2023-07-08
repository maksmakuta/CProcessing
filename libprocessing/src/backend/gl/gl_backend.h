#ifndef GL_BACKEND_H
#define GL_BACKEND_H

#include "../backend.h"

class GLBackend : public backend{
public:
    GLBackend();

    void init() override;

    void begin() override;
    void clearColor(float r,float g,float b,float a) override;
    void viewport(float w, float h) override;
    void end() override;

    ~GLBackend();
};

#endif
