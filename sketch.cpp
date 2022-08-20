//#define FPS
#include <processing.h>

#define W 200
#define H 200
#define R 100

PImage i;

void setup(){
    size(400,400);
    i = PImage("../Processing++/assets/wall.jpg");
}
/*mouseX-(W/2),mouseY-(H/2)*/
void draw(){
    i.load();
    background(10);
    image(i,0,0,W,H);
    image(i,W+W/2,0+H/2,R);
    image(i,0,H,W,H,R);
    image(i,W,H,W,H,R,R,R,R/2);
}
