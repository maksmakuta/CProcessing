#include <processing.h>

void setup(){
    size(600,400);
}

void draw(){
    background(0);
    noStroke();

    //fill(200);
    //rect(100,100,200,250);

    //fill(100,56,156);
    //rect(0,0,100,50);

    float s = map(mouseX,0.f,width,0.f,TAU);
    float f = map(mouseY,0.f,height,0.f,TAU);

    fill(200);
    arc(width/2,height/2,100,60,s,f);

    //fill(150,0,25);
    //triangle(10,10,25,25,25,10);

}
