#define USE_KEYS
#include <processing.h>
#define G 10

PVector pos,acc;

void setup(){
  size(600, 400);
  pos = createVector(width/2,height/2);
}

void draw(){
    background(10);
    circle(pos.x,pos.y,20);
}

void keyPressed(){
    if(keyCode == UP){
        pos.add(createVector(0,-G));
    }
    if(keyCode == DOWN){
        pos.add(createVector(0,G));
    }
    if(keyCode == LEFT){
        pos.add(createVector(-G,0));
    }
    if(keyCode == RIGHT){
        pos.add(createVector(G,0));
    }
}
