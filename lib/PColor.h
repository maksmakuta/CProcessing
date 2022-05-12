#ifndef PCOLOR_H
#define PCOLOR_H

#include "PMath.h"
#include <string>
#include <vector>

class color{
public:
    color() : color(0){/* ... */}
    color(int a) : color(a,a,a){/* ... */}
    color(int _r,int _g,int _b) : color(_r,_g,_b,1){/* ... */}
    color(int _r,int _g,int _b,int _a){
        this->r = (float)_r / 255.f;
        this->g = (float)_g / 255.f;
        this->b = (float)_b / 255.f;
        this->a = (float)_a / 255.f;
    }

    color(const std::string &h){
      int data[4];

      for(int & i : data)
          i = 255;

      switch (h.size() - 1) {
          case 3: // #abc | #rgb
              for(int i = 0;i < 3;i++){
                  data[i] = PMath::charHex(h[i+1]) * 16 + 15;
              }
              break;
          case 4: // #abcd | #rgba
              for(int i = 0;i < 4;i++){
                  data[i] = PMath::charHex( h[i+1]) * 16 + 15;
              }
              break;
          case 6: // #123456 | #rrggbb
              for(int i = 0;i < 3;i++){
                  data[i] = PMath::charHex(h[2*i+1]) * 16 + PMath::charHex(h[2*i+2]);
              }
              break;
          case 8: // #12345678 | #rrggbbaa
              for(int i = 0;i < 4;i++){
                  data[i] = PMath::charHex(h[2*i+1]) * 16 + PMath::charHex(h[2*i+2]);
              }
              break;
        }

        this->r = data[0] / 255;
        this->g = data[1] / 255;
        this->b = data[2] / 255;
        this->a = data[3] / 255;
    }


    color operator = (const std::string &shex){
        return color(shex.c_str());
    }

    std::vector<float> vector(){
        std::vector<float> v;
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        v.push_back(a);
        return v;
    }

    float r,g,b,a;
};

#endif
