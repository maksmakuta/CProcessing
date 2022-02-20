#ifndef PSHAPE_H
#define PSHAPE_H

#include <GL/glew.h>
#include <vector>
#include "PVector.h"

enum SHAPE_TYPE{
    POINTS          = GL_POINTS         ,
    LINES           = GL_LINES          ,
    LINE_LOOP       = GL_LINE_LOOP      ,
    TRIANGLES       = GL_TRIANGLES      ,
    TRIANGLE_FAN    = GL_TRIANGLE_FAN   ,
    TRIANGLE_STRIP  = GL_TRIANGLE_STRIP ,
    QUADS           = GL_QUADS          ,
    QUAD_STRIP      = GL_QUAD_STRIP     ,
    DEFAULT         = GL_POLYGON
};

class PShape{
public:
    PShape() : PShape(DEFAULT){ }
    PShape(SHAPE_TYPE st){
        this->type = st;
    }

    void push(float a,float b,float c){
        vertex.push_back(PVector(a,b,c));
    }

    ~PShape(){
        this->clear();
    }

    bool loop(){
        return !equal(vertex[0],vertex[vertex.size() - 1]);
    }

    void clear(){
        vertex.clear();
    }

    SHAPE_TYPE type;
    std::vector<PVector> vertex;
};

#endif
