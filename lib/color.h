#ifndef COLOR_H
#define COLOR_H

#include "ngl/nanovg.h"
#include <string>
#include <cstring>
#include "Math.h"

class color{
public:
    color() : color(0.f){/* ... */}
    color(float a) : color(a,a,a){/* ... */}
    color(float _r,float _g,float _b) : color(_r,_g,_b,1.f){/* ... */}
    color(float _r,float _g,float _b,float _a){
        this->r = _r;
        this->g = _g;
        this->b = _b;
        this->a = _a;
    }

    color(int a) : color(a,a,a){/* ... */}
    color(int _r,int _g,int _b) : color(_r,_g,_b,1){/* ... */}
    color(int _r,int _g,int _b,int _a){
        this->r = _r / 255;
        this->g = _g / 255;
        this->b = _b / 255;
        this->a = _a / 255;
    }

    color(const std::string &h){
        hex(h.c_str());
    }

    void hex(const char *hex){
      int data[4];

      for(int & i : data)
          i = 255;

      switch (std::strlen(hex) - 1) {
          case 3: // #abc | #rgb
              for(int i = 0;i < 3;i++){
                  data[i] = fHex(hex[i+1]) * 16 + 15;
              }
              break;
          case 4: // #abcd | #rgba
              for(int i = 0;i < 4;i++){
                  data[i] = fHex( hex[i+1]) * 16 + 15;
              }
              break;
          case 6: // #123456 | #rrggbb
              for(int i = 0;i < 3;i++){
                  data[i] = fHex(hex[2*i+1]) * 16 + fHex(hex[2*i+2]);
              }
              break;
          case 8: // #12345678 | #rrggbbaa
              for(int i = 0;i < 4;i++){
                  data[i] = fHex(hex[2*i+1]) * 16 + fHex(hex[2*i+2]);
              }
              break;
        }

        this->r = data[0] / 255;
        this->g = data[1] / 255;
        this->b = data[2] / 255;
        this->a = data[3] / 255;
    }


     color operator=(const std::string &shex){
         this->hex(shex.c_str());
         return *this;
     }

    static color rand(){
        return color(random(255),random(255),random(255));
    }

    NVGcolor nvg(){
        return nvgRGBAf(r,g,b,a);
    }

    float r,g,b,a;

private:
        static int fHex(char _c){
            switch(_c) {
                case '0': return 0;
                case '1': return 1;
                case '2': return 2;
                case '3': return 3;
                case '4': return 4;
                case '5': return 5;
                case '6': return 6;
                case '7': return 7;
                case '8': return 8;
                case '9': return 9;
                case 'a': case 'A': return 10;
                case 'b': case 'B': return 11;
                case 'c': case 'C': return 12;
                case 'd': case 'D': return 13;
                case 'e': case 'E': return 14;
                case 'f': case 'F': return 15;
                default: return  -1;
            }
        }

};

#endif
