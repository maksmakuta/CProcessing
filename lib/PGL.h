#ifndef PGL_H
#define PGL_H

// uses OpenGL 3.3 by default

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include "PShader.h"
#include "PImage.h"
#include "PShape.h"
#include "PColor.h"
#include "PStroker.h"

#define CENTER  0
#define RADIUS  1
#define CORNER  2
#define CORNERS 3

color fillColor,strokeColor,bg;
bool fillFlag,strokeFlag,builder;
PShape tmp;
std::vector<glm::mat4> matrices;
std::vector<PImage> textures;
glm::mat4 matrix;
float strokeWidth;
int cap,join,rMode,eMode,call = 0;
unsigned int textureID;
PShader sh;

glm::vec4 vec(const color& c){
    return {c.r,c.g,c.b,c.a};
}

void initGL(){
    fillColor = color(255);
    strokeColor  = color(255);
    bg  = color(150);
    fillFlag = false;
    strokeFlag = false;
    builder = false;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * s.size(), s.data().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);
    sh.umat4("matrix",matrix);
    sh.uvec4("color",vec(s.getColor()));
    sh.uint ("call",call);
    sh.uint ("texID",textureID);
    glDrawArrays(s.type(), 0, s.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
    //s.done();
    if(call != 0)
        call = 0;
}

PImage find(const std::string& i){
    bool found = false;
    if(textures.empty()){
        PImage img;
        img.load(i);
        textures.push_back(img);
        return img;
    }else{
        for(int a = 0;a < textures.size();a++){
            PImage img = textures[a];
            if(img.getName() == i){
                found = true;
                if(img.isLoaded())
                    return img;
                else{
                    img.load(img.getName());
                    textures[a] = img;
                    return img;
                }
            }
        }
    }
    return PImage();
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

void arc(float x,float y,float w,float h,float b,float e,float step){
    float d = PMath::radians(step);
    if(fillFlag){
        beginShape(TRIANGLE_FAN);
        tmp.setColor(fillColor);
        vertex(x,y);
        for(float a = b; a <= e;a += d){
            float _x = x + w * sin(a);
            float _y = y + h * cos(a);
            vertex(_x,_y);
        }
        endShape();
    }
    if(strokeFlag){
        tmp = PShape(TRIANGLES);
        tmp.setColor(strokeColor);
        for(float a = b; a <= e;a += d){
            float _x = x + w * sin(a);
            float _y = y + h * cos(a);
            tmp.push(_x,_y,0.f);
        }
        draw(strokify(tmp,strokeWidth,cap,join));
    }
}

// Draws an arc in the display window
void arc(float x,float y,float w,float h,float b,float e){
    arc(x,y,w,h,b,e,5.f);
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
    tmp = PShape(LINES);
    tmp.setColor(strokeColor);
    tmp.push(x1,y1,0.f);
    tmp.push(x2,y2,0.f);
    draw(strokify(tmp,strokeWidth,cap,join));
    //delete tmp;
}

// Draws a point, a coordinate in space at the dimension of one pixel
void point(float x,float y){
    tmp = PShape(TRIANGLES);
    tmp.setColor(strokeColor);
    tmp.push(x,y,0.f);
    draw(strokify(tmp,strokeWidth,cap,join));
    //delete tmp;
}

// A quad is a quadrilateral, a four sided polygon
void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    if(fillFlag){
        beginShape(QUADS);
        tmp.setColor(fillColor);
        vertex(x1,y1);
        vertex(x2,y2);
        vertex(x3,y3);
        vertex(x4,y4);
        endShape();
    }
    if(strokeFlag){
        tmp = PShape(TRIANGLES);
        tmp.setColor(strokeColor);
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
    if(fillFlag){
        beginShape(QUADS);
        tmp.setColor(fillColor);
        vertex(x,y);
        vertex(x+w,y);
        vertex(x+w,y+h);
        vertex(x,y+h);
        endShape();
    }
    if(strokeFlag){
        tmp = PShape();
        tmp.setColor(strokeColor);
        tmp.push(x,y,0.f);
        tmp.push(x+w,y,0.f);
        tmp.push(x+w,y+h,0.f);
        tmp.push(x,y+h,0.f);
        tmp.push(x,y,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
    }
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
    if(fillFlag){
        tmp = PShape(SHAPE_TYPE::TRIANGLE_FAN);
        tmp.setColor(fillColor);
        if(fillFlag) tmp.push(x+w/2,y+h/2,0.f);
        arc(tmp,x+w-rbr ,y+h-rbr,rbr,0       ,PI/2      ); //br
        arc(tmp,x+w-rtr ,y+rtr  ,rtr,PI/2    ,PI        ); //tr
        arc(tmp,x+rtl   ,y+rtl  ,rtl,PI      ,(3*PI)/2  ); //tl
        arc(tmp,x+rbl   ,y+h-rbl,rbl,(3*PI)/2,2*PI      ); //bl
        tmp.push(x+w-rbr ,y+h,0.f);
        draw(tmp);
    }
    if(strokeFlag){
        tmp = PShape(SHAPE_TYPE::TRIANGLES);
        tmp.setColor(strokeColor);
        arc(tmp,x+w-rbr ,y+h-rbr,rbr,0       ,PI/2      ); //br
        arc(tmp,x+w-rtr ,y+rtr  ,rtr,PI/2    ,PI        ); //tr
        arc(tmp,x+rtl   ,y+rtl  ,rtl,PI      ,(3*PI)/2  ); //tl
        arc(tmp,x+rbl   ,y+h-rbl,rbl,(3*PI)/2,2*PI      ); //bl
        tmp.push(x+w-rbr ,y+h,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
    }
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
        tmp.setColor(fillColor);
        vertex(x1,y1);
        vertex(x2,y2);
        vertex(x3,y3);
        endShape();
    }
    if(strokeFlag){
        tmp = PShape(TRIANGLES);
        tmp.setColor(strokeColor);
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
    tmp.setColor(strokeColor);
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

void image(const PImage& img,float x,float y,float w,float h){
    tmp = PShape(QUADS);
    tmp.push(x  ,y  ,0,0);
    tmp.push(x+w,y  ,1,0);
    tmp.push(x+w,y+h,1,1);
    tmp.push(x  ,y+h,0,1);
    textureID = find(img.getName()).getID();
    //call = 1;
    draw(tmp);
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

void background(color c){
    bg = c;
}
void background(int r,int g,int b,int a){
    bg = color(r,g,b,a);
}
void background(int r,int g,int b){
    background(r,g,b,255);
}
void background(int v,int a){
    background(v,v,v,a);
}
void background(int v){
    background(v,255);
}
void noFill(){
    fillFlag = false;
    fillColor = color(0);
}
void fill(color c){
    fillFlag = true;
    fillColor = c;
}
void fill(int r,int g,int b,int a){
    fillFlag = true;
    fillColor = color(r,g,b,a);
}
void fill(int r,int g,int b){
    fill(r,g,b,255);
}
void fill(int v,int a){
    fill(v,v,v,a);
}
void fill(int v){
    fill(v,255);
}
void noStroke(){
    strokeFlag = false;
    strokeColor = color(0);
}
void stroke(color c){
    strokeFlag = true;
    strokeColor = c;
}
void stroke(int r,int g,int b,int a){
    strokeFlag = true;
    strokeColor = color(r,g,b,a);
}
void stroke(int r,int g,int b){
    stroke(r,g,b,255);
}
void stroke(int v,int a){
    stroke(v,v,v,a);
}
void stroke(int v){
    stroke(v,255);
}

#endif
