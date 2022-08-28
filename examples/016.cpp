/**
 *  Happy Birthday,  ktpp!!!
 *  Author: Maks Makuta
 *  CProcessing version : 1.4.10
 *  URL: https://youtu.be/EV6Gjhrxg-g
 */

#include "processing.h"

class Particle{
public:

    int life;
    PVector pos,vel,acc;
    PVector p;
    color c;
    bool q;
    int x = 0;

    Particle(){}

    Particle(float x, float y,color _c,bool star){
        this->pos = createVector(x,y);
        if(!star)
            this->vel = createVector(0,-8);
        else{
            this->vel = PVector::random2D();
            this->vel.mult(random(2,8));
        }
        this->acc = createVector(0,0);
        this->c = _c;
        this->q = star;
        this->life = 255;
    }

    void update(PVector f){
        p = pos;
        acc.add(f);
        vel.add(acc);
        pos.add(vel);
        acc.mult(0);
        life -= 5;
    }

    void draw(){
        //fill(this->c.r,this->c.g,this->c.b,this->life);
		fill(this->c);
        circle(this->pos.x,this->pos.y,this->q ? 4 : 8);
    }

    bool done(){
        if(this->life < 0)
            return true;
        return false;
    }

};

class Firework{
public:
    std::vector<Particle*> particles;
    Particle *p;
    color c;
    int size;
    bool explosed = false;

    explicit Firework(int s){
        this->size = s;
        this->p = new Particle(random(width),height,color(random(255),random(255),random(255)),false);
        this->c = color(random(255),random(255),random(255));
    }

    void update(PVector f){
        if(!this->explosed){
            this->p->update(f);
            if(this->p->vel.y >= 0){
                this->explosed = true;
                for(int a = 0;a < this->size;a++){
                    particles.push_back(new Particle(this->p->pos.x,this->p->pos.y,this->c,true));
                }
            }
        }
        for(int i = particles.size() - 1;i >= 0;i--){
            particles[i]->update(f);
        }
    }

    void draw(){
        if(!this->explosed)
            this->p->draw();
        for(int i = particles.size() - 1;i >= 0;i--){
            particles[i]->draw();
            if(particles[i]->done()){
                particles[i] = particles[particles.size() - 1];
                particles.erase(particles.end() - 1);
            }
        }
    }

    bool done(){
        if(this->explosed && this->particles.empty())
            return true;
        return false;
    }
};

std::vector<Firework*> p;
PVector gravity;
PFont f;
std::string message = "Happy Birthday, ktpp!!!";
int ts = 0,te = 0;
bool state = false;

void setup() {  
  	size(640, 480);
	antialiasing(true);
	gravity = createVector(0,0.06f);
	f = PFont("/usr/share/fonts/TTF/RobotoMono-Regular.ttf");
}

void draw() {
  	background(0);
	textFont(f,50.0f);
	fill(200);
	std::string t = message.substr(ts,te);
	float w = textWidth(t);
	text(t,width/2.f - w/4,height/2.f);
	
	if(frameCount % 20 == 0){
		if(state){
			ts++;
			if(ts >= message.size()){
				state = !state;
				ts = 0;
				te = 0;
			}
		}else{
			te++;
			if(te >= message.size()){
				state = !state;
				ts = 0;
			}
		}
	}

    int particles = 0;
    if(random(1.f) < 0.05f)
        p.push_back(new Firework(random(15,150)));

    for(int i = p.size() - 1;i >= 0;i--){
	    p[i]->update(gravity);
        p[i]->draw();
        if(p[i]->done()){
            p[i] = p[p.size() - 1];
            p.erase(p.end() - 1);
        }else{
            particles += p[i]->size;
        }
    }

    printf("%i : %i\n",(int)p.size(),particles);

}
