#include <processing.h>

void setup(){
    for(int i = 0; i < 128;i++){
        double a = noise(i);
        printf("a = %.2f\n",a);
    }
}

void draw(){

}
