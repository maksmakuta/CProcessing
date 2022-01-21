#ifndef PROCESSING
#define PROCESSING

#include <vector>
#include <string>
#include "PColor.h"
#include "PGL.h"

typedef bool boolean;                   // Java boolean type
#define null nullptr;                   // Java null pointer type

/**
 *  ==== CProcessing ====
 *  @version 1.3 beta 6
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
struct processing{
    GLFWwindow* window = null;          // window pointer
    std::vector<std::string> args;      // program arguments
    int mouseX = 0;                     // Mouse x coordinate
    int mouseY = 0;                     // Mouse y coordinate
    int pmouseX = 0;                    // Previous mouse x coordinate
    int pmouseY = 0;                    // Previous mouse y coordinate
    bool mousePressed = false;          // Whether any mouse button is pressed
    int mouseButton = 0;                // Which button is pressed
    bool keyPressed = false;            // Whether a key was pressed
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
} state{};
// ==============================================

void redraw()           {}
void size(int w,int h)  {
    state.width  = w;
    state.height = h;
}
void background(int a)  {
    bg = color(a);
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

// Returns the number of milliseconds (thousandths of a second) since starting an applet
int millis(){
    return (int)(glfwGetTime() * 1000);
}

int main(int argc, char** argv){

    for(int i = 0; i < argc; i++) {
        state.args.push_back(std::string(argv[i]));
    }

    setup();


    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_VERSION_MAJOR, 2);
    //glfwWindowHint(GLFW_VERSION_MINOR, 1);

    state.window = glfwCreateWindow(state.width, state.height," ",NULL,NULL);
    if(!state.window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(state.window);
    glfwSwapInterval(0);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    while(!glfwWindowShouldClose(state.window)){

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r,bg.g,bg.b,bg.a);

        int w,h;
        glfwGetWindowSize(state.window,&w,&h);

        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,w,h,0,0,1);
        glMatrixMode(GL_MODELVIEW);

        draw();

        glfwPollEvents();
        glfwSwapBuffers(state.window);
    }

    return 0;
}

#endif
