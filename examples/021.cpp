/**
 *  Simple Dino T-Rex game
 *  Author: Maks Makuta
 *  URL: https://youtu.be/3RKSv1vfYB0
 */

#include "processing.h"

#define S 3 // cactuses
#define B 100 // dino size
#define Fmax 9.f // max force (gravity)
#define fori(n) for(int i = 0; i < n;i++) // for loop

int score = 0, // user score
    last = -1; // needed for score checking
bool play = false, // needed for gameover screen
    jump = false; // check if dino is jumping to prevent flying over cactuses
float dt = 1.f; // level speed
float dino = 0, // dino position (Y)
    dinof = 5; // dino velocity (Y)
PVector cactuses[S]; // cactuses (saves only position and height of cactus)

void setup() {
    size(800, 400);
    textAlign(CENTER,CENTER);
    fori(S){
        cactuses[i] = PVector(width + (width/S)*i,random(30,70));
    }
    dino = height - B*1.5;
}

/**
 * @brief intersectRect - check if rectangles are coliding
 */
bool intersectRect(int r1x,int r1y,int r1w,int r1h,
                   int r2x,int r2y,int r2w,int r2h) {
    return !(r2x > r1x + r1w ||
             r2x + r2w < r1x ||
             r2y > r1y + r1h ||
             r2y + r2h < r1y);
}

/**
 * @brief intersector - simple interface for intersectRect
 */
bool intersector(float d, PVector p) {
    return intersectRect(B,d,B,B,
                         p.x,height - B/2 - p.y,B*0.75,p.y);
}

void draw() {
    background(0);
    if(play){
        fori(S){
            noStroke();
            fill(20,125,70);
            rect(0,height - B/2,width,B/2); // ground
            fill(10,152,62);
            rect(cactuses[i].x,height - B/2,B*0.75,-cactuses[i].y); // cactus
            cactuses[i].sub(PVector(dt,0)); // apply velocity to cactus
            if(cactuses[i].x < -B){ // if cactus hides behind left wall then we create new one
                cactuses[i] = PVector(width + random(B,3*B),random(B/2.f,B*1.5f));
            }
            printf("%.2f,%.2f,%.6f,%i\r",cactuses[i].x,dino,dt,score); //debug
            if(cactuses[i].x < 50 && last != i){ // if cactus behind dino increase score
                score++;
                last = i; // this needed to detect cactus once
            }
            if(intersector(dino,cactuses[i])){ // if cactus touches dino then game over
                play = false;
            }
        }

        noStroke();
        fill(150,152,62);
        square(B,dino,B);// draw dino
        dino += dinof; // apply velocity to dino
        dinof += 0.1; // apply force to velocity
        if(dinof > Fmax && keyCode != DOWN) // if press DOWN button then fall faster
            dinof = Fmax;
        if(dino > height-(B*1.5)){ // if dino on ground then we can jump and disable velocity
            jump = false;
            dino = height-(B*1.5);
            dinof = 0;
        }
        dt += Fmax / 10000; // increase speed of cactuses
        fill(200);
        text(stringf("Score : %i",score),width/2,30); // score text
    }else{
        fill(200);
        text(stringf("Last score : %i",score),width/2,height/2); // gameover
    }
}

void keyPressed(){
    if(keypressed){
        if(keyCode == GLFW_KEY_SPACE){
            if(play){
                if(!jump){
                    jump = true;
                    dinof = -(Fmax*0.85);
                }
            }else{
                // set values to default
                play = true;
                fori(S){
                    cactuses[i] = PVector(width + (width/S)*i,random(30,70));
                }
                score = 0;
                dt = 1.0;
            }
        }
        if(key == GLFW_KEY_R){
            // recreate cactuses
            fori(S){
                cactuses[i] = PVector(width + (width/S)*i,random(30,70));
            }
        }
        if(keyCode == GLFW_KEY_DOWN){
            dinof = Fmax; //fast fall
        }
    }
}
