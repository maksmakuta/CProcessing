#ifndef PGL_H
#define PGL_H

#include <vector>
#include "PShader.h"
#include "PColor.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

color fillColor = color(255),strokeColor  = color(255),bg  = color(150);
bool fillFlag = true;

void applyColor(bool x){
    color c = x ? fillColor : strokeColor;
    glColor4f(c.r,c.g,c.b,c.a);
}

// Draws an arc in the display window
void arc(float x,float y,float w,float h,float b,float e){
    applyColor(fillFlag);
    glBegin(fillFlag ? GL_TRIANGLE_STRIP: GL_LINE_LOOP);
    for(float a = b; a <= e;a += PMath::rad(5.f)){
        float _x = x + w * sin(a);
        float _y = y + h * cos(a);
        glVertex2f(_x,_y);
        if(fillFlag){
        _x = x + w * sin(a+0.05f);
        _y = y + h * cos(a+0.05f);
        glVertex2f(_x,_y);
        glVertex2f(x,y);
        }
    }
    glEnd();
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
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}

// Draws a point, a coordinate in space at the dimension of one pixel
void point(float x,float y){
    applyColor(fillFlag);
    glBegin(GL_POINT);
    glVertex2f(x,y);
    glEnd();
}

// A quad is a quadrilateral, a four sided polygon
void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    applyColor(fillFlag);
    glBegin(fillFlag ? GL_POLYGON : GL_LINE_LOOP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);
    glVertex2f(x4,y4);
    glEnd();
}

// Draws a rectangle to the screen
void rect(float x,float y,float w,float h){
    applyColor(fillFlag);
    glBegin(fillFlag ? GL_POLYGON : GL_LINE_LOOP);
    glVertex2f(x,y+h);
    glVertex2f(x,y);
    glVertex2f(x+w,y);
    glVertex2f(x+w,y+h);

    glEnd();
}

// Draws a square to the screen
void square(float x,float y,float r){
    applyColor(fillFlag);
    rect(x,y,r,r);
}

// A triangle is a plane created by connecting three points
void triangle(float x1,float y1,float x2,float y2,float x3,float y3){
    applyColor(fillFlag);
    glBegin(fillFlag ? GL_TRIANGLES : GL_LINE_LOOP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);
    glEnd();
}

//Sets the resolution at which Beziers display
void bezierDetail(){}

//Evaluates the Bezier at point t for points a, b, c, d
float bezierPoint(float a,float b,float c,float d,float t){
    return  (powf((1.0 - t),3) * a) +
            (3.0 * t * pow((1.0 - t),2)*b) +
            (3.0 * t * t * (1.0 - t) *c) +
            (t*t*t*d);
}

//Calculates the tangent of a point on a Bezier curve
void bezierTangent(){}

//Draws a Bezier curve on the screen
void bezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){

    applyColor(fillFlag);
    glBegin(GL_LINE_STRIP);
    for(float t = 0.0;t < 1.0f+0.01f;t += 0.01f){
        float _x1 = bezierPoint(x1,x2,x3,x4,t);
        float _y1 = bezierPoint(y1,y2,y3,y4,t);
        glVertex2f(_x1,_y1);
    }
    glEnd();
}

#endif
