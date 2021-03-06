#ifndef PROCESSING
#define PROCESSING

/**
 *  ==== CProcessing ====
 *  @version 1.4 beta 4
 */

#include <vector>
#include <string>

#include "PColor.h"
#include "PFont.h"
#include "PGL.h"
#include "PImage.h"
#include "PMath.h"
#include "PShader.h"
#include "PTime.h"
#include "PVector.h"

#define DOWN    GLFW_KEY_DOWN
#define UP      GLFW_KEY_UP
#define LEFT    GLFW_KEY_LEFT
#define RIGHT   GLFW_KEY_RIGHT
#define ESC     GLFW_KEY_ESCAPE

typedef bool boolean;                   // Java boolean type
#define null nullptr;                   // Java null pointer type
//#define length(x) (sizeof(x)/sizeof(x[0]))
//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
using namespace PMath;

extern void setup();                // setup function
extern void draw();                 // draw  function
extern void mousePressed();         // mouse pressed function
extern void mouseReleased();        // mouse released function
extern void mouseMoved();           // mouse moved function
extern void mouseDragged();         // mouse dragged function
#ifdef USE_KEYS
extern void keyPressed();           // key pressed function
#else
extern void keyPressed(){}           // key pressed function
#endif
extern void keyReleased();          // key released function
// ==============================================
std::string windowT = "";
GLFWwindow* window = null;          // window pointer
std::vector<std::string> args;      // program arguments
double mouseX = 0;                  // Mouse x coordinate
double mouseY = 0;                  // Mouse y coordinate
double pmouseX = 0;                 // Previous mouse x coordinate
double pmouseY = 0;                 // Previous mouse y coordinate
bool mousepressed = false;          // Whether any mouse button is pressed
bool keypressed = false;            // Whether a key was pressed
int mouseButton = 0;                // Which button is pressed
unsigned char key = 0;              // Which (ASCII) key was pressed
int keyCode = 0;                    // Code for the last pressed key
int width  = 128;                   // window width
int height = 128;                   // window height
int screenWidth = 0;                // window width
int screenHeight = 0;               // window height
unsigned config = 0;                // configuration flags
double framerate = 0;               // Frames per second
double framedelay = 0.0;            // Delay in seconds between frame
int frameCount = 0;                 // frames since start
//std::vector<Style> styles;        // Stack of styles
//PixelColorBuffer pixels;          // virtual array of pixels to get and put from (operated thru backbuffer)
//PImage screenBuffer;              // buffer of current window
bool looping = true;                // true makes display call itself
bool redrawflag = false;            // to draw next frame immediately
int initialized = false;            // glfw initialized yet

// ==============================================

void frameRate(int fr){
    if(fr != 0 || fr < 0)
        framerate = 1.0/fr;
    else
        frameRate(30);
}

void title(const std::string& t){
    if(window == nullptr)
        windowT = t;
    else
        glfwSetWindowTitle(window,windowT.c_str());
}

void fullScreen(){
    config = 1;
}

void size(int w,int h)  {
    width  = w;
    height = h;
}

void noLoop(){
    looping = false;
}

void err(int a,const char* b){
    printf("GLFW Error %i: \n%s\n",a,b);
}
void onKey(GLFWwindow* w, int _key, int _scancode, int _action, int _mods){
    key = _key;

#ifdef USE_KEYS
    keypressed = (bool)(_action == GLFW_PRESS);
    if(keypressed){
        keyCode = _key;
        keyPressed();
    }
    keypressed = false;
#endif
}

int main(int argc, char** argv){
    for(int i = 1; i < argc; i++)
        args.push_back(std::string(argv[i]));

    for(auto s : args)
        std::cout << s << "\n";

    title(" ");
    frameRate(60);
    setup();

    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwInitHint(GLFW_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    if(config)
        glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(),&screenWidth,&screenHeight,&width,&height);

    window = glfwCreateWindow(width,height,windowT.c_str(),config ? glfwGetPrimaryMonitor() : NULL,NULL);

    if(!window){
        err(0,"Window = 0");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetErrorCallback(err);
    glfwSetKeyCallback(window,onKey);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    initialized = true;
    initGL();
    glActiveTexture(GL_TEXTURE0);
    double lasttime = glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        double now = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r,bg.g,bg.b,bg.a);
        pmouseX = mouseX;
        pmouseY = mouseY;
        glfwGetCursorPos(window,&mouseX,&mouseY);
        int w,h;
        glfwGetWindowSize(window,&w,&h);
        if(w < 128) w = 128;width  = w;
        if(h < 128) h = 128;height = h;
        glfwSetWindowSize(window,w,h);

        if(looping){
            begin(w,h);
            draw();
            end();
            while (glfwGetTime() < lasttime + framerate) {}
            lasttime += framerate;
            glfwSwapBuffers(window);
        }

        key = 0;
        glfwPollEvents();
    }
    doneGL();
    args.clear();

    glfwTerminate();

    return 0;
}

#endif
