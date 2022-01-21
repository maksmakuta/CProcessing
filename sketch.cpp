#include <processing.h>

void setup() {
  size(500, 500);
}

void draw() {
  background(0);
  stroke(255,0,255);
  bezier(100,100,200,200,500,0,500,500);

  fill(255);
  circle(100,100,5);
  fill(255);
  circle(200,200,5);
  fill(255);
  circle(500,0,5);
  fill(255);
  circle(500,500,5);
}
