#include "lib/processing.h"

int cx, cy;
float secondsRadius;
float minutesRadius;
float hoursRadius;
float clockDiameter;

int second,minute,hour;

void setup() {
    second = 0;
    minute = 0;
    hour = 0;

  size(640, 360);
  stroke(255);

  int radius = min(width, height) / 2;
  secondsRadius = radius * 0.72;
  minutesRadius = radius * 0.60;
  hoursRadius = radius * 0.50;
  clockDiameter = radius * 1.8;

  cx = width / 2;
  cy = height / 2;
}

void draw() {
  background(0);

  // Draw the clock background
  fill(80);
  noStroke();
  ellipse(cx, cy, clockDiameter, clockDiameter);

  // Angles for sin() and cos() start at 3 o'clock
  // subtract HALF_PI to make them start at the top
  float s = map<float>(second, 0, 60, 0, TWO_PI) - HALF_PI;
  float m = map<float>(minute + norm(second, 0, 60), 0, 60, 0, TWO_PI) - HALF_PI;
  float h = map<float>(hour + norm(minute, 0, 60), 0, 24, 0, TWO_PI * 2) - HALF_PI;

  // Draw the hands of the clock
  stroke(255);
  strokeWeight(1);
  line(cx, cy, cx + cos(s) * secondsRadius, cy + sin(s) * secondsRadius);
  strokeWeight(2);
  line(cx, cy, cx + cos(m) * minutesRadius, cy + sin(m) * minutesRadius);
  strokeWeight(4);
  line(cx, cy, cx + cos(h) * hoursRadius, cy + sin(h) * hoursRadius);

  // Draw the minute ticks
  strokeWeight(2);
  for (int a = 0; a < 360; a+=6) {
    float angle = radians(a);
    float x = cx + cos(angle) * secondsRadius;
    float y = cy + sin(angle) * secondsRadius;
    circle(x, y,5);
  }

  second+=5;
  if(second > 60){
      minute ++;
      second = 0;
  }
  if(minute > 60){
        hour ++;
        minute = 0;
  }
  if(hour > 24){
    hour = 0;
  }
}
