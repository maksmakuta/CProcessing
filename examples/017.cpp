#define USE_KEYS
#include "processing.h"

void setup() {
  size(640, 480);
}

bool rect(float x, float y, float w, float h, float px, float py){
  return (x < px && (x + w) > px) && ( y < py && (y + h) > py);
}

bool rect(float x, float y, float w, float h, float r,float px, float py){
  if(rect(x,y,w,h,px,py)){
    bool is = false;
    color col(200,50,50);
    PVector m(px,py);
    
    PVector a(x+r  ,y+r  );
    PVector b(x+r  ,y+h-r);
    PVector c(x+w-r,y+h-r);
    PVector d(x+w-r,y+r  );

    PVector q(x+(w/2),y+(h/2));

    if(a.dist(m) <= r){
      noStroke();
      fill(col);
      circle(a.x,a.y,r);
      is = true;
    }

    if(b.dist(m) <= r){
      noStroke();
      fill(col);
      circle(b.x,b.y,r);
      is = true;
    }

    if(c.dist(m) <= r){
      noStroke();
      fill(col);
      circle(c.x,c.y,r);
      is = true;
    }

    if(d.dist(m) <= r){
      noStroke();
      fill(col);
      circle(d.x,d.y,r);
      is = true;
    }

    if(rect(x+r,y,w-2*r,h,m.x,m.y)){
      noStroke();
      fill(col);
      rect(x+r,y,w-2*r,h);
      is = true;
    }

    if(rect(x,y+r,w,h-2*r,m.x,m.y)){
      noStroke();
      fill(col);
      rect(x,y+r,w,h-2*r);
      is = true;
    }
    
    return  is;

  }else
    return false;
}

float r = 10;

struct Rect{
  float x,y,w,h,r;
};

void draw() {
  background(0);

  Rect p{100,100,(float)width-200,(float)height - 200,r};
    
  strokeWeight(3);

  noStroke();
  fill(20);
  rect(p.x,p.y,p.w,p.h);

  if(rect(p.x,p.y,p.w,p.h,p.r,mouseX,mouseY)){
    noFill();
    stroke(255,100,100);
    rect(p.x,p.y,p.w,p.h,p.r);
  }
}

void keyPressed(){
  if(keyCode == GLFW_KEY_UP)
    r += 5.f;
  if(r > 0.f){
    if(keyCode == GLFW_KEY_DOWN)
      r -= 5.f;
  }
  std::cout << r << std::endl;
}
