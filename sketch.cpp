//#define FPS
#include "processing.h"
#include <string>


float angleRotate = 0.0;
void setup() {  
  size(640, 360);
}

void draw() {
  textFont(PFont("/usr/share/fonts/TTF/Roboto-Regular.ttf"),24.f);
  background(0);
  
  fill(200);
  text("Hello, ф or \u0444!", 0,mouseY);

}
