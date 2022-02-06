#include <processing.h>

float l = 0.f;
float a = 0.f;
bool x = true;
void setup() {
  size(500, 500);
}

void draw() {
    glLineWidth(10);
  background(127);
  noFill();
  stroke(190);

  translate(width/2,height/2);
  rotate(-a);
  arc(0,0,200,200,0,l);

  if(x){
      l += 0.1f;
  }else{
      l -= 0.1f;
  }

  if(l > TWO_PI | l <= 0.0f) x = !x;

  if(a > TWO_PI) a = 0.f;
  a += 0.2f;

}
