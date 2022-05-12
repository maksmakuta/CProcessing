/**
 *  7 Segment Display
 *  Author: Maks Makuta
 *  URL: https://youtu.be/sdYpHVouiXA
 */

#define USE_KEYS
#include <processing.h>

PVector s;
int c = 0;

void setup(){
  size(400,400);
  s = createVector(100,200);
}

std::vector<bool> load(int c){
  std::vector<bool> data[16] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}, // 9
    {1,1,1,0,1,1,1}, // A
    {0,0,1,1,1,1,1}, // B
    {1,0,0,1,1,1,0}, // C
    {0,1,1,1,1,0,1}, // D
    {1,0,0,1,1,1,1}, // E
    {1,0,0,0,1,1,1}, // F
  };
  return data[c];
}

void hex(float x,float y, float w,float h,int c){

  std::vector<bool> num = load(c);

  float W = w * 0.2;
  float H = h * 0.1;

  stroke(num[0] ? 200 : 10); // A
  line(x+W,y,x+w-W,y);
  stroke(num[1] ? 200 : 10); // B
  line(x+w,y+H,x+w,y-H+h/2);
  stroke(num[2] ? 200 : 10); // C
  line(x+w,y+H+h/2,x+w,y-H+h);
  stroke(num[3] ? 200 : 10); // D
  line(x+W,y+h,x+w-W,y+h);
  stroke(num[4] ? 200 : 10); // E
  line(x,y+H+h/2,x,y+h-H);
  stroke(num[5] ? 200 : 10); // F
  line(x,y+H,x,y-H+h/2);
  stroke(num[6] ? 200 : 10); // G
  line(x+W,y+h/2,x+w-W,y+h/2);
}

void draw(){
  background(20);

  strokeCap(ROUND);
  strokeWeight(20);

  hex(width/2 - s.x/2,height/2 - s.y /2,s.x,s.y,c);
}

void keyPressed(){
  if(keyCode == UP)
    c++;
  if(keyCode == DOWN)
    c--;
  if(c > 15)
    c = 0;
  if(c < 0)
    c = 15;
}
