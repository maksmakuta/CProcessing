#include <processing.h>
#define NUM_LINES 20

float t = 0;

void setup() {
  size(400, 400);
}

float x1(float t) {
  return sin(t/10) * 100 + sin(t/646) * 81;
}

float y1(float t) {
  return cos(t/ 538) * 142;
}
float x2(float t) {
  return sin(t/-128) * 98 + sin(t/20) * 99;
}

float y2(float t) {
  return cos(t/ 10) * 60;
}

void draw() {
  background(20);
  strokeWeight(4);
  translate(width/2, height/2);
  for ( int i= 0; i < NUM_LINES; i++) {
    stroke(255, 0, 0, 100);
    line(x1(t+i), y1(t+i), x2(t+i), y2(t+i));
    stroke(0, 200, 0, 30);
    line(x1(t+i+7), y1(t+i+7), x2(t+i+7), y2(t+i+7));
    stroke(0, 0, 200, 100);
    line(-x1(t+i), y1(t+i), -x2(t+i), y2(t+i));
    stroke(0, 200, 0, 30);
    line(-x1(t+i+7), y1(t+i+7), -x2(t+i+7), y2(t+i+7));
    }
  t += 0.7;
}

