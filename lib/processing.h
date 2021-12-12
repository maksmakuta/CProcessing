#ifndef PROCESSING
#define PROCESSING

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define NANOVG_GL3_IMPLEMENTATION
#include "ngl/nanovg.h"
#include "ngl/nanovg_gl.h"

#define PI 3.1415926
#define HALF_PI PI/2.0
#define TWO_PI PI*2.0
#define CAPPA 0.5522847493f

#define radians(x) x*(PI/180)
#define degrees(x) x*(180/PI)
#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b

struct Context{
    bool loop = true,fill = true;
    NVGcontext* nvgctx;
    NVGcolor fillColor,strokeColor,bg;
    NVGpaint paint;
} ctx;


GLFWwindow* window = nullptr;
    int width,height;
double mouseX,mouseY;
double framerate = 60;
unsigned long long frameCount = 0;


enum Cap{
    BUT     = NVGlineCap::NVG_BUTT,
    ROUND   = NVGlineCap::NVG_ROUND,
    SQUARE  = NVGlineCap::NVG_SQUARE,
    MITTER  = NVGlineCap::NVG_MITER,
    BEVEL   = NVGlineCap::NVG_BEVEL
};

enum CursorType{
    ARROW = GLFW_ARROW_CURSOR       ,
    CROSS = GLFW_CROSSHAIR_CURSOR   ,
    HAND  = GLFW_HAND_CURSOR,
    MOVE  = CROSS,
    TEXT  = GLFW_IBEAM_CURSOR,
    WAIT  = ARROW
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
    strokeWeight(1);
}
void noFill(){
    ctx.fillColor = ctx.bg;
}
void noLoop(){
    ctx.loop = false;
}

void noCursor(){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void cursor(CursorType t = ARROW){
    GLFWcursor* curs = glfwCreateStandardCursor(t);
    glfwSetCursor(window,curs);
}
void cursor(){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cursor(ARROW);
}
void delay(int millis){

}

void frameRate(int fps){
    framerate = fps;
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

void arc(float cx,float cy,float r,float a0,float a1){
    beginShape();
    nvgArc(ctx.nvgctx,cx,cy,r,a0,a1,a1-a0 > 0 ? NVG_CCW : NVG_CW);
    endShape();
}

void triangle(float x1,float y1,float x2,float y2,float x3,float y3){

  beginShape();
  nvgMoveTo(ctx.nvgctx,x1,y1);
  nvgLineTo(ctx.nvgctx,x2,y2);
  nvgLineTo(ctx.nvgctx,x3,y3);
  endShape();

}

void quad(float x1,float y1,
          float x2,float y2,
          float x3,float y3,
          float x4,float y4){

  beginShape();
  nvgMoveTo(ctx.nvgctx,x1,y1);
  nvgLineTo(ctx.nvgctx,x2,y2);
  nvgLineTo(ctx.nvgctx,x3,y3);
  nvgLineTo(ctx.nvgctx,x4,y4);
  endShape();
}

///////////////////
// Matrix

void pushMatrix(){
    nvgSave(ctx.nvgctx);
}

void popMatrix(){
    nvgRestore(ctx.nvgctx);
}


void translate(float x,float y){
    nvgTranslate(ctx.nvgctx,x,y);
}

void rotate(float a){
    nvgRotate(ctx.nvgctx,a);
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

void center(GLFWwindow* w,int _w,int _h){
  const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int window_width = mode->width;
    int window_height = mode->height;
    glfwSetWindowPos(w,window_width/2 - _w/2,window_height/2 - _h/2);
}

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
    glfwWindowHint(GLFW_RESIZABLE,0);

    window = glfwCreateWindow(width,height, "App", nullptr, nullptr );
    if (!window) error( "Failed to open GLFW window\n" );

    center(window,width,height);

    glfwSetErrorCallback(error);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(0);

    double lasttime = glfwGetTime();
    
    ctx.nvgctx = nvgCreateGL3(NVGcreateFlags::NVG_ANTIALIAS);

    while( !glfwWindowShouldClose(window) ){

        while (glfwGetTime() < lasttime + 1.0/framerate) {
            // TODO: Put the thread to sleep, yield, or simply do nothing
        }
        lasttime += 1.0/framerate;

        int winWidth, winHeight;
        int fbWidth, fbHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glfwGetCursorPos(window,&mouseX,&mouseY);
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(ctx.bg.r,ctx.bg.g,ctx.bg.b,ctx.bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        nvgBeginFrame(ctx.nvgctx,winWidth,winHeight,(float)winWidth / (float)fbWidth);
            draw();
        nvgEndFrame(ctx.nvgctx);
        glfwSwapBuffers(window);
        glfwPollEvents();
        frameCount++;
    }

    glfwTerminate();
    exit( EXIT_SUCCESS );

    return 0;
}

#endif
