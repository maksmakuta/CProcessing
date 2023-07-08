#ifndef BACKEND_H
#define BACKEND_H

class backend{
public:

    backend(){}

    virtual void init() = 0;

    virtual void begin() = 0;
    virtual void clearColor(float r,float g,float b,float a) = 0;
    virtual void viewport(float w, float h) = 0;
    virtual void end() = 0;

    virtual ~backend(){}
};

#endif
