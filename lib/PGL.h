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
#include "PolyLine2D/Polyline2D.h"

color fillColor = color(255),strokeColor  = color(255),bg  = color(150);
bool fillFlag = true;
PShape tmp;
std::vector<glm::mat4> matrices;
glm::mat4 matrix;
float strokeWidth = 1.0f;
int cap = 0,join = 0;

PShader sh;

struct vertex{
    float x,y,z;
};

glm::vec4 vec(const color& c){
    return {c.r,c.g,c.b,c.a};
}

void initGL(){
    sh = PShader::files("../Processing++/lib/glsl/def.vert","../Processing++/lib/glsl/def.frag");
}

void draw(PShape s){
    glUseProgram(sh.programID());
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PVector) * s.vertex.size(), s.vertex.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(sh.attrLoc("ver"), 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);
    sh.mat4("matrix",matrix);
    sh.vec4("color",fillFlag ? vec(fillColor) : vec(strokeColor));
    glDrawArrays(s.type, 0, s.vertex.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    s.clear();
}

void strokeWeigth(float w){
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
    tmp = PShape(t);
}

void vertex(float x,float y,float z = 0.0f){
    if(!tmp.vertex.empty())
        tmp.push(x,y,z);
    else
        std::cerr << "vertex() must be inside shapeBegin() and shapeEnd() functions\n";
}

void endShape(){
    draw(tmp);
    tmp.clear();
}

PShape strokify(PShape &path,float w,int cap, int join){
    using namespace crushedpixel;
    Polyline2D::JointStyle _join;
    switch(join){
        case 0 : _join = Polyline2D::JointStyle::MITER; break;
        case 1 : _join = Polyline2D::JointStyle::BEVEL; break;
        case 2 : _join = Polyline2D::JointStyle::ROUND; break;
        default: _join = Polyline2D::JointStyle::MITER; break;
    }
    Polyline2D::EndCapStyle _cap;
    if(path.loop())
        _cap = Polyline2D::EndCapStyle::JOINT;
    else{
        switch(join){
            case 0 : _cap = Polyline2D::EndCapStyle::ROUND;  break;
            case 1 : _cap = Polyline2D::EndCapStyle::SQUARE; break;
            case 2 : _cap = Polyline2D::EndCapStyle::BUTT;   break;
            default: _cap = Polyline2D::EndCapStyle::ROUND;  break;
        }
    }
    path.vertex = Polyline2D::create(path.vertex, w, _join,_cap);
    return path;
}

#define MITER   0
#define BEVEL   1
#define ROUND   2
#define SQUARE  1
#define PROJECT 2

// Draws an arc in the display window
void arc(float x,float y,float w,float h,float b,float e){
    float d = PMath::radians(5.f);
    if(!fillFlag){
        PShape sh(TRIANGLES);
        for(float a = b; a <= e;a += d){
            float _x = x + w * sin(a);
            float _y = y + h * cos(a);
            sh.push(_x,_y,0.f);
        }
        draw(strokify(sh,strokeWidth,cap,join));
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

// Draws an ellipse (oval) in the display window
void ellipse(float x,float y,float w,float h){
    arc(x,y,w,h,0,TWO_PI);
}

// Draws a line (a direct path between two points) to the screen
void line(float x1,float y1,float x2,float y2){
    PShape sh(TRIANGLES);
    sh.push(x1,y1,0.f);
    sh.push(x2,y2,0.f);
    draw(strokify(sh,strokeWidth,cap,join));
}

// Draws a point, a coordinate in space at the dimension of one pixel
void point(float x,float y){
    PShape sh(TRIANGLES);
    sh.push(x,y,0.f);
    draw(strokify(sh,strokeWidth,cap,join));
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
        PShape sh(TRIANGLES);
        sh.push(x1,y1,0.f);
        sh.push(x2,y2,0.f);
        sh.push(x3,y3,0.f);
        sh.push(x4,y4,0.f);
        draw(strokify(sh,strokeWidth,cap,join));
    }
}

// Draws a rectangle to the screen
void rect(float x,float y,float w,float h){
    quad(
        x  ,y+h,
        x  ,y  ,
        x+w,y  ,
        x+w,y+h
    );
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
        PShape sh(TRIANGLES);
        sh.push(x1,y1,0.f);
        sh.push(x2,y2,0.f);
        sh.push(x3,y3,0.f);
        draw(strokify(sh,strokeWidth,cap,join));
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
    PShape sh(LINES);
    float dt = 0.01f;
    for(float t = 0.0;t < 1.0f;t += dt){
        float _x1 = bezierPoint(x1,x2,x3,x4,t);
        float _y1 = bezierPoint(y1,y2,y3,y4,t);
        sh.push(_x1,_y1,0.f);
        _x1 = bezierPoint(x1,x2,x3,x4,t+dt);
        _y1 = bezierPoint(y1,y2,y3,y4,t+dt);
        sh.push(_x1,_y1,0.f);
    }
    draw(strokify(sh,strokeWidth,cap,join));
    sh.clear();
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
