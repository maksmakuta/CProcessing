//#define FPS
//#define USE_KEYS
#include "processing.h"
#include "extensions.h"

float s = 50;

void setup(){
    size(400,400);
}

void draw(){
    background(0);
    stroke(200);
    strokeWeight(5);
    circle(width/2.f, height / 2.f,s);
}

void keyPressed(){
    if(keyCode == UP){
        s+=10;
        if(s > height / 2.f)
            s = height/2.f;
    }
    if(keyCode == DOWN){
        s-=10;
        if(s < 10)
            s = 10;
    }
}
