#ifndef PROCESSING_H
#define PROCESSING_H

/**
 * processing v.2.0-beta3
 * @author Maks Makuta
 */

#include <iostream>
#include <string>
#include <random>
#include <initializer_list>

#define null nullptr

#define PI 3.14159265358979323846
#define HALP_PI PI/2.0
#define TWO_PI 2.0*PI
#define TAU TWO_PI
#define QUARTER_PI PI/4

// ================== variables =====================

extern int mouseX;
extern int mouseY;
extern int pmouseX;
extern int pmouseY;
extern bool isMousePressed;

// ==================== classes =====================

class PObject{
public:
    virtual std::string toString() = 0;
};

class PVector : public PObject{
public:
    float x,y,z;

    PVector();
    PVector(float _x,float _y);
    PVector(float _x,float _y,float _z);

    PVector set(float _x,float _y,float _z);
    PVector set(float _x,float _y);
    PVector set(PVector v);
    PVector set(float v[3]);
    PVector copy();
    float mag();
    float magSq();
    PVector add(PVector& v);
    PVector add(float x, float y);
    PVector add(float x, float y, float z);
    PVector sub(PVector& v);
    PVector sub(float x, float y);
    PVector sub(float x, float y, float z);
    PVector mult(float n);
    PVector div(float n);
    float dist(PVector& v);
    float dot(PVector& v);
    float dot(float x, float y, float z);
    PVector cross(PVector& v);
    PVector normalize();
    PVector normalize(PVector& target);
    PVector limit(float max);
    PVector setMag(float len);
    float heading();
    PVector rotate(float theta);
    PVector lerp(PVector& v,float amt);
    PVector lerp(float x,float y,float z, float amt);
    float* array();
    std::string toString() override;

    static PVector random2D();
    static PVector random2D(PVector& target);
    static PVector random3D();
    static PVector random3D(PVector& target);
    static PVector fromAngle(float angle);
    static PVector fromAngle(float angle, PVector& target);
    static PVector add(PVector& v1, PVector& v2);
    static PVector add(PVector& v1, PVector& v2, PVector& target);
    static PVector sub(PVector& v1, PVector& v2);
    static PVector sub(PVector& v1, PVector& v2, PVector& target);
    static PVector mult(PVector& v, float n);
    static PVector mult(PVector& v, float n, PVector& target);
    static PVector div(PVector& v, float n);
    static PVector div(PVector& v, float n, PVector& target);
    static float dist(PVector& v1,PVector& v2);
    static float dot(PVector& v1, PVector& v2);
    static PVector cross(PVector& v, PVector& target);
    static PVector cross(PVector& v1, PVector& v2, PVector& target);
    static PVector setMag(PVector& target,float len);
    static PVector lerp(PVector& v1, PVector& v2, float amt);
    static float angleBetween(PVector& v1,PVector& v2);

};

// =================== methods ======================

void randomSeed(long seed);
void noiseSeed(long seed);

template<typename T>
T random(T low, T high);
template<typename T>
T random(T high);

double randomGaussian();

void noiseDetail(int lod);
void noiseDetail(int lod, float falloff);

float noise(float x);
float noise(float x, float y);
float noise(float x, float y, float z);

int abs(int n);
int constrain(int amt,int low,int high);
int max(int a, int b);
int max(int a, int b,int c);
int max(std::initializer_list<int> list);
int min(int a, int b);
int min(int a, int b, int c);
int min(std::initializer_list<int> list);
int round(float n);
int floor(float n);

float abs(float n);
float ceil(float n);
float constrain(float amt,float low,float high);
float dist(float x1,float y1,float x2, float y2);
float dist(float x1,float y1,float z1, float x2,float y2,float z2);
float exp(float n);
float lerp(float start, float end, float t);
float log(float n);
float mag(float a, float b);
float mag(float a, float b, float c);
float map(float value,
          float start1,float stop1,
          float start2,float stop2);
float max(float a, float b);
float max(float a, float b,float c);
float max(std::initializer_list<float> list);
float min(float a, float b);
float min(float a, float b, float c);
float min(std::initializer_list<float> list);
float norm(float value,float start,float stop);
float pow(float n,float e);
float sq(float n);
float sqrt(float n);

float acos(float value);
float asin(float value);
float atan2(float y, float x);
float atan(float value);
float cos(float angle);
float degrees(float radians);
float radians(float degrees);
float sin(float angle);
float tan(float angle);

// ================== public functions ===================

void setup();
void draw();

void keyTyped();
void keyPressed();
void keyReleased();

void mousePressed();
void mouseReleased();
void mouseClicked();
void mouseMoved();
void mouseDragged();
void mouseWheel();

#endif
