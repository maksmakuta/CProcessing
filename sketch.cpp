#include "lib/processing.h"
void setup() {
  size(640, 360);
  stroke(255);
  noFill();
}

void draw() {
  background(0);
  for (int i = 0; i < 200; i += 20) {
    bezier(mouseX-(i/2.0), 40+i, 410, 20, 440, 300, 240-(i/16.0), 300+(i/8.0));
  }
}
