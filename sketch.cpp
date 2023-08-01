#include <processing.h>

void setup(){
    size(800,600);
    strokeJoin(MITER);
    strokeCap(ROUND);
}

std::vector<PVector> data = {
    PVector(140, 80),
    PVector(260, 200),
    PVector(140, 250)
};

void draw(){
    background(0);
    noFill();
    stroke(200);
    strokeWeight(40.0);
    line(50,30,250,30);
    line(300,30,500,30);
    line(550,30,750,30);
    beginShape();
    for(int a = 0;a < 3;a++){
        vertex(data[a].x,data[a].y);
    }
    endShape();

    strokeJoin(MITER);
    strokeCap(ROUND);


    stroke(200,50,50);
    strokeWeight(5);
    for(int a = 0;a < 3;a++){
        point(data[a].x,data[a].y);
    }
    point(50,30);
    point(250,30);
    point(300,30);
    point(500,30);
    point(550,30);
    point(750,30);
}
