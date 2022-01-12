#include <processing.h>

class Circle{
public:
    PVector pos;
    float radius;
    color c;

    Circle(float x,float y,float r,color _c){
        this->pos = createVector(x,y);
        this->radius = r;
        this->c = _c;
    }

    bool intersept(Circle c){
        return pos.dist(c.pos) >= radius + c.radius && !in(c.pos);
    }

    bool in(PVector p){
        return pos.dist(p) < radius;
    }

    void draw(){
        fill(c.r,c.g,c.b);
        circle(pos.x,pos.y,radius);
    }
};

std::vector<Circle> arr;
float max = 100,min = 5;
PFont fnt;
float tx,ty;

void setup(){
    size(600,600);
    fnt = createFont("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf",32);
    textFont(fnt);
}

void draw(){
    background(0);

    for(auto c : arr){
        c.draw();
    }

    if(arr.size() < 200){
        float x = random(width);
        float y = random(height);
        float r = max;
        color col = color::rand();
        bool add = true;
        if(!arr.empty()){
            for(auto c : arr){
                while(true){
                    bool t = c.intersept(Circle(x,y,r,col));
                    if(!t){
                        if(r < min) {
                            add = false;
                            break;
                        }
                        r -= 0.01f;

                    }else break;
                    if(!add) break;
                }
            }
        }else{
            tx = x;
            ty = y;
        }
        if(add) arr.push_back(Circle(x,y,r,col));
    }else{
        background(250,100,100,255);
    }


        fill(0);
        textAlign(CENTER,MIDDLE);
        text(std::to_string(arr.size()),tx,ty);
}


