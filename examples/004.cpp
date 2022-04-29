/**
 *  Planet Simulation 2D
 *  Author: Maks Makuta
 *  URL: https://www.youtube.com/watch?v=5Fmtu_DVWss
 */

#include <processing.h>

class Planet{
private:
    std::vector<Planet*> planets;
    PVector p; color c;
    float r, s;
public:
    Planet() : Planet(20,0  ,color(100,200,0  ), 0.f ){}

    Planet(float _r,float _d, color _c,float _s){
        this->p = createVector(1.f,0.f).mult(_d);
        this->c = _c;
        this->r = _r;
        this->s = _s;
    }

    void add(Planet* q){
        planets.push_back(q);
    }

    void update(){
        if(s != 0) p.rotate(radians(s));
        for(Planet* pl : planets) pl->update();
    }

    void draw(){
        fill(c);
        circle(p.x,p.y,r);
        for(Planet* pl : planets) pl->draw();
    }
};

Planet* sun = null;

void setup(){
    size(600,600);
    sun = new Planet();
    sun->add(new Planet(5 ,30 ,color(50 ,70 ,190),6.f ));
    sun->add(new Planet(7 ,45 ,color(100,90 ,240),5.f ));
    sun->add(new Planet(8 ,60 ,color(200,160,100),4.5f));
    sun->add(new Planet(10,80 ,color(200,100,0  ),4.f ));
    sun->add(new Planet(15,115,color(250,0  ,100),3.f ));
    sun->add(new Planet(20,160,color(  0,200,200),2.5f));
    sun->add(new Planet(25,200,color(255,0  ,0  ),2.f ));
    sun->add(new Planet(35,265,color(200, 70,255),1.f ));
}

void draw(){
    translate(width/2,height/2);
    background(0);
    sun->update();
    sun->draw();
}

