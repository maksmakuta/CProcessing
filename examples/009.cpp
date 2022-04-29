/**
 *  Ukrainian Flag
 *  Author: Maks Makuta
 *  URL: https://www.youtube.com/watch?v=8xdbqK7CO4E
 */

#include <processing.h>

void setup() {
  size(600, 400);
}

void draw() {
  background(0);
  float q = height/2;
  fill(0,0,255);
  rect(0,0,width,q);
  fill(255,255,0);
  rect(0,q,width,q);
}


