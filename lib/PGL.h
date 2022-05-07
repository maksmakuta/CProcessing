#ifndef PGL_H
#define PGL_H

// uses OpenGL 3.3 by default

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include "PShader.h"
#include "PShape.h"
#include "PColor.h"
#include "PStroker.h"

#define CENTER  0
#define RADIUS  1
#define CORNER  2
#define CORNERS 3

color fillColor,strokeColor,bg;
bool fillFlag,builder;
PShape tmp;
std::vector<glm::mat4> matrices;
glm::mat4 matrix;
float strokeWidth;
int cap,join,rMode,eMode;
PShader sh;

glm::vec4 vec(const color& c){
    return {c.r,c.g,c.b,c.a};
}

void initGL(){
    fillColor = color(255);strokeColor  = color(255);bg  = color(150);
    fillFlag = true;builder = false;
    strokeWidth = 1.0f;
    cap = 0;join = 0;
    sh = PShader::P5();
    rMode = 0;
    eMode = 0;
}

void doneGL(){
    matrices.clear();
    sh.done();
}

void draw(const PShape& s){
    glUseProgram(sh.programID());
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PVector) * s.size(), s.data().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(sh.attrLoc("ver"), 3, GL_FLOAT, GL_FALSE, sizeof(PVector), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);
    sh.mat4("matrix",matrix);
    sh.vec4("color",fillFlag ? vec(fillColor) : vec(strokeColor));
    glDrawArrays(s.type(), 0, s.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
    //s.done();
}

void strokeWeight(float w){
    strokeWidth = w;
}

void strokeJoin(int j){
    join = j;
}

void strokeCap(int c){
    cap = c;
}

void begin(float w,float h){
    glViewport(0,0,w,h);
    matrix = glm::ortho(0.f,w,h,0.f,-1.f,1.f);
}

void end(){ /* ... */ }

void beginShape(SHAPE_TYPE t = DEFAULT){
    builder = true;
    tmp = PShape(t);
}

void vertex(float x,float y,float z = 0.0f){
    if(builder)
        tmp.push(x,y,z);
    else
        std::cerr << "vertex() must be inside shapeBegin() and shapeEnd() functions\n";
}

void endShape(){
    builder = false;
    draw(tmp);
    tmp.done();
}

// Draws an arc in the display window
void arc(float x,float y,float w,float h,float b,float e){
    float d = PMath::radians(5.f);
    if(!fillFlag){
        tmp = PShape(TRIANGLES);
        for(float a = b; a <= e;a += d){
            float _x = x + w * sin(a);
            float _y = y + h * cos(a);
            tmp.push(_x,_y,0.f);
        }
        draw(strokify(tmp,strokeWidth,cap,join));
    }else{
        beginShape(TRIANGLE_STRIP);
        for(float a = b; a <= e;a += d){
            float _x = x + w * sin(a);
            float _y = y + h * cos(a);
            vertex(_x,_y);
            _x = x + w * sin(a+d);
            _y = y + h * cos(a+d);
            vertex(_x,_y);
            if(fillFlag) vertex(x,y);
        }
        endShape();
    }
}
// Draws a circle to the screen
void circle(float x,float y,float r){
    arc(x,y,r,r,0,TWO_PI);
}

void ellipseMode (unsigned mode) {
    assert (mode == CENTER || mode == RADIUS || mode == CORNER || mode == CORNERS);
    eMode = mode;
}

// Draws an ellipse (oval) in the display window
void ellipse(float x,float y,float w,float h){
    arc(x,y,w,h,0,TWO_PI);
}

// Draws a line (a direct path between two points) to the screen
void line(float x1,float y1,float x2,float y2){
    tmp = PShape(TRIANGLES);
    tmp.push(x1,y1,0.f);
    tmp.push(x2,y2,0.f);
    draw(strokify(tmp,strokeWidth,cap,join));
    //delete tmp;
}

// Draws a point, a coordinate in space at the dimension of one pixel
void point(float x,float y){
    tmp = PShape(TRIANGLES);
    tmp.push(x,y,0.f);
    draw(strokify(tmp,strokeWidth,cap,join));
    //delete tmp;
}

// A quad is a quadrilateral, a four sided polygon
void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    if(fillFlag){
        beginShape(QUADS);
        vertex(x1,y1);
        vertex(x2,y2);
        vertex(x3,y3);
        vertex(x4,y4);
        endShape();
    }else{
        tmp = PShape(TRIANGLES);
        tmp.push(x1,y1,0.f);
        tmp.push(x2,y2,0.f);
        tmp.push(x3,y3,0.f);
        tmp.push(x4,y4,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
    }
}

