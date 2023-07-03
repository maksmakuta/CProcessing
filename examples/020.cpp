
/**
 *  Simple Flappy Bird
 *  Author: Maks Makuta
 *  URL: https://youtu.be/BQt53tSyOrk
 */

#include "processing.h"

#define G 3 // gravity
#define T 5 // pipes size
#define B 20 // bird size

float s = B * 3.f;  // height of the passage between pipes
float S = 5.f;      // speed of pipes
float dt = 0.f;     // time of 1 frame
float y = 100.f;    // bird position
float dy = 0.f;     // bird velocity
PVector w[T];       // pipes array
bool isOver = false;// variable for game state
int score = 0;      // user score
int last = -1;      // pipe checker (needs for score logic)

PVector gen(int p = 0){
    float q = width / T;
    return createVector(
        width + q*p,
        random(B*4,height - B*4)
    );
}

void setup() {
    size(800, 600);
    for(int a = 0; a < T;a++){
        w[a] = gen(a);
    }
    dt = framerate / 1.f;
}

boolean circleRect(float cx, float cy, float radius,
                   float rx, float ry, float rw, float rh) {

    // temporary variables to set edges for testing
    float testX = cx;
    float testY = cy;

    // which edge is closest?
    if (cx < rx)         testX = rx;      // test left edge
    else if (cx > rx+rw) testX = rx+rw;   // right edge
    if (cy < ry)         testY = ry;      // top edge
    else if (cy > ry+rh) testY = ry+rh;   // bottom edge

    // get distance from closest edges
    float distX = cx-testX;
    float distY = cy-testY;
    float distance = (distX*distX) + (distY*distY);

    // if the distance is less than the radius, collision!
    if (distance <= radius*radius) {
        return true;
    }
    return false;
}

void draw() {
    background(0); // set background color - black
    // draw bird
    fill(200,200,0);
    circle(100,y,B);

    // draw pipes
    for(int a = 0; a < T;a++){
        PVector t = w[a];
        fill(50,180,20); // green
        noStroke();
        // top pipe
        rect(t.x - 10, 0, 20, t.y - s);
        // bottom pipe
        rect(t.x - 10, t.y + s, 20, height - t.y + s);
    }

    // rectangle for score
    fill(50); // dark-gray
    stroke(200); // light-gray
    rect(width/2 - 20,0,40,30);
    // score text
    noStroke();
    fill(255); // white
    textAlignH(CENTER);
    text(stringf("%i",score),width/2,20);
    // speed text
    fill(200,0,0); // red
    textAlignH(LEFT);
    text(stringf("%.2f",S),0,20);

    if(!isOver) y += dy; // move bird
    for(int a = 0; a < T;a++){
        // move pipes
        if(!isOver) w[a].x -= S * S * dt;
        if(w[a].x < -10){
            // if pipe at left side recreate
            // new pipe that moves from right side
            w[a] = gen(0);
        }
        if(100 - S < w[a].x && w[a].x <= 100){
            // check pipe and bird position
            // and update score and speed
            if(!isOver){
                if(last != a){
                    last = a;
                    score++;
                    S+=0.25f;
                }
            }
        }
        // check colision with top pipe
        if(circleRect(100,y,B,w[a].x - 10, 0, 20, w[a].y - s)){
            printf("top\n");
            isOver = true;
        }
        // check colision with bottom pipe
        if(circleRect(100,y,B,w[a].x - 10, w[a].y + s, 20, height - w[a].y + s)){
            printf("bottom\n");
            isOver = true;
        }
    }
    printf("%.2f,%.2f\n",dy,y);
    //check bird position
    if(y + B >= height){
        // bottom
        y = height - B;
        dy = 0;
        isOver = true;
    }else if(y <= B){
        //top
        y = B;
        dy = (G * 0.5f);
    }else{
        dy += G * G * dt;
    }
    // limit the bird speed
    if(dy >= G){
        dy = G;
    }
}

void keyPressed(){
    if(keypressed){
        if(key == GLFW_KEY_SPACE){
            if(isOver){  // regen pipes
                for(int a = 0; a < T;a++){
                    w[a] = gen(a);
                }
                // reset all data
                y = height / 3.f;
                score = 0;
                S = 5.f;
                last = -1;
                isOver = false;
            }else{ // bird flap
                dy = -G;
            }
        }
    }
}


