#ifndef PROCESSING
#define PROCESSING

#include <string>
#include <vector>


/**
 *  ==== CProcessing ====
 *  @version 1.3 beta 1
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

struct processing{
// ==============================================
std::vector<std::string> args;   // program arguments
int mouseX = 0;                  // Mouse x coordinate
int mouseY = 0;                  // Mouse y coordinate
int pmouseX = 0;                 // Previous mouse x coordinate
int pmouseY = 0;                 // Previous mouse y coordinate
bool mousePressed = false;       // Whether any mouse button is pressed
int mouseButton = 0;             // Which button is pressed
bool keyPressed = false;         // Whether a key was pressed
unsigned char key = 0;           // Which (ASCII) key was pressed
int keyCode = 0;                 // Code for the last pressed key
int width = 0;                   // window width
int height = 0;                  // window height
int screenWidth = 0;             // window width
int screenHeight = 0;            // window height
unsigned config = 0;             // configuration flags
int framerate = 30;              // Frames per second
double framedelay = 0.0;         // Delay in seconds between frame
int frameCount = 0;              // frames since start
//std::vector<Style> styles;     // Stack of styles
//PixelColorBuffer pixels;       // virtual array of pixels to get and put from (operated thru backbuffer)
//PImage screenBuffer;           // buffer of current window
bool looping = true;             // true makes display call itself
bool redrawflag = false;         // to draw next frame immediately
int initialized = false;         // glfw initialized yet
// ==============================================
} state{};

void redraw()           {}
void run()              {}
void size(int w,int h)  {}
void background(int a)  {}

int main(int argc, char** argv){

    for(int i = 0; i < argc; i++) {
        state.args.push_back(std::string(argv[i]));
    }

    return 0;
}

#endif
