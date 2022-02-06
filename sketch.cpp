#include <processing.h>

void setup() {
  size(100, 100);
}

void draw() {
  background(0);
  fill(200);
  point(1,1);
  line(10,0,0,10);
  rect(10,10,10,10);
  fill(100);
  triangle(0,20,0,30,10,20);
  bezier(100,0,50,50,0,75,100,100);

  circle(100,  0,5);
  circle( 50, 50,5);
  circle(  0, 75,5);
  circle(100,100,5);

  //circle(50,50,10);
  //ellipse(25,75,10,20);
}
