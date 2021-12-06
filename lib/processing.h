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

struct Context{
    int width,height;
    bool loop;
    float stroke;
    NVGcontext* nvgctx;
} ctx;


// must be initialized
void setup();
void draw();

// API
void size(int w,int h){
    ctx.width = w;
    ctx.height = h;
}
void stroke(float s){
    ctx.stroke = s;
}
void noStroke(){
    ctx.stroke = 0.f;
}
void noLoop(){
    ctx.loop = false;
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

    setup();

    if(!glfwInit() )error("Failed to initialize GLFW\n" );

    glfwWindowHint(GLFW_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_VERSION_MINOR,2);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( ctx.width, ctx.height, "App", nullptr, nullptr );
    if (!window) error( "Failed to open GLFW window\n" );

    glfwSetErrorCallback(error);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval( 1 );

    ctx.nvgctx = nvgCreateGL3(NVGcreateFlags::NVG_STENCIL_STROKES | NVGcreateFlags::NVG_ANTIALIAS);

    while( !glfwWindowShouldClose(window) ){
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        nvgBeginFrame(ctx.nvgctx,winWidth,winHeight,(float)winWidth / (float)fbWidth);
        if(ctx.loop) draw();
        nvgEndFrame(ctx.nvgctx);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    exit( EXIT_SUCCESS );

    return 0;
}

#endif
