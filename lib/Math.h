#ifndef MATH_H
#define MATH_H

class Math{
public:
    Math(){}

    static float lerp(float start, float stop, float amt) {
        return start + (stop-start) * amt;
    }
};
#endif
