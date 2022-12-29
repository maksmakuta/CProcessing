/**
 *  Snake game 2.0
 *  Author: Maks Makuta
 *  URL: https://www.youtube.com/watch?v=YdKCV6U9O4U
 */
#define USE_KEYS
#include "processing.h"
#include "extensions.h"

std::vector<PVector> usr;
std::vector<PVector> bot;
std::vector<PVector> apples;
int n = -1;

float uSpeed = .2f;
float bSpeed = .1f;
bool state = false;
bool winner = false;

void near(std::vector<PVector>& arr) {
  float d = width;
  for (int a = 0; a < apples.size(); a++) {
    float t = apples[a].dist(arr[0]);
    if (t < d) {
      d = t;
      n = a;
    }
  }
}

void init(){
  uSpeed = .1f;
  bSpeed = .1f;
  bot.clear();
  usr.clear();
  apples.clear();
  for (int a = 0; a < 30; a++)
    apples.push_back(createVector(random(width), random(height)));
  for (int a = 0; a < 1; a++){
    usr.push_back(createVector(random(width), random(height)));
    bot.push_back(createVector(random(width), random(height)));
  }
  near(bot);
}

void setup() {
  size(500, 500);
  title("title");
  init();
}

void track(std::vector<PVector>& arr,int p,bool isBot) {
  PVector m = (p == 0 ? (isBot ? apples[n] : createVector(mouseX,mouseY)) : arr[p - 1]);
  if (m.dist(arr[p]) > 15.f) {
    PVector s = m.sub(arr[p]);
    arr[p].add(s.limit(isBot ? bSpeed : uSpeed));
  }
}

void check(std::vector<PVector>& pos,PVector& apple,bool isBot){
  if (pos[0].dist(apple) < 15.f) {
    pos.push_back(pos[pos.size() - 1]);
    apple = createVector(random(width), random(height));
    if(isBot)
      bSpeed += 0.1f;
    else
      uSpeed += 0.1f;
    near(pos);
  }
}

void eat(PVector p,std::vector<PVector>& arr,bool isBot){
  for(int a = 0;a < arr.size();a++){
    if(p.dist(arr[a]) < 10.f){
      int s = arr.size() - a;
      arr.erase(arr.begin() + a,arr.end());
      if(isBot){
        uSpeed /= 2.f;
        bSpeed *= 1.25f;
        for(int b = 0;b < s;b++)
          bot.push_back(bot[bot.size() - 1]);
        near(arr);
      }else{
        bSpeed /= 2.f;
        uSpeed *= 1.25f;
        for(int b = 0;b < s;b++)
          usr.push_back(usr[usr.size() - 1]);
      }
    }
  }
}

void draw() {
  background(10, 25);

  if(state){
    near(bot);
    for(int a = 0;a < usr.size();a++){
      track(usr,a,false);
      eat(usr[0],bot,false);
    }
    for (int a = 0; a < bot.size(); a++) {
      track(bot,a,true);
      eat(bot[0],usr,true);
    }
    for (PVector &p : bot) {
      fill(200,0,250);
      noStroke();
      circle(p.x, p.y, 10.f);
    }
    for (PVector &p : usr) {
      fill(200,150,0);
      noStroke();
      circle(p.x, p.y, 10.f);
    }

    for (PVector &p : apples) {
      fill(200, 100, 50);
      noStroke();
      circle(p.x, p.y, 10.f);
      check(bot,p,true);
      check(usr,p,false);
    }
    fill(200);
    noStroke();

    textAlign(ALIGN_TOP,ALIGN_LEFT);
    text(stringf("(USR) size : %i | speed : %.1f", usr.size(), uSpeed), 0, height);
    text(stringf("(BOT) size : %i | speed : %.1f", bot.size(), bSpeed), 0, height - 20);

    uSpeed = constrain(uSpeed,0.1f,5.f);
    bSpeed = constrain(bSpeed,0.1f,5.f);

    if(bot.empty()){
      bot.push_back(createVector(random(width), random(height)));
    }
    if(usr.empty()){
      usr.push_back(createVector(random(width), random(height)));
    }
    if(bot.size() >= 100 || usr.size() >= 100){
      state = false;
      winner = true;
    }
  }else{
    textAlign(ALIGN_CENTER,ALIGN_CENTER);
    fill(200);
    if(winner){
      std::string w;
      if(bot.size() >= 100){
        w = "Computer";
      }else{
        w = "Player";
      }
      text(stringf("Winner is %s",w.c_str()),width/2.f,height/2.f);
    }else{
      text("Click space to play or pause",width/2.f,height/2.f);
    }
  }
}

void keyPressed() {
  if(key == ' ')
    state = !state;
  if(key == 'R')
    init();
  if(winner){
    if(keyCode == GLFW_KEY_ENTER){
      init();
      state = false;
      winner = false;
    }
  }
}
