#ifndef PSHAPE_H
#define PSHAPE_H

#include <GL/glew.h>
#include <vector>
#include "PVector.h"

enum SHAPE_TYPE{
    POINTS          = GL_POINTS         ,
    LINES           = GL_LINES          ,
    TRIANGLES       = GL_TRIANGLES      ,
    TRIANGLE_FAN    = GL_TRIANGLE_FAN   ,
    TRIANGLE_STRIP  = GL_TRIANGLE_STRIP ,
    QUADS           = GL_QUADS          ,
    QUAD_STRIP      = GL_QUAD_STRIP     ,
    DEFAULT         = GL_POLYGON
};

class PShape{
private:
    std::vector<PVector> vertex;
    SHAPE_TYPE type;
public:
    PShape(SHAPE_TYPE st){
        this->type = st;
    }

    void push(float a,float b,float c){
        vertex.push_back(PVector(a,b,c));
    }

    //@old
    void draw(){
        glBegin(type);
        for(PVector v : vertex){
            glVertex3f(v.x,v.y,v.z);
        }
        glEnd();
    }

    ~PShape(){
        vertex.clear();
    }
};

#endif
