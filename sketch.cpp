//#define USE_KEYS
#include "processing.h"
#include "extensions.h"

#define SIZE 7

float angles[SIZE];
float speeds[SIZE];

void setup(){
    size(400,400);
    for(int i = 0 ; i < SIZE ;i++){
        angles[i] = random(PI);
        speeds[i] = random(-0.005,0.005);
    }
}

void draw(){
    background(0);
    strokeWeight(3.f);
    stroke(255);
    circle(width / 2.f, height / 2.f ,height/3.f );

    for(int i = 0 ; i < SIZE - 1 ;i++){
        float x1 = (width / 2.f) + sin(angles[i]) * (height/3.f);
        float y1 = (height / 2.f) + cos(angles[i]) * (height/3.f);

        float x2 = (width / 2.f) + sin(angles[i+1]) * (height/3.f);
        float y2 = (height / 2.f) + cos(angles[i+1]) * (height/3.f);

        line(x1,y1,x2,y2);

        angles[i] += speeds[i];
        if(i == SIZE - 1)
            angles[i+1] += speeds[i+1];
    }
}

void keyPressed(){

}
