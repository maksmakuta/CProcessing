#include <processing.h>

void setup() {
  size(500, 500);
}

void draw() {
  background(127);
  noFill();
  pushMatrix();
    stroke(255,200,0);
    strokeWeigth(50);
    translate(width/2,height/2);
    rotate(-HALF_PI);
    arc(0,0,200,200,QUARTER_PI,TWO_PI - QUARTER_PI);
  popMatrix();

  strokeWeigth(10);
  stroke(255);
  bezier(0,0,0,500,500,0,500,500);
}
