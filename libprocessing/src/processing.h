#ifndef PROCESSING_H
#define PROCESSING_H

/**
 * processing v.2.0-beta10
 * @author Maks Makuta
 */

#include <iostream>
#include <string>
#include <random>
#include <initializer_list>

#define null nullptr

#define PI 3.14159265358979323846
#define HALF_PI PI/2.0
#define TWO_PI 2.0*PI
#define TAU TWO_PI
#define QUARTER_PI PI/4
#define EPSILON  0.0001

#define ALPHA 1
#define  ARGB 1 << 1
#define   RGB 1 << 2
#define   HSB 1 << 3

#define PROJECT 0x0002
#define SQUARE  0x0020
#define MITER   0x0004
#define BEVEL   0x0040
#define ROUND   0x8000

#define CENTER  0x0001
#define RADIUS  0x0010
#define CORNER  0x0100
#define CORNERS 0x1000

#define POINTS          0x0001
#define LINES           0x0002
#define TRIANGLES       0x0003
#define TRIANGLE_FAN    0x0004
#define TRIANGLE_STRIP  0x0005
#define QUADS           0x0006
#define QUAD_STRIP      0x0007

#define CLOSE true

typedef std::array<float,6> mat2Data;
typedef std::array<float,16> mat4Data;

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

class color : public PObject{
public:
    float r,g,b,a;
    int i;

    color(int gray);
    color(int gray, int alpha);
    color(int rgb, float alpha);
    color(int x, int y, int z);
    color(int x, int y, int z,int w);
    color& operator = (int argb);
    std::string toString() override;
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
    PVector add(PVector v);
    PVector add(float x, float y);
    PVector add(float x, float y, float z);
    PVector sub(PVector v);
    PVector sub(float x, float y);
    PVector sub(float x, float y, float z);
    PVector mult(float n);
    PVector div(float n);
    float dist(PVector v);
    float dot(PVector v);
    float dot(float x, float y, float z);
    PVector cross(PVector v);
    PVector normalize();
    PVector normalize(PVector& target);
    PVector limit(float max);
    PVector setMag(float len);
    float heading();
    PVector rotate(float theta);
    PVector lerp(PVector v,float amt);
    PVector lerp(float x,float y,float z, float amt);
    float* array();
    std::string toString() override;
    bool equal(PVector v);

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

class PMatrix2D : public PObject{
public:
    PMatrix2D();
    PMatrix2D(mat2Data* mat);
    PMatrix2D(PMatrix2D*);
    PMatrix2D(float a00,float a01,float a02,
              float a10,float a11,float a12);

    void reset();
    PMatrix2D* get();
    mat2Data* get(mat2Data* target);
    void set(PMatrix2D* src);
    void set(mat2Data* source);
    void set(float m00, float m01, float m02,
             float m10, float m11, float m12);

    void translate(float tx, float ty);
    void rotate(float angle);
    void rotateZ(float angle);
    void scale(float s);
    void scale(float sx, float sy);
    void shearX(float angle);
    void shearY(float angle);

    void apply(PMatrix2D* source);
    void apply(float n00, float n01, float n02,
               float n10, float n11, float n12);

    void preApply(PMatrix2D* source);
    void preApply(float n00, float n01, float n02,
                  float n10, float n11, float n12);

    PVector* mult(PVector* source, PVector* target);
    float* mult(float* source, float* target);

    bool invert();
    float determinant();

    std::string toString()  override;

    float m00, m01, m02,
          m10, m11, m12;
};

class PMatrix3D : public PObject{
public:
    PMatrix3D();
    PMatrix3D(mat4Data* mat);
    PMatrix3D(PMatrix3D*);
    PMatrix3D(
        float m00, float m01, float m02,
        float m10, float m11, float m12
    );
    PMatrix3D(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33
    );

    void reset();
    PMatrix3D* get();
    mat4Data* get(mat4Data* target);
    void set(PMatrix3D* src);
    void set(mat4Data* source);
    void set(float m00, float m01, float m02, float m03,
             float m10, float m11, float m12, float m13,
             float m20, float m21, float m22, float m23,
             float m30, float m31, float m32, float m33);

    void translate(float tx, float ty);
    void translate(float tx, float ty, float tz);
    void rotate(float angle);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void rotate(float angle, float v0, float v1, float v2);
    void scale(float s);
    void scale(float sx, float sy);
    void scale(float x, float y, float z);
    void shearX(float angle);
    void shearY(float angle);

    void apply(PMatrix3D* source);
    void apply(float n00, float n01, float n02,
               float n10, float n11, float n12);
    void apply(float n00, float n01, float n02, float n03,
               float n10, float n11, float n12, float n13,
               float n20, float n21, float n22, float n23,
               float n30, float n31, float n32, float n33);

    void preApply(PMatrix3D* source);
    void preApply(float n00, float n01, float n02,
                  float n10, float n11, float n12);
    void preApply(float n00, float n01, float n02, float n03,
                  float n10, float n11, float n12, float n13,
                  float n20, float n21, float n22, float n23,
                  float n30, float n31, float n32, float n33);

    PVector* mult(PVector* source, PVector* target);
    float* mult(float* source, float* target, int s = 3);

