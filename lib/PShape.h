#ifndef PSHAPE_H
#define PSHAPE_H

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "PColor.h"
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

struct vertex{
    float x,y,z;
    float s,t;
};

class PShape{
private:
    SHAPE_TYPE stype;
    color c;
    std::vector<vertex> verData;
public:
    PShape() : PShape(DEFAULT){ }

    PShape(SHAPE_TYPE st){
        this->stype = st;
    }

    void push(float a,float b,float c){
        verData.push_back({a,b,c,0.f,0.f});
    }

    void setColor(const color& _c){
        this->c = _c;
    }

    color getColor() const {
        return this->c;
    }

    void push(const PVector& v){
        push(v.x,v.y,v.z);
    }

    void add(const PShape& p){
        std::vector<vertex> tmp = p.data();
        if(!tmp.empty()){
            verData.insert(verData.end(),tmp.begin(),tmp.end());
        }
    }

    vertex at(int p){
        if(p >= 0 && p < this->size())
            return verData[p];
        else{
            std::cerr << "Undefined position\n";
            return {0,0,0,0,0};
        }
    }

    int size() const{
        return verData.size();
    }

    SHAPE_TYPE type() const{
        return stype;
    }
    void type(SHAPE_TYPE s){
        stype = s;
    }

    std::vector<vertex> data() const{
        return verData;
    }

    void data(const std::vector<vertex>& d) {
        this->verData = d;
    }

    bool loop(){
        auto a = createVector(verData[0].x,verData[0].y,verData[0].z);
        auto b = createVector(  verData[verData.size() - 1].x,
                                verData[verData.size() - 1].y,
                                verData[verData.size() - 1].z
                );
        return PVector::equal(a,b);
    }

    void done(){
        verData.clear();
    }
};

#endif
