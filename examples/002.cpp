#include "lib/processing.h"

int gridSize = 40;

void setup() {
    size(640, 360);
}

void draw(){
 background(0);
    for (int x = gridSize; x <= width - gridSize; x += gridSize) {
        for (int y = gridSize; y <= height - gridSize; y += gridSize) {
            noStroke();
            fill(255);
            rect(x-1, y-1, 3, 3);
            stroke(155);
            line(x, y, width/2, height/2);
        }
    }
}
