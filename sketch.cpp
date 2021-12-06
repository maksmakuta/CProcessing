#include "lib/processing.h"

float i = 0.0f;
bool state = true;
float s;

void setup() {
  size(640, 360);
  s = min(width,height) /2;
}

void draw() {
  background(0);

  if(state){
    i += 0.001f;
  }else{
    i -= 0.001f;
  }

  fill(200);
  circle(width /2,height / 2, s * i);

  if(i > 1.f || i < 0.f) state = !state;
}
