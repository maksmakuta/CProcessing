#include "lib/processing.h"

int b = 0;

void setup() {
  size(450,900);
}

void draw(){
  background(0);
  fill(150,100,128);
  circle(width/2,0-b,width/4);
  if(_keyPressed){
      if(keyCode == SPACE){
          b -= 10;
      }
  }
}

