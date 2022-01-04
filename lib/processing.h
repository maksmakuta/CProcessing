#ifndef PROCESSING_H
#define PROCESSING_H

/* ============ Processing for C/C++ =================
 * @version 0.0.1
 *
 * TODO list:
 * - context
 * - shaders
 * - 2d renderer:
 * -- rect,circe,ellipse, custom shapes
 * -- textures
 * -- moveTo,lineTo,bezierTo
 * -- gradients
 * - 3d renderer
 * - custom types
 * ============        macros        =================
 * GC           -> for delete data          --> gc();
 * KEY          -> for key handle           --> pressedKey();
 * CXX          -> use operators for some classes
 */

#include "pGL.h"
#include <GLFW/glfw3.h>

#include "Math.h"
#include "PFont.h"
#include "PVector.h"
#include "color.h"

GLFWwindow* window = nullptr;

void setup();
void draw();

// ============       variables     =================

int width,height;
color bg,fillColor,strokeColor;

// ============  default functions  =================

void size(int w,int h){
    width = w;
    height = h;
}

void background(int c){
    bg = color(c);
}

void background(int r,int g,int b){
    bg = color(r,g,b);
}

void background(color c){
    bg = c;
}

// ============    glfw functions   =================

void error(const char* terr){
    fprintf(stderr,"%s\n",terr);
    if(window != nullptr) glfwTerminate();
    exit(EXIT_FAILURE);
}
// ============        main()       =================

int main(){
    background(0);

    if(!glfwInit())error("Failed to initialize GLFW\n" );

    glfwWindowHint(GLFW_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_VERSION_MINOR,3);
    //glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    GLFWmonitor* m = nullptr;
    //if(ctx.fullscreen){
    //    m = glfwGetPrimaryMonitor();
    //    glfwGetMonitorWorkarea(m,nullptr,nullptr,&width,&height);
    //}
    window = glfwCreateWindow(1,1," ",m,nullptr);
    if (!window) error( "Failed to open GLFW window\n" );

    //glfwSetErrorCallback(error);
    //glfwSetKeyCallback(window,onKey);
    //glfwSetMouseButtonCallback(window, onMouse);
    //glfwSetWindowSizeCallback(window,onResize);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glfwSwapInterval(0);

    double lasttime = glfwGetTime();

    //ctx.nvgctx = nvgCreateGL3(NVGcreateFlags::NVG_ANTIALIAS | NVGcreateFlags::NVG_STENCIL_STROKES);

    setup();
    //if(ctx.fullscreen){
    //    m = glfwGetPrimaryMonitor();
    //    glfwGetMonitorWorkarea(m,nullptr,nullptr,&width,&height);
    //}
    glfwSetWindowSize(window,width,height);

    while( !glfwWindowShouldClose(window) ){

        //while (glfwGetTime() < lasttime + 1.0/framerate) {
        //    // TODO: Put the thread to sleep, yield, or simply do nothing
        //}
        //lasttime += 1.0/framerate;

        int winWidth, winHeight;
        int fbWidth, fbHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        //glfwGetCursorPos(window,&mouseX,&mouseY);
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(bg.r,bg.g,bg.b,bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //nvgBeginFrame(ctx.nvgctx,winWidth,winHeight,(float)winWidth / (float)fbWidth);
            draw();
        //nvgEndFrame(ctx.nvgctx);
        glfwSwapBuffers(window);
        glfwPollEvents();
        //frameCount++;
    }
#ifdef GC
       gc();
#endif
    glfwTerminate();
    exit( EXIT_SUCCESS );
    return 0;
}

#endif
