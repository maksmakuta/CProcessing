/**
 *  DVD screensaver
 *  Author: Maks Makuta
 *  CProcessing version : 1.4.7
 *  URL: https://www.youtube.com/watch?v=lKL7twr-BcE
 */

#include <processing.h>

PImage *img = new PImage[5];
int x = 0;
PVector pos,acc;

void setup() {
  size(800, 800);
  for(int a = 0;a < 5;a++)
    img[a] = PImage("../assets/dvdlogo-0" + std::to_string(a+1) + ".png");
  pos = createVector(width/2,height/2);
  acc = PVector::random2D();
}

void draw() {
  background(80);
  for(int a = 0;a < 5;a++)
    img[a].load();  
  image(img[x],pos.x,pos.y,200,200);
  pos.add(acc);
  if(pos.x + 200 >= width || pos.x <= 0 || pos.y + 200 >= height || pos.y <= 0){
    acc = createVector(random<float>(-1,1),random<float>(-1,1));
    x++;
    if(x >= 5)
      x = 0;
    printf("i -> %i\n",x);
  }
}
