/**
 *  Matrix Simulation
 *  Author: Maks Makuta
 *  URL: https://youtube.com/shorts/cjLb8aGsUtM
 */

#include <processing.h>

#define SIZE 36

struct CHAR{
    PVector pos;
    char c;
    int l;
};

PFont f;
std::vector<CHAR> characters;
PVector gravity;
int s;

void setup(){
    size(500,500);
    gravity = createVector(0.f,SIZE/6);
    f = createFont("/usr/share/fonts/gnu-free/FreeMono.otf",SIZE);
}

void draw(){
    background(0);
    s = width / SIZE;
    if(random(1.0) < 0.8){
        CHAR c{};
        c.pos = createVector( random<int>(s) * SIZE,-s);
        c.c = (char)random(32,126);
        c.l = 255;
        characters.push_back(c);
    }

    for(int a = 0;a < characters.size();a++){
        CHAR c = characters[a];

        fill(0,c.l,0);

        textFont(f);
        text(std::string("") + c.c,c.pos.x,c.pos.y);
        c.pos.add(gravity);
        if(c.l > 0) c.l--;

        characters[a] = c;

        if(c.pos.y > height + SIZE){
            characters[a] = characters[characters.size() - 1];
            characters.resize(characters.size() - 2);
            characters.shrink_to_fit();
        }
    }
    printf("%li\n",characters.size());
}
