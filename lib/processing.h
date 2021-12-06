#ifndef PROCESSING
#define PROCESSING

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define NANOVG_GL3_IMPLEMENTATION
#include "gl/nanovg.h"
#include "gl/nanovg_gl.h"

#define PI 3.1415926
#define HALF_PI PI/2.0
#define TWO_PI PI*2.0
#define QPI TWO_PI*2.0
#define CAPPA 0.5522847493f

#define radians(x) x*(PI/180)
#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b

struct Context{
    bool loop = true,fill = true;
    NVGcontext* nvgctx;
    NVGcolor fillColor,strokeColor,bg;
    NVGpaint paint;
} ctx;

    int width,height;
double mouseX,mouseY;

enum Cap{
    BUT     = NVGlineCap::NVG_BUTT,
    ROUND   = NVGlineCap::NVG_ROUND,
    SQUARE  = NVGlineCap::NVG_SQUARE,
    MITTER  = NVGlineCap::NVG_MITER,
    BEVEL   = NVGlineCap::NVG_BEVEL
};

// must be initialized
void setup();
void draw();

// API
void size(int w,int h){
    width = w;
    height = h;
}
void strokeWeight(float s){
    nvgStrokeWidth(ctx.nvgctx,s);
}
void noStroke(){
    ctx.strokeColor = ctx.bg;
}
void noFill(){
    ctx.fillColor = ctx.bg;
}
void noLoop(){
    ctx.loop = false;
}

///////////////////////////
// colors

void background(float r,float g,float b){
    ctx.bg = nvgRGB(r,g,b);
}

void background(float f){
   background(f,f,f);
}

void fill(float r,float g,float b){
    ctx.fillColor = nvgRGB(r,g,b);
}

void fill(float c){
    fill(c,c,c);
}

void stroke(float r,float g,float b){
    ctx.strokeColor = nvgRGB(r,g,b);
}

void stroke(float c){
    stroke(c,c,c);
}

void strokeCap(int c){
    nvgLineCap(ctx.nvgctx,c);
}

///////////////////////////
// shapes

void beginShape(){
    nvgBeginPath(ctx.nvgctx);
}

void endShape(){
    nvgFillColor(ctx.nvgctx,ctx.fillColor);
    nvgFill(ctx.nvgctx);
    nvgStrokeColor(ctx.nvgctx, ctx.strokeColor);
    nvgStroke(ctx.nvgctx);
}

void rect(float x,float y,float w, float h){
    beginShape();
    nvgRect(ctx.nvgctx,x,y,w,h);
    endShape();
}

void circle(float x,float y,float r){
    beginShape();
    nvgCircle(ctx.nvgctx,x,y,r);
    endShape();
}

void point(float x,float y){
    circle(x,y,1);
}

void ellipse(float x,float y,float w,float h){
    beginShape();
    nvgEllipse(ctx.nvgctx,x,y,w/2,h/2);
    endShape();
}

void line(float x1,float y1,float x2,float y2){
    beginShape();
    nvgMoveTo(ctx.nvgctx,x1,y1);
    nvgLineTo(ctx.nvgctx,x2,y2);
    endShape();
}

void bezier(float x1,float y1,float x2,float y2,
            float x3,float y3,float x4,float y4){
    beginShape();
    nvgMoveTo(ctx.nvgctx,x1,y1);
    nvgBezierTo(ctx.nvgctx,x2,y2,x3,y3,x4,y4);
    endShape();
}
// TODO arc
void arc(float cx,float cy,float rx,float ry,float a0,float a1){

  beginShape();
  nvgMoveTo(ctx.nvgctx,cx,cy);
  nvgLineTo(ctx.nvgctx,cx-rx,cy);
  nvgBezierTo(ctx.nvgctx,cx-rx, cy+ry*CAPPA, cx-rx*CAPPA, cy+ry, cx, cy+ry);
  endShape();

  fill(0);
  point(cx-rx,cy);
  point(cx-rx, cy+ry*CAPPA);
  point(cx-rx*CAPPA, cy+ry);
  point(cx, cy+ry);
}

////////////////////
// Math

template<class T> T map(T val,T rmin,T rmax,T min,T max){
        T OldRange = rmax - rmin;
        T NewRange = max - min;
        return (((val - rmin) * NewRange) / OldRange) + min;
}

float norm(float val,float min,float max){
    return map<float>(val,min,max,0.f,1.f);
}

////////////////////////////////////////////////////////////////

GLFWwindow* window = nullptr;

void error(const char* terr){
    fprintf(stderr,"%s\n",terr);
    if(window != nullptr) glfwTerminate();
    exit(EXIT_FAILURE);
}

void error(int a,const char* t){
    fprintf(stderr,"%i -> %s\n",a,t);
}

int main(){
    background(0);
    setup();

    if(!glfwInit() )error("Failed to initialize GLFW\n" );

    glfwWindowHint(GLFW_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_VERSION_MINOR,3);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(width,height, "App", nullptr, nullptr );
    if (!window) error( "Failed to open GLFW window\n" );

    glfwSetErrorCallback(error);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval( 1 );

    ctx.nvgctx = nvgCreateGL3(NVGcreateFlags::NVG_ANTIALIAS);
    //| NVGcreateFlags::NVG_STENCIL_STROKES);

    while( !glfwWindowShouldClose(window) ){
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glfwGetCursorPos(window,&mouseX,&mouseY);
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(ctx.bg.r,ctx.bg.g,ctx.bg.b,ctx.bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        nvgBeginFrame(ctx.nvgctx,winWidth,winHeight,(float)winWidth / (float)fbWidth);
        //if(ctx.loop)
            draw();
        nvgEndFrame(ctx.nvgctx);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    exit( EXIT_SUCCESS );

    return 0;
}

#endif
