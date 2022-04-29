/**
 *  Pong Game
 *  Author: Maks Makuta
 *  URL:
 *      Part 1: https://youtube.com/shorts/4Kbj8MB6ic0
 *      Part 2: https://youtube.com/shorts/SZuBMV7uCNI
 */

#include <processing.h>
#define Q 30

struct Ball{
    PVector pos,vel;
    float r;

    void update(){
        pos.add(vel);
    }

    void draw(){
        fill(255);
        circle(pos.x,pos.y,r);
    }
};

struct Pad{
  PVector pos;
  float w,h;

  void draw(){
      fill(255);
      rect(pos.x,pos.y,w,h);
  }
};


bool circlerect(Ball b,Pad p){
      float testX = b.pos.x;
      float testY = b.pos.y;

      if (b.pos.x < p.pos.x)          testX = p.pos.x;
      else if (b.pos.x > p.pos.x+p.w) testX = p.pos.x+p.w;
      if (b.pos.y < p.pos.y)          testY = p.pos.y;
      else if (b.pos.y > p.pos.y+p.h) testY = p.pos.y+p.h;

      float distX = b.pos.x-testX;
      float distY = b.pos.y-testY;
      float distance = sqrt( (distX*distX) + (distY*distY) );

      if (distance <= b.r) {
        return true;
      }
      return false;
}

PFont fnt;
Pad p1,p2,tp,bp;
Ball b;
float ph;
int p1s = 4,p2s = 7;
PVector bpp;

void setup(){
    size(500,500);
    ph = height / 3.f;
    p1 = Pad{createVector(Q      ,300),10,ph};
    p2 = Pad{createVector(width-Q,300),10,ph};

    tp = Pad{createVector(0,0),(float)width,10};
    bp = Pad{createVector(0,height),(float)width,10};

    PVector bv = PVector::random2D();
    b = Ball{createVector(width/2,height/2),bv,20};

    fnt = createFont("/usr/share/fonts/gnu-free/FreeMono.otf",32);

    p1s = 1,p2s = 1;
}

void draw(){
    background(0);
    {
        stroke(255);
        noFill();
        circle(width/2,height/2,100);
        line(width/2,0,width/2,height);
    }
    p1.draw();
    p2.draw();
    b .draw();

    bpp = b.pos;

    p1.pos = createVector(Q,mouseY-ph/2.f);
    p2.pos = createVector(width - Q, bpp.y - ph/2.f );

    if(circlerect(b,p1) || circlerect(b,p2)){
        PVector v = b.vel;
        PVector nv = PVector::random2D().mult(random(1.5,4.5));
        b.vel = createVector(-v.x,nv.y);
    }

    if(circlerect(b,tp) || circlerect(b,bp)){
        PVector v = b.vel;
        b.vel = createVector(v.x,-v.y);
    }

    if(b.pos.x < 0 ){
        p2s++;
        PVector bv = PVector::random2D().mult(3.f);
        b = Ball{createVector(width/2,height/2),bv,20};     
    }

    if(b.pos.x > width){
        p1s++;
        PVector bv = PVector::random2D().mult(3.f);
        b = Ball{createVector(width/2,height/2),bv,20};
    }

    b.update();

    textFont(fnt);
    textAlign(CENTER);
    text(std::to_string(p1s-1) + " " + std::to_string(p2s-1),width/2,40);

}
