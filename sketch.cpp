#include <processing.h>

void setup() {
  size(500, 500);
}

void draw() {
    /*
  background(0);
  fill(200);
  stroke(200);
  glLineWidth(2);
  rect(1,1,width/2,height/2);
  line(0,0,width/2,height/2);
  circle(width/4,height/4,width/4);
  arc(width/2,height/4,width/4,width/4,0,PI);
  */

    for (int y = 0; y < 400; y++) {
      float x = randomGaussian() * 60;
      line(200, y, 200 + x, y);
    }
    noLoop();
}
