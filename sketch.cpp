#include "lib/processing.h"
void setup() {
  size(640, 360);
  noStroke();
}

void draw() {
  background(0);
  // Scale the mouseX value from 0 to 640 to a range between 0 and 175
  float c = map<float>(mouseX, 0, width, 0, 175);
  // Scale the mouseX value from 0 to 640 to a range between 40 and 300
  float d = map<float>(mouseX, 0, width, 40, 300);
  fill(255, c, 0);
  ellipse(width/2, height/2, d, d);   
}
