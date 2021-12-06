#include "lib/processing.h"

// fully clone from https://processing.org/examples/bezier.html

void setup() {
    size(640, 360);
    background(0);
    noStroke();
}

void draw() {
    fill(102);
    rect(81, 81, 63, 63);

    fill(255);
    ellipse(252, 144, 72, 140);

    fill(255);
    line(140,50,240,50);

    fill(150);
    circle(400,150,50);
}
