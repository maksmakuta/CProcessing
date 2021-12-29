#ifndef MATH_H
#define MATH_H

#include <random>

#define PI          3.1415926
#define HALF_PI     PI/2.0
#define QUARTER_PI  PI/4.0
#define TWO_PI      PI*2.0
#define CAPPA       0.5522847493f

#define radians(x) x*(PI/180)
#define degrees(x) x*(180/PI)
#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b
#define _lerp(start,stop,amt) (start + (stop-start) * amt)

#define tmp template<class T>

tmp T rand(T min, T max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(min, max);
    return static_cast<T>(distr(gen));
}

tmp T map(T val,T rmin,T rmax,T min,T max){
        T OldRange = rmax - rmin;
        T NewRange = max - min;
        return (((val - rmin) * NewRange) / OldRange) + min;
}

tmp T constrain(T val,T min,T max){
       if(val < min) return min;
       else if(val > max) return max;
       else return val;
}

float norm(float val,float min,float max){
    return map<float>(val,min,max,0.f,1.f);
}

tmp T random(T x)       { return rand<T>(0,x); }
tmp T random(T x,T y)   { return rand<T>(x,y); }


#endif
