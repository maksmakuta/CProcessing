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

#define RGB 1 << 4
#define HSB 1 << 8

// ================== variables =====================

extern bool isMousePressed;
extern bool focused;

extern int mouseX;
extern int mouseY;
extern int pmouseX;
extern int pmouseY;
extern int displayHeight;
extern int displayWidth;
extern int frameCount;
extern int framerate;
extern int height;
extern int width;
extern int pixelHeight;
extern int pixelWidth;


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

class PMatrix : public PObject{
public:
    PMatrix();

    std::string toString() override;
};

class PMatrix2D : public PMatrix{
public:
    PMatrix2D();
};

class PMatrix3D : public PMatrix{
public:
    PMatrix3D();
};

class PImage : public PObject{
public:
    int* pixels = null;
    int  width, height;

    void loadPixels();
    void updatePixels();
    void resize();
    void get();
    void set();
    void mask();
    void filter();
    void copy();
    void blendColor();
    void blend();
    void save();

    std::string toString() override;
};

class PShader : public PObject{
private:
    void create(const char* vert,const char* frag);
public:
    PShader();                       // load default shader
    PShader(const std::string& vert,const std::string& frag);   // code
    PShader(const char* vert,const char* frag);                 // file

    void set(const std::string &name, int x);
    void set(const std::string &name, float x);
    void set(const std::string &name, bool x);
    void set(const std::string &name, int x, int y);
    void set(const std::string &name, float x, float y);
    void set(const std::string &name, bool x, bool y);
    void set(const std::string &name, int x, int y, int z);
    void set(const std::string &name, float x, float y, float z);
    void set(const std::string &name, bool x, bool y, bool z);
    void set(const std::string &name, int x, int y, int z, int w);
    void set(const std::string &name, float x, float y, float z, float w);
    void set(const std::string &name, bool x, bool y, bool z, bool w);
    void set(const std::string &name, int* vec);
    void set(const std::string &name, float* vec);
    void set(const std::string &name, bool* vec);
    void set(const std::string &name, int* vec,int ncoords);
    void set(const std::string &name, float* vec,int ncoords);
    void set(const std::string &name, bool* vec,int ncoords);
    void set(const std::string &name, PMatrix2D &mat);
    void set(const std::string &name, PMatrix3D &mat);
    void set(const std::string &name, PMatrix2D &mat, bool use3x3);
    void set(const std::string &name, PMatrix3D &mat, bool use3x3);
    void set(const std::string &name, PImage &tex);

    std::string toString() override;
    int program;
};

class color : public PObject{
public:
    float r,g,b,a;


    std::string toString() override;

};

class PFont : public PObject{
public:
    PFont();
private:

};

class PShape : public PObject{
public:
    PShape();

    void isVisible();
    void setVisible();
    void disableStyle();
    void enableStyle();
    void beginContour();
    void endContour();
    void beginShape();
    void endShape();
    void getChildCount();
    void getChild();
    void addChild();
    void getVertexCount();
    void getVertex();
    void setVertex();
    void setFill();
    void setStroke();
    void translate();
    void rotateX();
    void rotateY();
    void rotateZ();
    void rotate();
    void scale();
    void resetMatrix();
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

// text functions

void createFont();
void loadFont();
void textFont();
void text();
void textAlign();
void textLeading();
void textMode();
void textSize();
void textWidth();
void textAscent();
void textDescent();

// shader functions

void loadShader();
void resetShader();
void shader();

// matrix functions

void pushMatrix();
void popMatrix();

void translate(float tx, float ty);
void translate(float tx, float ty, float tz);
void rotate(float angle);
void rotateX(float angle);
void rotateY(float angle);
void rotateZ(float angle);
void rotate(float angle, float vx, float vy, float vz);
void scale(float s);
void scale(float sx, float sy);
void scale(float x, float y, float z);

void resetMatrix();
void applyMatrix(float n00, float n01, float n02,
                 float n10, float n11, float n12);
void applyMatrix(float n00, float n01, float n02, float n03,
                 float n10, float n11, float n12, float n13,
                 float n20, float n21, float n22, float n23,
                 float n30, float n31, float n32, float n33);

void printMatrix();

// drawing functions

void noStroke();
void stroke(float gray);
void stroke(float gray, float alpha);
void stroke(float x, float y, float z);
void stroke(float x, float y, float z, float a);

void noFill();
void fill(float gray);
void fill(float gray, float alpha);
void fill(float x, float y, float z);
void fill(float x, float y, float z, float a);

void background(float gray);
void background(float gray, float alpha);
void background(float x, float y, float z);
void background(float x, float y, float z, float a);
void background(PImage image);

void colorMode(int mode);
void colorMode(int mode, float max);
void colorMode(int mode, float maxX, float maxY, float maxZ);
void colorMode(int mode, float maxX, float maxY, float maxZ, float maxA);


int color(int gray);
int color(int gray, int alpha);
int color(int rgb, float alpha);
int color(int x, int y, int z);

float alpha(int what);
float red(int what);
float green(int what);
float blue(int what);
float hue(int what);
float saturation(int what);
float brightness(int what);

int lerpColor(int c1, int c2, float amt);
int lerpColor(int c1, int c2, float amt, int mode);

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

void cursor(int kind);
void cursor(PImage img);
void cursor(PImage img, int x,int y);
void cursor();
void delay(int napTime);
int  displayDensity();
int  displayDensity(int display);
void frameRate(float fps);
void fullScreen();
void fullScreen(int display);
void fullScreen(char* renderer);
void fullScreen(char* renderer, int display);
void noCursor();
void noSmooth();
void pixelDensity(int density);
void settings();
void size(int w, int h);
void size(int w, int h, int renderer);
void smooth(int level);

void windowMove();
void windowMoved();
void windowRatio();
void windowResizable();
void windowResize();
void windowResized();
void windowTitle();

#endif
