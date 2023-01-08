/**
 *  Snake game 2.1
 *  Author: Maks Makuta
 *  URL: https://youtu.be/bcIsRJrt50c
 */
#define USE_KEYS
#include "processing.h"
#include "extensions.h"

#define WIN_SIZE  100
#define BOTS_SIZE 2
#define BODY_SIZE 10.00f
#define SPEED_ADD   .05f
#define SPEED_MIN   .20f
#define SPEED_MAX  5.00f

PVector r(){
  return createVector(random(width),random(height));
}

class Snake{
public:
  float speed;

  Snake(){
    c = color(random(256),random(256),random(256));
    body.push_back(r());
    speed = SPEED_MIN;
  }

  void update(){
      move(p);
  }

  void update(PVector& vec){
      move(vec);
  }

  void eat(std::vector<PVector>& apples){
    near(apples);
    for(int a = 0;a < apples.size();a++){
      if(apples[a].dist(body[0]) <= BODY_SIZE){
        apples[a] = r();
        body.push_back(body[body.size() - 1]);
        speed += SPEED_ADD;
        near(apples);
      }
    }
  }

  void draw(){
    if(body.size() > 1){
      stroke(c);
      noFill();
      strokeCap(ROUND);
      strokeWeight(BODY_SIZE*2);
      for(int a = 0;a < body.size() - 1;a++){
        line(body[a].x,body[a].y,body[a+1].x,body[a+1].y);
      }
    }else{
      noStroke();
      fill(c);
      circle(body[0].x,body[0].y,BODY_SIZE);
    }
  }

  void eat(Snake& snake){
    for(int a = 0;a < size();a++){
      if(body[a].dist(snake.head()) < BODY_SIZE){
        snake.grow(this->size() - a);
        body.erase(body.begin() + a,body.end());
        speed *= 0.75;
        if(this->size() == 0){
          body.push_back(r());
        }
      }
    }
  }

  void grow(int a){
    for(int i = 0;i < a;i++)
      body.push_back(body[body.size() - 1]);
    speed *= 1.25;
  }

  PVector head(){
    return body[0];
  }

  int size(){return body.size();}

private:
  void near(const std::vector<PVector>& apples){
    float d = width;
    for(PVector apple : apples){
      float dst = apple.dist(body[0]);
      if(dst < d){
        d = dst;
        p = apple;
      }
    }
  }

  void move(PVector& vec){
    speed = constrain(speed,SPEED_MIN,SPEED_MAX);
    for(int a = 0;a < body.size();a++){
      PVector m = (a == 0 ? vec : body[a - 1]);
      if (m.dist(body[a]) >= BODY_SIZE/2){
        PVector s = m.sub(body[a]);
        body[a].add(s.limit(speed));
      }
    }
  }

  color c;
  std::vector<PVector> body;
  PVector p;

};

Snake player;
std::vector<Snake> bots;
std::vector<PVector> apples;
bool state = false;
std::string win_name;
bool winner = false;

void init(){
  apples.clear();
  player = Snake();
  for (int a = 0; a < 30; a++)
    apples.push_back(createVector(random(BODY_SIZE,width-BODY_SIZE), random(BODY_SIZE,height-BODY_SIZE)));
  bots.clear();
  for (int a = 0; a < BOTS_SIZE; a++)
    bots.push_back(Snake());
}

void setup() {
  size(500, 500);
  title("title");
  init();
}

void draw() {
  background(10, 25);

  if(state){
    PVector m = createVector(mouseX,mouseY);

    for(PVector apple : apples){
      noStroke();
      fill(200,100,20);
      circle(apple.x,apple.y,BODY_SIZE);
    }

    for(int a = 0; a < BOTS_SIZE;a++){
      bots[a].update();
      bots[a].draw();
      bots[a].eat(apples);
      bots[a].eat(player);
      player.eat(bots[a]);

      if(bots[a].size() >= WIN_SIZE){
        win_name = stringf("BOT %i",a+1);
        state = false;
        winner = true;
      }
    }

     if(player.size() >= WIN_SIZE){
        win_name = "Player";
        state = false;
        winner = true;
      }

    player.update(m);
    player.draw();
    player.eat(apples);

    fill(255);
    noStroke();
    for(int a = 0; a < BOTS_SIZE;a++){
      textAlign(ALIGN_LEFT,ALIGN_NONE);
      text(stringf("BOT %i | SIZE : %i | SPEED : %.1f",a+1,bots[a].size(),bots[a].speed),0,height-30*(a+1));
    }
    text(stringf("PLAYER | SIZE : %i | SPEED : %.1f",player.size(),player.speed),0,height);

  }else{
    fill(255);
    noStroke();
    textAlign(ALIGN_CENTER,ALIGN_CENTER);
    if(winner){
      text(std::string("Winner is " + win_name),width/2.f,height/2.f);
      text("Press ENTER to restart",width/2.f,height/2.f+30);
    }else{
      text("Press SPACE key to resume/pause",width/2.f,height/2.f);
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
      winner = false;
      state = true;
    }
  }
}
