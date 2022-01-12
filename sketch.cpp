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
        return pos.dist(c.pos) >= radius + c.radius && !(pos.dist(c.pos) < radius);
    }

    void draw(){
        fill(c.r,c.g,c.b);
        circle(pos.x,pos.y,radius);
    }
};

std::vector<Circle> arr;
float max = 50,min = 5;
void setup(){
    size(600,600);
}

void draw(){
    background(0);

    for(auto c : arr){
        c.draw();
    }

    if(arr.size() < 300){
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
        }
        if(add) arr.push_back(Circle(x,y,r,col));
    }else{
        background(255);
    }
    //printf("%li\n",arr.size());
}


