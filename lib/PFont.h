#ifndef PFONT_H
#define PFONT_H

#include "ngl/nanovg.h"


/**
 * @brief The PFont class
 *  simplified version of Processing PFont class
 */
class PFont{
private:
    int fnt;
    int size;
public:
    PFont(){
        this->fnt = 0;
        this->size = 0;
    }

    PFont(const std::string& name, int s){
        this->fnt  = 0; //TODO this class
        this->size = s;
    }

    int getSize(){
        return this->size;
    }


};



#endif