void rectMode (unsigned mode){
    assert (mode == CENTER || mode == RADIUS || mode == CORNER || mode == CORNERS);
    rMode = mode;
}

// Draws a rectangle to the screen
void rect(float x,float y,float w,float h){

    quad (x  , y  ,
          x+w, y  ,
          x+w, y+h,
          x  , y+h);

    /*
    switch(rMode){
        case CORNER:
            quad (x, y, x+a, y, x+a, y+b, x, y+b);
        break;
        case CENTER:
            quad (x-a/2, y-b/2, x+a/2, y-b/2, x+a/2, y+b/2, x-a/2, y+b/2);
        break;
        case RADIUS:
            quad (x-a, y-b, x+a, y-b, x+a, y+b, x-a, y+b);
        break;
        case CORNERS:
            quad (x, y, a, y, a, b, x, b);
        break;
    }
    */
}

void arc(PShape& sh,float x,float y,float r, float b,float e){

    float dr;
    if(r >= 0 && r <= 50)
        dr = 10.f;
    else if(r > 50 && r <= 100)
        dr = 5.f;
    else
        dr = 2.f;
    float d = PMath::radians(dr);
    for(float a = b; a <= e;a += d){
        float _x = x + r * sin(a);
        float _y = y + r * cos(a);
        sh.push(_x,_y,0.f);
    }
}

void rect(float x,float y,float w,float h,float rtl,float rtr,float rbl,float rbr){
    tmp = PShape(SHAPE_TYPE::TRIANGLE_FAN);
    if(fillFlag) tmp.push(x+w/2,y+h/2,0.f);
    arc(tmp,x+w-rbr ,y+h-rbr,rbr,0       ,PI/2      ); //br
    arc(tmp,x+w-rtr ,y+rtr  ,rtr,PI/2    ,PI        ); //tr
    arc(tmp,x+rtl   ,y+rtl  ,rtl,PI      ,(3*PI)/2  ); //tl
    arc(tmp,x+rbl   ,y+h-rbl,rbl,(3*PI)/2,2*PI      ); //bl
    tmp.push(x+w-rbr ,y+h,0.f);
    if(fillFlag)
        draw(tmp);
    else
        draw(strokify(tmp,strokeWidth,cap,join));
}

void rect(float x,float y,float w,float h,float r){
    rect(x,y,w,h,r,r,r,r);
}

// Draws a square to the screen
void square(float x,float y,float r){
    rect(x,y,r,r);
}

// A triangle is a plane created by connecting three points
void triangle(float x1,float y1,float x2,float y2,float x3,float y3){
    if(fillFlag){
        beginShape(TRIANGLES);
        vertex(x1,y1);
        vertex(x2,y2);
        vertex(x3,y3);
        endShape();
    }else{
        tmp = PShape(TRIANGLES);
        tmp.push(x1,y1,0.f);
        tmp.push(x2,y2,0.f);
        tmp.push(x3,y3,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
        //delete tmp;
    }
}

//Evaluates the Bezier at point t for points a, b, c, d
float bezierPoint(float a,float b,float c,float d,float t){
    return  (powf((1.0 - t),3) * a) +
            (3.0 * t * pow((1.0 - t),2)*b) +
            (3.0 * t * t * (1.0 - t) *c) +
            (t*t*t*d);
}

//Draws a Bezier curve on the screen
void bezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    tmp = PShape(LINES);
    float dt = 0.01f;
    for(float t = 0.0;t < 1.0f;t += dt){
        float _x1 = bezierPoint(x1,x2,x3,x4,t);
        float _y1 = bezierPoint(y1,y2,y3,y4,t);
        tmp.push(_x1,_y1,0.f);
        _x1 = bezierPoint(x1,x2,x3,x4,t+dt);
        _y1 = bezierPoint(y1,y2,y3,y4,t+dt);
        tmp.push(_x1,_y1,0.f);
    }
    draw(strokify(tmp,strokeWidth,cap,join));
    //delete tmp;
}

// ================================================================================

void pushMatrix(){
    matrices.push_back(matrix);
}

void popMatrix(){
    if(matrices.size() > 0){
        matrix = matrices[matrices.size() - 1];
        matrices.pop_back();
    }else{
        matrix = glm::mat4{0.f};
    }
}

void translate(float x,float y,float z = 0.0f){
    matrix = glm::translate(matrix,glm::vec3{x,y,z});
}

void rotate(float a){
    matrix = glm::rotate(matrix,a,glm::vec3{0.f,0.f,1.f});
}

#endif
