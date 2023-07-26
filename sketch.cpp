#include <processing.h>

void setup(){
    size(600,400);
    strokeWeight(20);
}

void draw(){
    background(0);
    noFill();
    fill(200,100,20);
    point(width/2,height/2);
    point(mouseX,mouseY);

    stroke(200);
    fill(200);
    line(width/2,height/2,mouseX,mouseY);
}
