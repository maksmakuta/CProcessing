#define USE_KEYS
#include "processing.h"

PFont f;
float a = 0;
void setup() { 
  size(640, 480); 
  f = PFont();
}

void draw() {
  textFont(f,30.f);
  background(0);
  fill(200);
  strokeWeight(5.f);
  stroke(250,120,50);
  rect(100, 100, 100,100, 25);

  pushMatrix();
    //resetMatrix();
    rotate(radians(a));
    translate(mouseX,mouseY);
    fill(255,100,150);
    std::string str = "Hello World";
    float w = textWidth(str);
    text(str,-(w/2),0);
    line(-(w/2),0,w/2,0);
  popMatrix();
}

void keyPressed(){
  if(keyCode == UP){
    a++;
  }

  if(keyCode == DOWN)
    a--;
}
