#ifndef MATH_H
#define MATH_H

#define PI          3.1415926
#define HALF_PI     PI/2.0
#define QUARTER_PI  PI/4.0
#define TWO_PI      PI*2.0
#define CAPPA       0.5522847493f

#define radians(x) x*(PI/180)
#define degrees(x) x*(180/PI)
#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b
#define lerp(start,stop,amt) (start + (stop-start) * amt)

#endif
