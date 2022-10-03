#define USE_KEYS
//#include <>
#include "processing.h"
#include "extensions.h"

void setup() { 
  size(640, 480);
}

void v_switch(bool &state,float px,float py,std::string textOn = "",std::string textOff = ""){
  float x = px;
  float y = py;
  float w = 100.f;
  float h = w/2.f;
  float r = h * 0.8f;

  if(!textOff.empty()){
    float w = textWidth(textOff);
    x += w;
    fill(255);
    text(textOff, px, y+h/2.f);
  }

  if(!textOn.empty()){
    float sw = textWidth(textOn);
    fill(255);
    text(textOn, x+w,y+h/2.f);
  }

  if(inRect(x,y,w,h,r,mouseX,mouseY)){
    if(mousepressed){
      state = !state;
    }
  }
  stroke(255);
  fill(100);
  rect(x,y,w,h,h/2);

  if(state){
    fill(250);
    circle(x + w - w/4, y + h/2, r/2);
  }else{
    fill(250);
    circle(x + w/4, y + h/2, r/2);
  }

}

void button(float x,float y,float w,float h, std::string msg,auto fun){
  float s = textWidth(msg);

  fill(200,150,100);
  rect(x,y,w,h,h/4);

  fill(0);
  text(msg,x+w/2-s/2,y+h/2);

  if(inRect(x,y,w,h,mouseX,mouseY)){
    if(mousepressed){
      fun();
    }
  }
}

void slider(float &val,float x,float y,float w,float min,float max){

  if(val <= min)
    val = min;

  if(val >= max)
    val = max;

  float p = map<float>(val,min,max,0,w);

  fill(255);
  textSize(20);

  float t = val;
  std::string s = stringf("%.2f",t);
  float tw = textWidth(s);
  text(s,x+p-tw/2,y);
  stroke(100);
  line(x+p-tw/2,y,x+p+tw,y);

  strokeWeight(5);
  stroke(100,150,200);
  line(x,y,x+w,y);

  strokeWeight(1);
  noStroke();
  fill(255);
  circle(x+p,y,5);

  PVector m(mouseX,mouseY);
  PVector c(x+p,y);
  
  if(inRect(x,y-5,w,10,mouseX,mouseY)){
    float t = map<float>(mouseX,x,x+w,min,max);
    if(mousepressed){
      val = t;
    }

    fill(120);
    std::string s = stringf("%.2f",t);
    float tw = textWidth(s);
    text(s,mouseX-tw/2,y);
  }
}


float r = 255.f;
float g = 0.f;
float b = 0.f;

void draw() {
  background(0);
  
  textSize(48);
  fill(r,g,b);
  text("Some text",300,200);
  
  textSize(20);
  slider(r,100,100,100,0,255);
  slider(g,100,150,100,0,255);
  slider(b,100,200,100,0,255);

}

void keyPressed(){
}
