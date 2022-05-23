/**
 *  Stupid Simple Snake Game
 *  Author: Maks Makuta
 *  URL: https://youtu.be/cNitJ-N4tPo
 */
#define USE_KEYS
#include <processing.h>
#include "hexPrinter.h"

#define SIZE 25

std::vector<PVector> body;
int p = 0,m = 0,w,h;
boolean play = true;
PVector vel,head,apple;

void setup(){
  size(400,400);
  frameRate(10);
  w = width / SIZE;
  h = height / SIZE;
  vel = createVector(0,1);
  head = createVector(w/2,h/2);
  apple = createVector(random(w),random(h));
}

void check(PVector &p){
  if(p.x < 0) p.x = w;
  if(p.x > w) p.x = 0;
  if(p.y < 0) p.y = h;
  if(p.y > h) p.y = 0;
}

void move(){
  body[p] = head;
  p++;
  if(p > body.size() - 1)
    p = 0;
  for(PVector p : body)
    check(p);
}

void draw(){
  background(20);
  if(play){
    noStroke();
    if(!body.empty()){
      for(PVector p : body){
        fill(255);
        rect(p.x * SIZE,p.y * SIZE,SIZE,SIZE);
      }
    }
    fill(255,250,0);
    rect(head.x * SIZE,head.y * SIZE,SIZE,SIZE);

    fill(255,100,50);
    rect(apple.x * SIZE,apple.y * SIZE,SIZE,SIZE);

    if(!body.empty())
      move();
    head.add(vel);
    check(head);

    for(PVector p : body){
      if(PVector::equal(p,head))
        play = false;
    }

    if(PVector::equal(apple,head)){
      body.push_back(createVector(apple.x,apple.y));
      apple = createVector(random(w),random(h));
    }
  }else{
    printHex(body.size(),width/2,height/2,128);
  }
}

void keyPressed(){
  if(keyCode == UP && m != 1){
    vel = createVector(0,-1);
    m = 0;
  }
  if(keyCode == DOWN && m != 0){
    vel = createVector(0,1);
    m = 1;
  }
  if(keyCode == RIGHT && m != 3){
    vel = createVector(1,0);
    m = 2;
  }
  if(keyCode == LEFT && m != 2){
    vel = createVector(-1,0);
    m = 3;
  }
  if(keyCode == ESC && play == false){
    body.clear();
    w = width / SIZE;
    h = height / SIZE;
    head = createVector(w/2,h/2);
    vel = createVector(0,1);
    apple = createVector(random(w),random(h));
    play = true;
  }
}
