#ifndef PGL_H
#define PGL_H

#include <vector>
#include "PShader.h"
#include "PShape.h"
#include "PColor.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <GL/glew.h>

color fillColor = color(255),strokeColor  = color(255),bg  = color(150);
bool fillFlag = true;
PShape* tmp = NULL;
std::vector<PShape*> shapes;

void begin(float w,float h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,w,h,0,0,1);
    glMatrixMode(GL_MODELVIEW);
}

void end(){
    for(PShape* s : shapes){
        s->draw();
    }
    shapes.clear();
}

void applyColor(bool x){
    color c = x ? fillColor : strokeColor;
    glColor4f(c.r,c.g,c.b,c.a);
}

void beginShape(SHAPE_TYPE t = DEFAULT){
    tmp = new PShape(t);
}

void vertex(float x,float y,float z = 0.0f){
    if(tmp != NULL)
        tmp->push(x,y,z);
    else
        std::cerr << "vertex() must be inside shapeBegin() and shapeEnd() functions\n";
}

void endShape(){
     shapes.push_back(tmp);
     tmp = NULL;
}

// Draws an arc in the display window
void arc(float x,float y,float w,float h,float b,float e){
    applyColor(fillFlag);
    beginShape(fillFlag ? TRIANGLE_STRIP: LINES);
    for(float a = b; a <= e;a += PMath::rad(5.f)){
        float _x = x + w * sin(a);
        float _y = y + h * cos(a);
        vertex(_x,_y);
        if(fillFlag){
            _x = x + w * sin(a+0.05f);
            _y = y + h * cos(a+0.05f);
            vertex(_x,_y);
            vertex(x,y);
        }
    }
    endShape();
}
// Draws a circle to the screen
void circle(float x,float y,float r){
    applyColor(fillFlag);
    arc(x,y,r,r,0,TWO_PI);
}

// Draws an ellipse (oval) in the display window
void ellipse(float x,float y,float w,float h){
    applyColor(fillFlag);
    arc(x,y,w,h,0,TWO_PI);
}

// Draws a line (a direct path between two points) to the screen
void line(float x1,float y1,float x2,float y2){
    applyColor(fillFlag);
    beginShape(LINES);
    vertex(x1,y1);
    vertex(x2,y2);
    endShape();
}

// Draws a point, a coordinate in space at the dimension of one pixel
void point(float x,float y){
    applyColor(fillFlag);
    beginShape(POINTS);
    vertex(x,y);
    endShape();
}

// A quad is a quadrilateral, a four sided polygon
void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    applyColor(fillFlag);
    beginShape(fillFlag ? QUADS : LINES);
    vertex(x1,y1);
    vertex(x2,y2);
    vertex(x3,y3);
    vertex(x4,y4);
    endShape();
}

// Draws a rectangle to the screen
void rect(float x,float y,float w,float h){
    applyColor(fillFlag);
    beginShape(fillFlag ? QUADS : LINES);
    vertex(x  ,y+h);
    vertex(x  ,y  );
    vertex(x+w,y  );
    vertex(x+w,y+h);
    endShape();
}

// Draws a square to the screen
void square(float x,float y,float r){
    applyColor(fillFlag);
    rect(x,y,r,r);
}

// A triangle is a plane created by connecting three points
void triangle(float x1,float y1,float x2,float y2,float x3,float y3){
    applyColor(fillFlag);
    beginShape(fillFlag ? TRIANGLES : LINES);
    vertex(x1,y1);
    vertex(x2,y2);
    vertex(x3,y3);
    endShape();
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
    applyColor(fillFlag);
    beginShape(LINES);
    float dt = 0.05f;
    for(float t = 0.0;t < 1.0f;t += dt){
        float _x1 = bezierPoint(x1,x2,x3,x4,t);
        float _y1 = bezierPoint(y1,y2,y3,y4,t);
        vertex(_x1,_y1);
        _x1 = bezierPoint(x1,x2,x3,x4,t+dt);
        _y1 = bezierPoint(y1,y2,y3,y4,t+dt);
        vertex(_x1,_y1);
    }
    endShape();
}

#endif
