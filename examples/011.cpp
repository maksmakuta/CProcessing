/**
 *  Game Of Life
 *  Author: Maks Makuta
 *  URL: https://www.youtube.com/watch?v=DHQ1FEeaT0A
 */

#include <processing.h>
#define W 100
#define H 100

bool mat[W][H];
float cW,cH;
void setup() {
    size(1000, 1000);
    frameRate(10);
    for(int a = 0;a < W;a++)
        for(int b = 0;b < H;b++)
            mat[a][b] = random(a*b) % 2;
    cW = width / W;
    cH = height / H;
}

bool newGen(int a,int b){
    int n = 0;
    for(int x = -1;x <= 1;x++)
        for(int y = -1;y <= 1;y++){
            int px = a + x;
            int py = b + y;
            if(px < 0) px = W;
            if(px > W) px = 0;
            if(py < 0) py = H;
            if(py > H) py = 0;
            if(px != a || py != b)
                if(mat[px][py]) n++;
        }

    if((mat[a][b] && (n == 2 || n == 3)) ||
            (!mat[a][b] && n == 3)) return true;
    else return false;
}

void draw() {
    background(0);
    bool tmp[W][H];
    for(int a = 0;a < W;a++)
        for(int b = 0;b < H;b++){
            fill(mat[a][b]?170:0);
            rect(a*cW,b*cH,cW,cH);
            tmp[a][b] = newGen(a,b);
        }
    std::copy(&tmp[0][0],&tmp[0][0]+W*H,&mat[0][0]);
}
