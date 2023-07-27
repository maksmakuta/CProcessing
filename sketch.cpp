#include <processing.h>

void setup(){
    size(400,400);
}

std::vector<PVector> data = {
    PVector(140, 80),
    PVector(260, 200),
    PVector(140, 320)
};

void draw(){
    background(0);
    noFill();
    stroke(200);
    strokeWeight(40.0);
    strokeJoin(MITER);
    beginShape();
    for(int a = 0;a < 3;a++){
        vertex(data[a].x,data[a].y);
    }
    endShape();
    stroke(200,50,50);
    strokeWeight(5);
    for(int a = 0;a < 3;a++){
        point(data[a].x,data[a].y);
    }
}