    void transpose();
    bool invert();
    float determinant();

    std::string toString()  override;

    float m00, m01, m02, m03,
          m10, m11, m12, m13,
          m20, m21, m22, m23,
          m30, m31, m32, m33;
};

class PImage : public PObject{
private:
    int length = 0;
public:
    int* pixels = null;
    int  width, height, format;

    PImage();
    PImage(int width, int height);
    PImage(int width, int height, int format);
    PImage(int width, int height, int format, int* pixels, int pwidth, int pheight);

    void loadPixels();
    void updatePixels();
    void updatePixels(int x,int y,int w,int h);
    void resize(int w,int h);
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

class PFont : public PObject{
public:
    PFont();
private:

};

class PShape : public PObject{
private:
    bool visibility = true,
        newShape = false;
    std::vector<PShape> childs;
    std::vector<PVector> vertexes;
    std::string name;
    PMatrix3D mat;
public:
    PShape();

    bool isVisible();
    void setVisible(bool isVisible);
    //void disableStyle();
    //void enableStyle();
    //void beginContour();
    //void endContour();
    void beginShape();
    void endShape(bool close = false);

    int getChildCount();
    PShape getChild(int index);
    PShape getChild(const std::string& name);
    void addChild(PShape& child);

    int getVertexCount();
    PVector getVertex(int index);
    PVector getVertex(int index, PVector& vec);
    void setVertex(int index, float x,float y);
    void setVertex(int index, float x,float y,float z);
    void setVertex(int index, PVector vec);
    void vertex(PVector vec);
    void vertex(float x, float y);
    void vertex(float x, float y,float z);

    void setFill(color c);
    void setStroke(color c);
    void fill(color c);
    void stroke(color c);
    void noFill();
    void noStroke();

    void translate(float x,float y);
    void translate(float x,float y, float z);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void rotate(float angle);
    void scale(float s);
    void scale(float x,float y);
    void scale(float x,float y,float z);
    void resetMatrix();

    std::string toString() override;


    bool isStroke = false,
         isFill = true;
    color strokeColor = 0,
          fillColor = 0;
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

int constrain(int amt,int low,int high);
int max(int a, int b);
int max(int a, int b,int c);
int max(std::initializer_list<int> list);
int min(int a, int b);
int min(int a, int b, int c);
int min(std::initializer_list<int> list);

float constrain(float amt,float low,float high);
float dist(float x1,float y1,float x2, float y2);
float dist(float x1,float y1,float z1, float x2,float y2,float z2);
float lerp(float start, float end, float t);
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
float sq(float n);

float degrees(float radians);
float radians(float degrees);

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

PShader loadShader(const std::string& fragFilename);
PShader loadShader(const std::string& fragFilename,const std::string&  vertFilename);
void resetShader();
void shader(PShader &shader);

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

void ellipseMode(int mode);
void rectMode(int mode);
void strokeCap(int);
void strokeJoin(int);
void strokeWeight(float);

void noStroke();
void stroke(color c);
void stroke(float gray);
void stroke(float gray, float alpha);
void stroke(float x, float y, float z);
void stroke(float x, float y, float z, float a);

void noFill();
void fill(color c);
void fill(float gray);
void fill(float gray, float alpha);
void fill(float x, float y, float z);
void fill(float x, float y, float z, float a);

void background(color c);
void background(float gray);
void background(float gray, float alpha);
void background(float x, float y, float z);
void background(float x, float y, float z, float a);
void background(PImage image);

void colorMode(int mode);
void colorMode(int mode, float max);
void colorMode(int mode, float maxX, float maxY, float maxZ);
void colorMode(int mode, float maxX, float maxY, float maxZ, float maxA);

float alpha(int what);
float red(int what);
float green(int what);
float blue(int what);
float hue(int what);
float saturation(int what);
float brightness(int what);

int lerpColor(int c1, int c2, float amt);
int lerpColor(int c1, int c2, float amt, int mode);
color lerpColor(color c1, color c2, float amt);
color lerpColor(color c1, color c2, float amt, int mode);

void beginShape(int mode = 0);
void vertex(float x, float y);
void vertex(float x, float y,float z);
void endShape();

void arc(float x, float y, float w, float h, float start, float stop);
void arc(float x, float y, float w, float h, float start, float stop, int mode);
void circle(float x, float y, float extent);
void ellipse(float x, float y, float w, float h);
void line(float x1, float y1, float x2, float y2);
void line(float x1, float y1, float z1, float x2, float y2, float z2);
void point(float x, float y);
void point(float x, float y, float z);
void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
void rect(float x, float y, float w, float h);
void rect(float x, float y, float w, float h, float r);
void rect(float x, float y, float w, float h, float tl, float tr, float br, float bl);
void square(float x,float y,float s);
void triangle(float x1,float y1,float x2,float y2,float x3,float y3);

void frustum(float left,float right,float bottom,float top,float near,float far);
void ortho();
void ortho(float left,float right,float bottom,float top);
void ortho(float left,float right,float bottom,float top,float near,float far);
void perspective();
void perspective(float fov, float aspect,float zNear,float zFar);

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
