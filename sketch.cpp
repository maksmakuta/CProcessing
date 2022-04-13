#include <processing.h>

void setup() {
    size(640, 480);
}

void draw() {
    background(0);
    float w = width / 2;
    float h = height / 2;
    ellipse(w,h,w * 0.75,h * 0.25);
}
