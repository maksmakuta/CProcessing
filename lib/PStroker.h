#ifndef PSTROKER_H
#define PSTROKER_H

#include <PShape.h>

#define MITER   0
#define BEVEL   1
#define ROUND   2
#define SQUARE  1
#define PROJECT 2

PShape makeJoin(){

}

PShape makeCap(){

}

PShape strokify(PShape contour, float w, int cap, int join){
    contour.type(LINES);
    return contour;


}

#endif // PSTROKER_H
