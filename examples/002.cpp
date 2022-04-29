/**
 *  Purple Rain
 *  Author: Maks Makuta
 *  URL: https://youtube.com/shorts/WTZ2FJUfHq0
 */

#include <processing.h>

class Rain{
  private:
    PVector pos,g; float l,s;
  public:
    Rain(){
      this->pos = createVector(random(width),-random(height));
      this->g = createVector(0.f,random(3,12));
      this->l = random(2.f,20.f);
      this->s = random(1.f,5.f);
    }

    void update(){
      this->pos.add(g);
      if(this->pos.y > height + 10)
          *this = Rain();
    }

    void draw(){
      fill(200,100,250);
      rect(pos.x, pos.y,s,l);
    }
};

std::vector<Rain> r;
void setup(){
    size(640,1000);
    for(int a = 0;a < 500;a++)
      r.push_back(Rain());
}

void draw(){
  background(250);
  for(int a = 0;a < r.size();a++){
    r[a].update();
    r[a].draw();
  }
}
