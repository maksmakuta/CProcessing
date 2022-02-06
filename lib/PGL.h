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


color fillColor = color(255),strokeColor  = color(255),bg  = color(150);
bool fillFlag = true;
PShape* tmp = NULL;
std::vector< PShape* > shapes;
std::vector<glm::mat4> matrices;
glm::mat4 matrix;

struct vertex{
    float x,y,z;
};

glm::vec4 vec(const color& c){
    return {c.r,c.g,c.b,c.a};
}

void draw(PShape* s){
    PShader sh = PShader::files("../Processing++/lib/glsl/def.vert","../Processing++/lib/glsl/def.frag");
    glUseProgram(sh.programID());
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PVector) * s->vertex.size(), s->vertex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(sh.attrLoc("ver"), 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);
    sh.mat4("matrix",matrix);
    sh.vec4("color",fillFlag ? vec(fillColor) : vec(strokeColor));
    glDrawArrays(s->type, 0, s->vertex.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void begin(float w,float h){
    glViewport(0,0,w,h);
    matrix = glm::ortho(0.f,w,h,0.f,-1.f,1.f);
}

void end(){
    for(PShape* s : shapes)
        draw(s);
    shapes.clear();
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
    beginShape(fillFlag ? TRIANGLE_STRIP : LINES);
    float d = PMath::rad(5.f);
    for(float a = b; a <= e;a += d){
        float _x = x + w * sin(a);
        float _y = y + h * cos(a);
        vertex(_x,_y);
        _x = x + w * sin(a+d+0.05f);
        _y = y + h * cos(a+d+0.05f);
        vertex(_x,_y);
        if(fillFlag) vertex(x,y);
    }
    endShape();
}
// Draws a circle to the screen
void circle(float x,float y,float r){
    arc(x,y,r,r,0,TWO_PI);
}

// Draws an ellipse (oval) in the display window
void ellipse(float x,float y,float w,float h){
    arc(x,y,w,h,0,TWO_PI);
}

// Draws a line (a direct path between two points) to the screen
void line(float x1,float y1,float x2,float y2){
    beginShape(LINES);
    vertex(x1,y1);
    vertex(x2,y2);
    endShape();
}

// Draws a point, a coordinate in space at the dimension of one pixel
void point(float x,float y){
    beginShape(POINTS);
    vertex(x,y);
    endShape();
}

// A quad is a quadrilateral, a four sided polygon
void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    beginShape(fillFlag ? QUADS : LINES);
    vertex(x1,y1);
    vertex(x2,y2);
    vertex(x3,y3);
    vertex(x4,y4);
    endShape();
}

// Draws a rectangle to the screen
void rect(float x,float y,float w,float h){
    beginShape(fillFlag ? QUADS : LINES);
    vertex(x  ,y+h);
    vertex(x  ,y  );
    vertex(x+w,y  );
    vertex(x+w,y+h);
    endShape();
}

// Draws a square to the screen
void square(float x,float y,float r){
    rect(x,y,r,r);
}

// A triangle is a plane created by connecting three points
void triangle(float x1,float y1,float x2,float y2,float x3,float y3){
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
