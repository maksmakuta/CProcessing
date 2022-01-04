#ifndef PIMAGE_H
#define PIMAGE_H

/*
 * ================== INFO ==================
 * Formats: RGB, ARGB, Alpha
 * Supported types: png, jpg, gif, tga
 * ================= PImage =================
 * @version 0.1
 */

#include <vector>

enum Format{
    RGB,
    ARGB,
    Alpha
};

class PImage{
public:
    bool hiDpi = false;
    int pixelWidth,pixelHeight;
    int width,height;
    Format format;
    int pixelDensity;
    std::vector<int> pixels;

    PImage(int w, int h)                : PImage(w,h,ARGB){/* ... */}
    PImage(int w, int h,Format F)       : PImage(w,h,F,1) {/* ... */}
    PImage(int w, int h,Format F,int f){
       this->init(w,h,F,f);
    }

    void init(int w,int h,Format F,int f){
        this->width = w;
        this->height = h;
        this->format = F;
        this->pixelDensity = f;
        this->pixelWidth = width * pixelDensity;
        this->pixelHeight = height * pixelDensity;
        this->pixels = std::vector<int>();
    }
};

#endif
