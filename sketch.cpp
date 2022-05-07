#include <processing.h>

void setup() {
    size(400,400);
}


void draw() {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    background(20);
    stroke(200);
    strokeWeight(10);
    line(20,20,width-20,height - 20);
}

