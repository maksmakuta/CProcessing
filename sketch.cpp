#define USE_KEYS
#include "processing.h"

bool state = false;

void setup() { 
  size(640, 480);
}

bool in(float a,float b, float val){
  return val >= a && val <= b;
}

bool inRect(float x,float y,float w,float h,float a,float b){
  return in(x,x+w,a) && in(y,y+h,b);
}

void v_switch(float px,float py,std::string textOn = "",std::string textOff = ""){
  float x = px;
  float y = py;
  float w = 100.f;
  float h = w/2.f;

  if(!textOff.empty()){
    float w = textWidth(textOff) + defSize;
    x += w;
    text(textOff, px, y+h/2.f + defSize/2);
  }

  if(!textOn.empty()){
    float sw = textWidth(textOn);
    stroke(255);
    text(textOn, x+w+defSize,y+h/2.f + defSize/2);
  }

  if(inRect(x,y,w,h,mouseX,mouseY)){
    if(mousepressed){
      state = !state;
    }
  }
  noStroke();
  fill(100);
  rect(x,y,w,h,h/2);

  float r = h * 0.8f;
  if(state){
    fill(250);
    circle(x + w - w/4, y + h/2, r/2);
  }else{
    fill(250);
    circle(x + w/4, y + h/2, r/2);
  }

}

void draw() {
  background(0);

  v_switch(100,100,"on","off");

}

void keyPressed(){
}
