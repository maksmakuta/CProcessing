/**
 * Clock
 * Author: Maks Makuta
 * URL: https://youtube.com/shorts/NMo2JOYTdFA
 */

#include <processing.h>

time_t t;
int h,m,s;

void setup(){
    size(600,600);
    t = time(NULL);
    s = t % 60;
    m = t % (60 * 60) / 60;
    h = ( t / (60 * 60) % 24 ) + 2;
}

void clock_line(int v,float r,float g,float b, float w,float t){
    strokeWeight(16);
    strokeCap(ROUND);
    fill(r,g,b);
    stroke(b,g,r);
    float a = map<float>(v,0,t,TWO_PI,0) - PI;
    line(0,0,w * sin(a),w * cos(a));
}

void draw(){
    if(t != time(NULL)){
        t = time(NULL);
        s = t % 60;
        m = (t % (60 * 60)) / 60;
        h = (t / (60 * 60)) % 24 + 2;
        printf("%i:%i:%i\n",h,m,s);
    }

    noFill();
    translate(width/2,height/2);
    background(0);

    fill(255,250,190);
    circle(0,0,290);
    fill(190,250,260);
    circle(0,0,260);

    int _h = h % 12;
    clock_line(s ,200,100,200,250,60);
    clock_line(m ,100,200,100,200,60);
    clock_line(_h,100,200,200,150,12);

    for(int a = 0;a < 360;a++){
        float angle = radians(a);
        if(a % 30 == 0){
            strokeWeight(16);
            stroke(200,200,0);
            point(275 * sin(angle),275 * cos(angle));
        }else if(a % 6 == 0){
            strokeWeight(8);
            stroke(0,200,200);
            point(250 * sin(angle),250 * cos(angle));
        }
    }

    strokeWeight(16);
    fill(200,200,100);
    point(0,0);

}

