#define USE_KEYS
#include "processing.h"
#include "extensions.h"
#include <vector>
#include "hex.h"

using namespace std;

#define HEXSIZE 20


Layout l(layout_pointy,Point(HEXSIZE,HEXSIZE),Point(0,0));
vector<Hex> hexes;
float map_radius = 3;

    void setup() {
        size(500,500);
        title("title");

for (int q = -map_radius; q <= map_radius; q++) {
    int r1 = std::max(-map_radius, -q - map_radius);
    int r2 = std::min(map_radius, -q + map_radius);
    for (int r = r1; r <= r2; r++) {
        hexes.push_back(Hex(q, r, -q-r));
    }
}
    }

    void hex(float x, float y){

        beginShape(TRIANGLE_FAN);
        vertex(x,y);
        for(int a = 0;a <= 360;a+=60){
            float r = radians((float)a);
            vertex(x + sin(r)*HEXSIZE,y + cos(r)*HEXSIZE);
        }
        endShape();

    }

    void draw() {
      background(10);

      pushMatrix();
      translate(width/2.f,height/2.f);

        fill(255);

        for(Hex h : hexes){
            Point p = hex_to_pixel(l,h);
            hex(p.x,p.y);
        }
popMatrix();

//
    }

    void keyPressed(){}
