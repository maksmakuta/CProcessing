#ifndef PIMAGE_H
#define PIMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

class PImage{
private:
    unsigned int textureID = 0;
    std::string fname;
    bool loaded = false;
    int w,h,c;
public:
    PImage(){}
    PImage(const std::string& name){
        this->fname = name;
    }

    void load(){
        if(loaded != true){
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_set_flip_vertically_on_load(true);
            auto data = stbi_load(fname.c_str(), &w, &h, &c, 0);
            if (data){
                printf("ID -> %i\nc -> %i\n",textureID,c);
                float img[] = { GL_RED,GL_RG,GL_RGB,GL_RGBA };
                glTexImage2D(GL_TEXTURE_2D, 0, img[c-1], w, h, 0, img[c-1], GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                this->loaded = true;
            }else{
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
        }
    }

    bool isLoaded() const{
        return this->loaded;
    }

    std::string getName() const{
        return this->fname;
    }

    unsigned int getID() const{
        return this->textureID;
    }

    int getWidth() const{
        return this->w;
    }

    int getHeight() const{
        return this->h;
    }

    void done(){
        glDeleteTextures(1,&textureID);
    }

    ~PImage(){
        done();
    }
};

#endif
