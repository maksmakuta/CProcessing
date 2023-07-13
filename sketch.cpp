#include <processing.h>

bool z = true;
int a = 0;

void setup(){
    size(640,480);
}

void draw(){
    background(a);
    if(z){
        a++;
    }else{
        a--;
    }
    if(a == 255 || a == 0){
        z = !z;
    }
}
