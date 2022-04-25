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
private:
    SHAPE_TYPE stype;
    std::vector<PVector> vertex;
public:
    PShape() : PShape(DEFAULT){ }
    PShape(SHAPE_TYPE st){
        this->stype = st;
    }

    void push(float a,float b,float c){
        vertex.push_back(PVector(a,b,c));
    }

    int size() const{
        return vertex.size();
    }

    SHAPE_TYPE type() const{
        return stype;
    }
    void type(SHAPE_TYPE s){
        stype = s;
    }

    std::vector<PVector> data() const{
        return vertex;
    }

    void data(const std::vector<PVector>& d) {
        this->vertex = d;
    }

    bool loop(){
        return !PVector::equal(vertex[0],vertex[vertex.size() - 1]);
    }

    void done(){
        vertex.clear();
    }
};

#endif
