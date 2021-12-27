#ifndef PROCESSING
#define PROCESSING

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <GL/glew.h>
#include "Math.h"
#define GLFW_INCLUDE_GLEW
#include <GLFW/glfw3.h>
#define NANOVG_GL3_IMPLEMENTATION
#include "ngl/nanovg.h"
#include "ngl/nanovg_gl.h"

#include "PFont.h"
#include "PVector.h"
#include "color.h"

typedef bool boolean; // add Java bool type

struct Context{
    bool fill = true,fullscreen = false;
    NVGcontext* nvgctx;
    NVGcolor fillColor,strokeColor,bg;
    NVGpaint paint;
    PFont font;
} ctx;

enum Key{
    None  = 0                     ,
    KEY_UP    = GLFW_KEY_UP           ,
    KEY_DOWN  = GLFW_KEY_DOWN         ,
    KEY_LEFT  = GLFW_KEY_LEFT         ,
    KEY_RIGHT = GLFW_KEY_RIGHT        ,
    KEY_ENTER = GLFW_KEY_ENTER        ,
    KEY_ESC   = GLFW_KEY_ESCAPE       ,
    KEY_SPACE = GLFW_KEY_SPACE        ,
    KEY_ALT   = GLFW_KEY_LEFT_ALT     ,
    KEY_SHIFT = GLFW_KEY_LEFT_SHIFT   ,
    KEY_CTRL  = GLFW_KEY_LEFT_CONTROL ,


    KEY_A = GLFW_KEY_A,
    KEY_B = GLFW_KEY_B,
    KEY_C = GLFW_KEY_C,
    KEY_D = GLFW_KEY_D,
    KEY_E = GLFW_KEY_E,
    KEY_F = GLFW_KEY_F,
    KEY_G = GLFW_KEY_G,
    KEY_H = GLFW_KEY_H,
    KEY_I = GLFW_KEY_I,
    KEY_J = GLFW_KEY_J,
    KEY_K = GLFW_KEY_K,
    KEY_L = GLFW_KEY_L,
    KEY_M = GLFW_KEY_M,
    KEY_N = GLFW_KEY_N,
    KEY_O = GLFW_KEY_O,
    KEY_P = GLFW_KEY_P,
    KEY_Q = GLFW_KEY_Q,
    KEY_R = GLFW_KEY_R,
    KEY_S = GLFW_KEY_S,
    KEY_T = GLFW_KEY_T,
    KEY_U = GLFW_KEY_U,
    KEY_V = GLFW_KEY_V,
    KEY_W = GLFW_KEY_W,
    KEY_X = GLFW_KEY_X,
    KEY_Y = GLFW_KEY_Y,
    KEY_Z = GLFW_KEY_Z,
};

GLFWwindow* window = nullptr;
int width = -1,height = -1;
double mouseX,mouseY;
double framerate = 60;
bool loop = true;
unsigned long long frameCount = 0;
bool _keyPressed = false;
bool mousePressed = false;
char key;
int keyCode = None;

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
#ifndef USE_KEY
void keyPressed(){}
#else
void keyPressed();
#endif

void exit(); // uses for delele all pointers if any

// API
void size(int w,int h){
    if(ctx.fullscreen == true){
        printf("Cannot use fullscreen() with size()\n");
        exit(0);
    }
    width = w;
    height = h;
}
void fullscreen(){
    ctx.fullscreen = true;
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
    loop = false;
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
    usleep(millis * 1000);
}

void frameRate(int fps){
    framerate = fps;
}

int millis(){
    return (int) ( glfwGetTime()*1000.0 );
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

void fill(color c){
    fill(c.r,c.g,c.b);
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

char parse(int key);

void onMouse(GLFWwindow* window, int button, int action, int mods){
    if (action == GLFW_PRESS)
        mousePressed = true;


    mousePressed = false;
}

void onKey(GLFWwindow* window, int _key, int _scancode, int _action, int _mods){
    if(_action == GLFW_PRESS){
        _keyPressed = true;
        keyCode = _key;
        key = parse(_key);
        keyPressed();
    }

    _keyPressed = false;
    keyCode = None;
    key = 0;
}

int main(){
    background(0);
    setup();

    if(!glfwInit())error("Failed to initialize GLFW\n" );

    glfwWindowHint(GLFW_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_VERSION_MINOR,3);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    GLFWmonitor* m = nullptr;
    if(ctx.fullscreen){
        m = glfwGetPrimaryMonitor();
        glfwGetMonitorWorkarea(m,nullptr,nullptr,&width,&height);
    }

    window = glfwCreateWindow(width,height, "App",m,nullptr);
    if (!window) error( "Failed to open GLFW window\n" );

    //glfwSetErrorCallback(error);
    glfwSetKeyCallback(window,onKey);
    glfwSetMouseButtonCallback(window, onMouse);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(0);

    double lasttime = glfwGetTime();

    ctx.nvgctx = nvgCreateGL3(NVGcreateFlags::NVG_ANTIALIAS | NVGcreateFlags::NVG_STENCIL_STROKES);

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
    exit();
    glfwTerminate();
    exit( EXIT_SUCCESS );

    return 0;
}


char parse(int key){
    bool b = glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
             glfwGetKey(window,GLFW_MOD_CAPS_LOCK ) == GLFW_PRESS ;
    switch (key) {
        case GLFW_KEY_0: return '0';
        case GLFW_KEY_1: return '1';
        case GLFW_KEY_2: return '2';
        case GLFW_KEY_3: return '3';
        case GLFW_KEY_4: return '4';
        case GLFW_KEY_5: return '5';
        case GLFW_KEY_6: return '6';
        case GLFW_KEY_7: return '7';
        case GLFW_KEY_8: return '8';
        case GLFW_KEY_9: return '9';

        case GLFW_KEY_A: return b ? 'A' : 'a';
        case GLFW_KEY_B: return b ? 'B' : 'b';
        case GLFW_KEY_C: return b ? 'C' : 'c';
        case GLFW_KEY_D: return b ? 'D' : 'd';
        case GLFW_KEY_E: return b ? 'E' : 'e';
        case GLFW_KEY_F: return b ? 'F' : 'f';
        case GLFW_KEY_G: return b ? 'G' : 'g';
        case GLFW_KEY_H: return b ? 'H' : 'h';
        case GLFW_KEY_I: return b ? 'I' : 'i';
        case GLFW_KEY_J: return b ? 'J' : 'j';
        case GLFW_KEY_K: return b ? 'K' : 'k';
        case GLFW_KEY_L: return b ? 'L' : 'l';
        case GLFW_KEY_M: return b ? 'M' : 'm';
        case GLFW_KEY_N: return b ? 'N' : 'n';
        case GLFW_KEY_O: return b ? 'O' : 'o';
        case GLFW_KEY_P: return b ? 'P' : 'p';
        case GLFW_KEY_Q: return b ? 'Q' : 'q';
        case GLFW_KEY_R: return b ? 'R' : 'r';
        case GLFW_KEY_S: return b ? 'S' : 's';
        case GLFW_KEY_T: return b ? 'T' : 't';
        case GLFW_KEY_U: return b ? 'U' : 'u';
        case GLFW_KEY_V: return b ? 'V' : 'v';
        case GLFW_KEY_W: return b ? 'W' : 'w';
        case GLFW_KEY_X: return b ? 'X' : 'x';
        case GLFW_KEY_Y: return b ? 'Y' : 'y';
        case GLFW_KEY_Z: return b ? 'Z' : 'z';
    }
    return 0;
}

#endif
