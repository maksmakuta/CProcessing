#include <processing.h>
#define W 100
#define H 100

bool mat[W][H];
float cellW,cellH;

void setup() {
    title("Conway's Game of Life");
    size(500, 500);
    frameRate(2);
    for(int a = 0;a < W;a++){
        for(int b = 0;b < H;b++){
            mat[a][b] = random(a+b) % 2;
        }
    }
    cellW = width / W;
    cellH = height / H;
}

bool nFun(int px, int py){
    int n = 0;
    for(int a = -1;a <= 1;a++){
        for(int b = -1;b <= 1;b++){
            int x = px + a;
            int y = py + b;
            if(x < 0) x = W - 1;
            if(x > W-1) x = 0;
            if(y < 0) y = H - 1;
            if(y > H-1) y = 0;
            if(px != x || py != y)
                if(mat[x][y]) n++;
        }
    }

    bool c = mat[px][py];
    if( c && (n == 2 || n == 3)) return true;
    else if(!c && n == 3) return true;
    else return false;
}

void draw() {
    background(0);
    bool tmp[W][H];
    for(int a = 0;a < W;a++){
        for(int b = 0;b < H;b++){
            fill(mat[b][a] ? 125 : 0);
            rect(cellW * a,cellH * b,W,H);
            tmp[a][b] = nFun(a,b);
        }
    }
    std::copy(&tmp[0][0], &tmp[0][0]+W*H,&mat[0][0]);
}
