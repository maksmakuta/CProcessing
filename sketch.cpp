#define FPS
#include "processing.h"

std::string t = "Text";
bool a = true;
void setup() {
  size(800, 800);
  antialiasing(true);
}

void draw() {
  background(0);
  textFont(PFont("/usr/share/fonts/gnu-free/FreeSans.otf"));
  fill(255);
  text(t,mouseX,mouseY);
}
