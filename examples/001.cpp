#include "lib/processing.h"

int u = 60;
bool showGrid = true;

void drawGrid(){
    strokeWeight(1);
    noFill();
    stroke(200);
    for(int col = 0;col < 9;col++)
        line(col * u,0,col * u, width);
    for(int row = 0;row < 9;row++)
        line(0,row * u,height, row * u);
}

void setup() {
    size(540, 540);
}

void draw() {
    background(255);
    if(showGrid) drawGrid();
    strokeCap(ROUND);
    strokeWeight(1.5 * u);

    stroke(5,100,255);
    bezier(4 * u,1 * u,7 * u,1 * u,7 * u,5 * u,4 * u,5 * u);

    stroke(30,50,170);
    line(1 * u,6 * u,4 * u,2 * u);

    stroke(130,175,255);
    line(1 * u,3 * u,2 * u,5 * u);
}
