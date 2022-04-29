/**
 *  Particles Animation
 *  Author: Maks Makuta
 *  URL: https://www.youtube.com/watch?v=l3QEZCp_C0Q
 */

#include <processing.h>
#define SIZE 70
#define R 50 

struct Point{
  PVector pos,acc;
  static Point obj(){
    return {createVector(random(width),random(height)),PVector::random2D()};
  }
};

std::vector<Point> data;

void setup() {
  size(400, 400);
  for(int a = 0;a < SIZE;a++)
    data.push_back(Point::obj());
}

void draw() {
    background(0);

    for(int a = 0;a < SIZE;a++){
      for(int b = 0;b < SIZE;b++){
          if(a != b)
            if(data[a].pos.dist(data[b].pos) < R){
              circle(data[a].pos.x,data[a].pos.y,10);
              line(data[a].pos.x,data[a].pos.y,data[b].pos.x,data[b].pos.y);
            }
      }
      data[a].pos.add(data[a].acc);
      if(data[a].pos.x < 0 || data[a].pos.x > width)
        data[a].acc.x *= -1;
      if(data[a].pos.y < 0 || data[a].pos.y > height)
        data[a].acc.y *= -1;
    }
}
