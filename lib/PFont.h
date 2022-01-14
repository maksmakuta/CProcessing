#ifndef PFONT_H
#define PFONT_H

/**
 * @brief The PFont class
 */
class PFont{
private:
    int fnt;
    float size;
public:
    PFont(){
        this->fnt = -1;
        this->size = 0.f;
    }

    PFont(int f, float s){
        this->fnt  = f; //TODO this class
        this->size = s;
    }

    float getSize(){
        return this->size;
    }

    int getFont(){
        return this->fnt;
    }

    void setSize(float s){
        this->size = s;
    }

};



#endif
