#include <processing.h>

void setup() { size(400, 400); }

void draw(){
  strokeWeight(10);
  background(20);
  float s = 50;
  auto c = createVector(width/2,height/2);
  auto m = createVector(mouseX,mouseY);

  noStroke();
  fill(m.dist(c) >= s ? 200 : 80);
  stroke(m.dist(c) <= s ? 200 : 80);
  rect(width/2-s/2,height/2-s/2,s,s,s/2);

}
