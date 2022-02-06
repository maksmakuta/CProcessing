#ifndef PROCESSING
#define PROCESSING

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

typedef bool boolean;                   // Java boolean type
#define null nullptr;                   // Java null pointer type

using namespace PMath;

/**
 *  ==== CProcessing ====
 *  @version 1.3 beta 10
 *
 */

extern void setup();                    // setup function
extern void draw();                     // draw  function
extern void mousePressed();             // mouse pressed function
extern void mouseReleased();            // mouse released function
extern void mouseMoved();               // mouse moved function
extern void mouseDragged();             // mouse dragged function
extern void keyPressed();               // key pressed function
extern void keyReleased();              // key released function

// ==============================================

GLFWwindow* window = null;          // window pointer
std::vector<std::string> args;      // program arguments
int mouseX = 0;                     // Mouse x coordinate
int mouseY = 0;                     // Mouse y coordinate
int pmouseX = 0;                    // Previous mouse x coordinate
int pmouseY = 0;                    // Previous mouse y coordinate
//bool mousePressed = false;          // Whether any mouse button is pressed
//bool keyPressed = false;            // Whether a key was pressed
int mouseButton = 0;                // Which button is pressed
unsigned char key = 0;              // Which (ASCII) key was pressed
int keyCode = 0;                    // Code for the last pressed key
int width = 0;                      // window width
int height = 0;                     // window height
int screenWidth = 0;                // window width
int screenHeight = 0;               // window height
unsigned config = 0;                // configuration flags
int framerate = 30;                 // Frames per second
double framedelay = 0.0;            // Delay in seconds between frame
int frameCount = 0;                 // frames since start
//std::vector<Style> styles;        // Stack of styles
//PixelColorBuffer pixels;          // virtual array of pixels to get and put from (operated thru backbuffer)
//PImage screenBuffer;              // buffer of current window
bool looping = true;                // true makes display call itself
bool redrawflag = false;            // to draw next frame immediately
int initialized = false;            // glfw initialized yet
// ==============================================

void size(int w,int h)  {
    width  = w;
    height = h;
}
void background(int a)  {
    bg = color(a);
}

void noLoop(){
    looping = false;
}

void noFill(){
    fillColor = color(1,1,1,0);
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
    strokeColor = color(1,1,1,0);
}

void stroke(int r,int g,int b,int a){
    fillFlag = false;
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

int main(int argc, char** argv){

    for(int i = 0; i < argc; i++) {
        args.push_back(std::string(argv[i]));
    }

    setup();


    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_VERSION_MAJOR, 2);
    //glfwWindowHint(GLFW_VERSION_MINOR, 1);

    window = glfwCreateWindow(width,height," ",NULL,NULL);
    if(!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // force 60 fps

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    while(!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r,bg.g,bg.b,bg.a);

        int w,h;
        glfwGetWindowSize(window,&w,&h);
        if(w < 128) w = 128;width  = w;
        if(h < 128) h = 128;height = h;
        glfwSetWindowSize(window,w,h);

        begin(w,h);
        {
            draw();
        }
        end();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shapes.clear();
    args.clear();
    glfwTerminate();

    return 0;
}

#endif
