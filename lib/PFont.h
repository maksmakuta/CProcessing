#ifndef PFONT_H
#define PFONT_H

#include <string>
#include <GL/glew.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

class PFont{
private:
    std::string fontName;
    bool isInit = false;
public:
    stbtt_bakedchar cdata[96];
    GLuint ftex;

    PFont() : PFont("/usr/share/fonts/TTF/OpenSans-Regular.ttf"){/* ... */}

    PFont(std::string fname){
        this->fontName = fname;
    }

    bool initStatus(){
        return isInit;
    }

    void init(float s){
        //printf("isInit = %i",isInit ? 1 : 0);
        if(isInit != true){
            this->isInit = true;
            unsigned char *ttf_buffer = new unsigned char[1<<20];
            unsigned char *temp_bitmap = new unsigned char[512*512];
            fread(ttf_buffer, 1, 1<<20, fopen(fontName.c_str(), "rb"));
            stbtt_BakeFontBitmap(ttf_buffer,0, s, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
            delete []ttf_buffer;
            glGenTextures(1, &ftex);
            printf("texID -> %i\n",ftex);
            glBindTexture(GL_TEXTURE_2D, ftex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
            delete []temp_bitmap;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    }
};

#endif
