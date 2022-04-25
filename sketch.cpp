#include <processing.h>

int q = 0,r = 10;

void setup() {
    size(640, 480);
}

void draw() {
    noFill();
    strokeWeigth(5);

    background(0);
    float w = width / 2;
    float h = height / 2;
    float s = 16;
    translate(w,h);
    for(int a = 0;a < s;a++){
        circle(0,q,r);
        rotate(TWO_PI / s);
    }
    if(key == 'q' || key == 'Q')
        q += 5;
    if(key == 'a' || key == 'A')
        q -= 5;
    if(key == 'w' || key == 'W')
        r += 5;
    if(key == 's' || key == 'S')
        r -= 5;
}

