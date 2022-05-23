#ifndef PIMAGE_H
#define PIMAGE_H

#include <GL/glew.h>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

class PImage{
private:
    unsigned int textureID;
    std::string fname;
    bool loaded = false;
    int w,h,c;
public:
    PImage(){}
    PImage(const std::string& name){
        this->fname = name;
    }

    void load(const std::string& name){
       glGenTextures(1, &textureID);
       glBindTexture(GL_TEXTURE_2D, textureID);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
       //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
       stbi_set_flip_vertically_on_load(1);
       unsigned char *data = stbi_load(name.c_str(), &w, &h, &c, 0);
       if (data){
           std::cout << "Path : " << name << "\n"
                     << "Size : " << w << "x" << h << '\n'
                     << "Channels : " << c << std::endl;
           GLenum const fmt[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
           glTexImage2D(GL_TEXTURE_2D, 0, fmt[c-1], w, h, 0,fmt[c-1], GL_UNSIGNED_BYTE, data);
           glGenerateMipmap(GL_TEXTURE_2D);
           this->loaded = true;
       }else{
           std::cout << "Failed to load texture" << std::endl;
       }
       glBindTexture(GL_TEXTURE_2D, 0);
       stbi_image_free(data);
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

PImage loadImage(const std::string& name){
    return PImage(name);
}

#endif
