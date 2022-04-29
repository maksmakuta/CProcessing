/**
 *  Bubble Sort Visualisation
 *  Author: Maks Makuta
 *  URL: https://www.youtube.com/watch?v=DIOe6c3kgow
 */

#include <processing.h>
#define SIZE 300

std::vector<int> data;
int i = 0,j = 0;
int comps = 0,swaps = 0;

void setup(){
    size(1600, 1000);
    for(int a = 1;a <= SIZE;a++){
        data.push_back(random(height));
    }
}

void draw(){
    background(10);

    if(i < SIZE - 1){
        if(j < SIZE - i - 1){
            if(data[j] > data[j + 1]){
                std::swap(data[j],data[j+1]);
                ++swaps;
            }
            ++comps;
            ++j;
        }else{
            ++i;
            j = 0;
        }
    }else{
        noLoop();
        printf("Comps : %i\nSwaps : %i\n",comps,swaps);
    }

    float w = width/(float)SIZE;
    for(int a = 0;a < SIZE;a++){
        fill(255);
        rect(w * a,height,w,-data[a]);
    }

}
