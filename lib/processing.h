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
#define PI_TWO PI*2.0
#define PI_Four PI_TWO*2.0


struct Context{
    int width,height;
    bool loop = true,fill = true;
    NVGcontext* nvgctx;
    NVGcolor color,bg;
    NVGpaint paint;
} ctx;

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
    ctx.width = w;
    ctx.height = h;
}
void strokeWeight(float s){
    nvgStrokeWidth(ctx.nvgctx,s);
}
void noStroke(){
    strokeWeight(0.f);
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
    ctx.color = nvgRGB(r,g,b);
}

void fill(float c){
    fill(c,c,c);
}

void stroke(float r,float g,float b){
    ctx.color = nvgRGB(r,g,b);
}

void stroke(float c){
    stroke(c,c,c);
}

void noFill(){
    ctx.fill = false;
}

void strokeCap(int c){
    nvgLineCap(ctx.nvgctx,c);
}

///////////////////////////
// shapes

void begin(){
    nvgBeginPath(ctx.nvgctx);
}

void end(){
    if(ctx.fill){
        nvgFillColor(ctx.nvgctx,ctx.color);
        nvgFill(ctx.nvgctx);
    }else{
        nvgStrokeColor(ctx.nvgctx, ctx.color);
        nvgStroke(ctx.nvgctx);
    }
}

void rect(float x,float y,float w, float h){
    begin();
    nvgRect(ctx.nvgctx,x,y,w,h);
    end();
}

void circle(float x,float y,float r){
    begin();
    nvgCircle(ctx.nvgctx,x,y,r);
    end();
}

void ellipse(float x,float y,float w,float h){
    begin();
    nvgEllipse(ctx.nvgctx,x,y,w,h);
    end();
}

void line(float x1,float y1,float x2,float y2){
    begin();
    nvgMoveTo(ctx.nvgctx,x1,y1);
    nvgLineTo(ctx.nvgctx,x2,y2);
    end();
}

void bezier(float x1,float y1,float x2,float y2,
            float x3,float y3,float x4,float y4){
    begin();
    nvgMoveTo(ctx.nvgctx,x1,y1);
    nvgBezierTo(ctx.nvgctx,x2,y2,x3,y3,x4,y4);
    end();
}



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

    window = glfwCreateWindow( ctx.width, ctx.height, "App", nullptr, nullptr );
    if (!window) error( "Failed to open GLFW window\n" );

    glfwSetErrorCallback(error);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval( 1 );

    ctx.nvgctx = nvgCreateGL3(NVGcreateFlags::NVG_ANTIALIAS);

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
