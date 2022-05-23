#ifndef HEXPRINER_H
#define HEXPRINER_H

#include <vector>
#include <string>

std::vector<bool> load(int c){
  std::vector<bool> data[16] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}, // 9
    {1,1,1,0,1,1,1}, // A
    {0,0,1,1,1,1,1}, // B
    {1,0,0,1,1,1,0}, // C
    {0,1,1,1,1,0,1}, // D
    {1,0,0,1,1,1,1}, // E
    {1,0,0,0,1,1,1}, // F
  };
  return data[c];
}

void hex(float x,float y, float w,float h,int c){
  strokeCap(ROUND);
  strokeWeight(15);
  std::vector<bool> num = load(c);

  float W = w * 0.2;
  float H = h * 0.1;
  if(num[0]){
    stroke(200); // A
    line(x+W,y,x+w-W,y);
  }
  if(num[1]){
    stroke(200); // B
    line(x+w,y+H,x+w,y-H+h/2);
  }
  if(num[2]){
    stroke(200); // C
    line(x+w,y+H+h/2,x+w,y-H+h);
  }
  if(num[3]){
    stroke(200); // D
    line(x+W,y+h,x+w-W,y+h);
  }
  if(num[4]){
    stroke(200); // E
    line(x,y+H+h/2,x,y+h-H);
  }
  if(num[5]){
    stroke(200); // F
    line(x,y+H,x,y-H+h/2);
  }
  if(num[6]){
    stroke(200); // G
    line(x+W,y+h/2,x+w-W,y+h/2);
  }
}

void printHex(const std::string& t,float x, float y, float h){
    float w = h/2;
    if(t.size() == 1){
        hex(x-w/2,y-h/2,w,h,t[0] - '0');
    }else if(t.size() == 2){
        hex(x-w-w/4,y-h/2,w,h,t[0] - '0');
        hex(x+w/4  ,y-h/2,w,h,t[1] - '0');
    }else if(t.size() == 3){
        float q = w/3;
        hex(x-w/2-w-q,y-h/2,w,h,t[0] - '0');
        hex(x-w/2    ,y-h/2,w,h,t[1] - '0');
        hex(x-w/2+w+q,y-h/2,w,h,t[2] - '0');
    }
}

void printHex(int integer, float x, float y, float h){
    std::string s = std::to_string(integer);
    printHex(s,x,y,h);
}

#endif // HEXPRINER_H
