/**
 *  Atom Simulation
 *  Author: Maks Makuta
 *  URL:
 *      Part 1: https://youtube.com/shorts/6tgeQQATyB4
 *      Part 2: https://youtube.com/shorts/A8IQgEsS4Xo
 */

#include <processing.h>
#define SIZE 9

PVector ellipse(float w,float h,float angle){
    return PVector(w * sin(angle),h * cos(angle));
}

class Proton{
  public:

    Proton() : Proton(100,300,0.f){}

    Proton(float w,float h,float _a){
        this->s = createVector(w,h);
        this->speed = 0.1f;
        this->a = _a;
    }

    void update(){
        this->pos = ellipse(s.x,s.y,radians(a));
        this->a += speed;
        if(a >= 360)
            a = 0.0f;
    }

    void draw(){
        fill(255);
        noStroke();
        circle(pos.x,pos.y,15);

        noFill();
        stroke(255);
        ellipse(0,0,2*s.x,2*s.y);
    }

    PVector pos,s;
    float speed,a = 0.0f;
};


Proton *p;

float angle = 360.0 / SIZE;
void setup(){
    size(500,500);
    p = new Proton[SIZE];
    for(int a = 0;a < SIZE;a++){
        //float w = a % 2 != 0 ?  200.f : 300.f;
        p[a] = Proton(80,200,0);
    }
}

void draw(){
    translate(width/2,height/2);
    background(0);

    for(int a = 0;a < SIZE;a++){
        p[a].update();
        pushMatrix();
        rotate(radians(angle * a));
        p[a].draw();
        popMatrix();
    }

    fill(255);
    circle(0,0,50);
}
