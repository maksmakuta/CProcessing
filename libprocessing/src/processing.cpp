#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <stb/stb_image_write.h>
#include <cmath>
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <stack>
#include <fstream>
#include <limits>
#include <assert.h>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "backend/gl/gl_backend.h"
#include "backend/shaders.h"
#include "processing.h"

void todo(const std::string& text){
    std::cout << "TODO: " << text << std::endl;
}

void error(const std::string& text){
    std::cerr << text << std::endl;
}

// ================== functions =====================
std::string readFile(const std::string& filename);
// ================== variables =====================
GLFWwindow* window = null;
backend* gl = null;
PShader* sh = null;

PMatrix3D mat;
std::stack<PMatrix3D> matStack;

float* tmp = null;
std::vector<vert> _data; // vertex data for drawing
long _seed = 0L;    // random seed
long _pseed = 0L;   // perlin noise seed
int _poctaves = 4;  // perlin octaves
float _ppersisrtence = 0.5; // perlin persistance
std::string title;
int smoothness = 4; // multisamples (MSAA), can be 1,2,4,8

int colMode = RGB;
float colMaxR = 0xFF;
float colMaxG = 0xFF;
float colMaxB = 0xFF;
float colMaxA = 0xFF;

int hsbKey = 0xFFFFFF;
float hsbColor[3] = {0.f,0.f,0.f};
color bgColor = 0xff000000, // 0xAARRGGBB
    strokeColor = 0xff0011bb,
    fillColor = 0xff0011bb;

PShape tempShape;
bool tempBuilder;

bool  strokeDraw = true;
bool  fillDraw = true;
int   _strokeCap = ROUND;
int   _strokeJoin = MITER;
float _strokeWidth = 4.f;
int   _rectMode = CORNER;
int   _ellipseMode = CENTER;

// variables taken from header

int mouseX = -1;
int mouseY = -1;
int pmouseX = -1;
int pmouseY = -1;
bool isMousePressed = false;
int displayHeight = 0;
int displayWidth = 0;
int frameCount = 0;
int framerate = 60;
int height = 320;
int width = 320;
int pixelHeight = 1;
int pixelWidth = 1;
bool focused = true;

// =================== methods ======================

void shape(PShape& s){
    std::cout << "size :" << s.getVertexCount() << "\n";
    if(gl != null){
        gl->begin();
        gl->bindShader(sh->program);
        sh->set("mat",mat);
        sh->set("images",0);
        for(int a = 0;a < s.getVertexCount();a++){
            PVector p = s.getVertex(a);
            vert v;
            v.x = p.x;
            v.y = p.y;
            v.z = p.z;
            if(s.isFill){
                v.r = s.fillColor.r;
                v.g = s.fillColor.g;
                v.b = s.fillColor.b;
                v.a = s.fillColor.a;
            }else{
                v.r = s.strokeColor.r;
                v.g = s.strokeColor.g;
                v.b = s.strokeColor.b;
                v.a = s.strokeColor.a;
            }
            _data.push_back(v);
        }
        gl->draw(_data);
        _data.clear();
        gl->end();
    }
}

void tempBuff(int size){
    if(tmp != null)
        delete []tmp;
    tmp = new float[size];
}

void randomSeed(long seed){
    _seed = seed;
}
void noiseSeed(long seed){
    _pseed = seed;
}
void noiseDetail(int lod){
    _poctaves = lod;
}
void noiseDetail(int lod, float falloff){
    _poctaves = lod;
    _ppersisrtence = falloff;
}

template<typename T>
T random(T low, T high) {
    std::mt19937 gen(_seed);
    std::uniform_real_distribution<T> dis(low, high);
    return dis(gen);
}

template<typename T>
T random(T high) {
    return random(0,high);
}

double randomGaussian(){return random(0.0,1.0);}

double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double gradient(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return std::abs(((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v));
}

void toHSB(int color){
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8 ) & 0xFF;
    int b = (color      ) & 0xFF;
    float normR = r / colMaxR;
    float normG = g / colMaxG;
    float normB = b / colMaxB;
    float val_max = max({normR,normG,normB});
    float val_min = min({normR,normG,normB});
    float delta = val_max - val_min;
    hsbKey = color;
    hsbColor[2] = val_max;
    if(val_max == 0.f){
        hsbColor[1] = 0.f;
    }else{
        hsbColor[1] = delta / val_max;
    }
    if (delta == 0.f) {
        hsbColor[0] = 0.f;
    } else if (val_max == normR) {
        hsbColor[0] = 60.f * ((normG - normB) / delta);
    } else if (val_max == normG) {
        hsbColor[0] = 60.f * (2.f + (normB - normR) / delta);
    } else if (val_max == normB) {
        hsbColor[0] = 60.f * (4.f + (normR - normG) / delta);
    }
    if (hsbColor[0] < 0.f) {
        hsbColor[0] += 360.f;
    }
}
int constrain(int amt, int low, int high) {
    if (amt < low) {
        return low;
    } else if (amt > high) {
        return high;
    } else {
        return amt;
    }
}

float constrain(float amt, float low, float high) {
    if (amt < low) {
        return low;
    } else if (amt > high) {
        return high;
    } else {
        return amt;
    }
}
float dist(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

float dist(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
float lerp(float start, float end, float t){
    return start + t * (end - start);
}
float mag(float a, float b) {
    return std::sqrt(a * a + b * b);
}
float mag(float a, float b, float c) {
    return std::sqrt(a * a + b * b + c * c);
}
float map(float value,float start1,float stop1,float start2,float stop2){
    float norm = (value - start1) / (stop1 - start1);
    return start2 + norm * (stop2 - start2);
}
float max(float a, float b) {
    return (a > b) ? a : b;
}
float max(float a, float b, float c) {
    return max(max(a, b), c);
}
int max(int a, int b) {
    return (a > b) ? a : b;
}
int max(int a, int b, int c) {
    return max(max(a, b), c);
}
int max(std::initializer_list<int> list) {
    return *std::max_element(list.begin(), list.end());
}
float max(std::initializer_list<float> list) {
    return *std::max_element(list.begin(), list.end());
}
int min(int a, int b) {
    return (a < b) ? a : b;
}
int min(int a, int b, int c) {
    return min(min(a, b), c);
}
float min(float a, float b) {
    return (a < b) ? a : b;
}
float min(float a, float b, float c) {
    return min(min(a, b), c);
}
int min(std::initializer_list<int> list) {
    return *std::min_element(list.begin(), list.end());
}
float min(std::initializer_list<float> list) {
    return *std::min_element(list.begin(), list.end());
}
float norm(float value,float start,float stop){
    return map(value,start,stop,0.f,1.f);
}
float sq(float n){
    return n*n;
}
float degrees(float radians){
    return radians * (180.f / PI);
}
float radians(float degrees){
    return degrees * (PI / 180.f);
}
float noise(float x, float y, float z){
    long t = _seed;
    randomSeed(_pseed);
    x *= randomGaussian();
    y *= randomGaussian();
    z *= randomGaussian();
    randomSeed(t);
    std::mt19937 gen(_pseed);
    int permutation[256];
    for(int a = 0; a < 256;a++){
        permutation[a] = a;
    }
    std::shuffle(permutation, permutation + 256, gen);
    int X = (int)x & 255;
    int Y = (int)y & 255;
    int Z = (int)z & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    int A = (permutation[X] + Y) % 256;
    int AA = (permutation[A] + Z) % 256;
    int AB = (permutation[(A + 1) % 256] + Z) % 256;
    int B = (permutation[(X + 1) % 256] + Y) % 256;
    int BA = (permutation[B] + Z) % 256;
    int BB = (permutation[(B + 1) % 256] + Z) % 256;

    return  lerp(lerp(lerp(gradient(permutation[AA], x, y, z),
                           gradient(permutation[BA], x - 1, y, z),
                           u),
                      lerp(gradient(permutation[AB], x, y - 1, z),
                           gradient(permutation[BB], x - 1, y - 1, z),
                           u),
                      u),
                      lerp(lerp(gradient(permutation[AA + 1], x, y, z - 1),
                                gradient(permutation[BA + 1], x - 1, y, z - 1),
                                u),
                           lerp(gradient(permutation[AB + 1], x, y - 1, z - 1),
                                gradient(permutation[BB + 1], x - 1, y - 1, z - 1),
                                u),
                           v),
                w
            );
}
float noise(float x, float y){
    return noise(x,y,0.f);
}
float noise(float x){
    return noise(x,0.f);
}
void size(int w, int h){
    height = h;
    if(height < 128)
        height = 128;
    width = w;
    if(width < 128)
        width = 128;
}
void ellipseMode(int mode){
    if(mode == CENTER || mode == RADIUS ||
       mode == CORNER || mode == CORNERS )
    _ellipseMode = mode;
}
void rectMode(int mode){
    if(mode == CENTER || mode == RADIUS ||
        mode == CORNER || mode == CORNERS )
    _rectMode = mode;
}
void noStroke(){
    strokeDraw = false;
}
void stroke(float gray){
    stroke(gray,colMaxA);
}
void stroke(float gray, float alpha){
    stroke(gray,gray,gray,alpha);
}
void stroke(float x, float y, float z){
    stroke(x,y,z,colMaxA);
}
void stroke(float x, float y, float z, float a){
    strokeDraw = true;
    int   r = (int) constrain(x,0.f,colMaxR);
    int   g = (int) constrain(y,0.f,colMaxG);
    int   b = (int) constrain(z,0.f,colMaxB);
    int  _a = (int) constrain(a,0.f,colMaxA);
    strokeColor = (_a << 24) | (r << 16) | (g << 8) | b;
}
void stroke(color c){
    strokeDraw = true;
    strokeColor = c;
}

void noFill(){
    fillDraw = false;
}
void fill(float gray){
    fill(gray,colMaxA);
}
void fill(float gray, float alpha){
    fill(gray,gray,gray,alpha);
}
void fill(float x, float y, float z){
    fill(x,y,z,colMaxA);
}
void fill(float x, float y, float z, float a){
    fillDraw = true;
    int r = constrain(x,0.f,colMaxR);
    int g = constrain(y,0.f,colMaxG);
    int b = constrain(z,0.f,colMaxB);
    int A = constrain(a,0.f,colMaxA);
    fillColor = (A << 24) | (r << 16) | (g << 8) | b;
}
void fill(color c){
    fillDraw = true;
    fillColor = c;
}
void background(float gray){
    background(gray,gray,gray);
}
void background(float gray, float alpha){
    background(gray,gray,gray,alpha);
}
void background(float x, float y, float z){
    background(x,y,z,colMaxA);
}
void background(float x, float y, float z, float a){
    int r = (int) constrain(x,0.f,colMaxR);
    int g = (int) constrain(y,0.f,colMaxG);
    int b = (int) constrain(z,0.f,colMaxB);
    int A = (int) constrain(a,0.f,colMaxA);
    bgColor = (A << 24) | (r << 16) | (g << 8) | b;
}
void background(PImage image){
    todo("background(PImage)");
}
void strokeCap(int c){
    if(c == SQUARE || c == ROUND || c == PROJECT)
        _strokeCap = c;
}
void strokeJoin(int j){
    if(j == MITER || j == ROUND || j == BEVEL)
        _strokeJoin = j;
}
void strokeWeight(float size){
    if(size <= 1.f)
        _strokeWidth = 1.f;
    else
        _strokeWidth = size;
}
void colorMode(int mode){
    assert(mode == RGB || mode == HSB);
    colMode = mode;
}
void colorMode(int mode, float max){
    assert(mode == RGB || mode == HSB);
    colMode = mode;
    colMaxR = max;
    colMaxG = max;
    colMaxB = max;
    colMaxA = max;
}
void colorMode(int mode, float maxX, float maxY, float maxZ){
    assert(mode == RGB || mode == HSB);
    colMode = mode;
    colMaxR = maxX;
    colMaxG = maxY;
    colMaxB = maxZ;
}
void colorMode(int mode, float maxX, float maxY, float maxZ, float maxA){
    assert(mode == RGB || mode == HSB);
    colMode = mode;
    colMaxR = maxX;
    colMaxG = maxY;
    colMaxB = maxZ;
    colMaxA = maxA;
}
color lerpColor(color c1, color c2, float amt){
    float R = lerp(c1.r,c2.r,amt);
    float G = lerp(c1.g,c2.g,amt);
    float B = lerp(c1.b,c2.b,amt);
    float A = lerp(c1.a,c2.a,amt);
    return color(R,G,B,A);
}
color lerpColor(color c1, color c2, float amt, int mode){
    return lerpColor(c1,c2,amt);
}
int lerpColor(int c1, int c2, float amt){
    color cc1 = c1;
    color cc2 = c2;
    return lerpColor(cc1,cc2,amt).i;
}
int lerpColor(int c1, int c2, float amt, int mode){
    return lerpColor(c1,c2,amt);
}

float alpha(int what){
    float outgoing = (what >> 24) & 0xff;
    if (colMaxA == 255) return outgoing;
    return (outgoing / 255.0f) * colMaxA;
}
float red(int what){
    float outgoing = (what >> 16) & 0xff;
    if (colMaxR == 255) return outgoing;
    return (outgoing / 255.0f) * colMaxR;
}
float green(int what){
    float outgoing = (what >> 8) & 0xff;
    if (colMaxG == 255) return outgoing;
    return (outgoing / 255.0f) * colMaxG;
}
float blue(int what){
    float outgoing = what & 0xff;
    if (colMaxB == 255) return outgoing;
    return (outgoing / 255.0f) * colMaxB;
}

float hue(int what){
    if (what != hsbKey) {
        toHSB(what);
        hsbKey = what;
    }
    return hsbColor[0] * colMaxR;
}
float saturation(int what){
    if (what != hsbKey) {
        toHSB(what);
        hsbKey = what;
    }
    return hsbColor[1] * colMaxG;
}
float brightness(int what){
    if (what != hsbKey) {
        toHSB(what);
        hsbKey = what;
    }
    return hsbColor[2] * colMaxB;
}
void smooth(int level){
    smoothness = level;
}
void noSmooth(){
    smoothness = -1;
}
PShader loadShader(const std::string& fragFilename){
    std::string fragCode = readFile(fragFilename);
    return PShader(vertexCode,fragCode);
}
PShader loadShader(const std::string& fragFilename,const std::string&  vertFilename){
    std::string fragCode = readFile(fragFilename);
    std::string vertCode = readFile(vertFilename);
    return PShader(vertCode,fragCode);
}
void resetShader(){
    gl->bindShader(sh->program);
}
void shader(PShader &shader){
    gl->bindShader(shader.program);
}
void pushMatrix(){
    matStack.push(mat);
}
void popMatrix(){
    mat = matStack.top();
    matStack.pop();
}
void translate(float tx, float ty){
    mat.translate(tx,ty);
}
void translate(float tx, float ty, float tz){
    mat.translate(tx,ty,tz);
}
void rotate(float angle){
    mat.rotate(angle);
}
void rotateX(float angle){
    mat.rotateX(angle);
}
void rotateY(float angle){
    mat.rotateY(angle);
}
void rotateZ(float angle){
    mat.rotateZ(angle);
}
void rotate(float angle, float vx, float vy, float vz){
    mat.rotate(angle,vx,vy,vz);
}
void scale(float s){
    mat.scale(s);
}
void scale(float sx, float sy){
    mat.scale(sx,sy);
}
void scale(float x, float y, float z){
    mat.scale(x,y,z);
}
void resetMatrix(){
    mat.reset();
}
void applyMatrix(float n00, float n01, float n02,
                 float n10, float n11, float n12){
    mat.apply(n00,n01,n02,
              n10,n11,n12);
}
void applyMatrix(float n00, float n01, float n02, float n03,
                 float n10, float n11, float n12, float n13,
                 float n20, float n21, float n22, float n23,
                 float n30, float n31, float n32, float n33){
    mat.apply(n00,n01,n02,n03,
              n10,n11,n12,n13,
              n20,n21,n22,n23,
              n30,n31,n32,n33);
}

void printMatrix(){
    using namespace std;
    cout << "MATRIX" << endl;
    cout << mat.m00 << " " << mat.m01 << " " << mat.m02 << " " << mat.m03 << "\n";
    cout << mat.m10 << " " << mat.m11 << " " << mat.m12 << " " << mat.m13 << "\n";
    cout << mat.m20 << " " << mat.m21 << " " << mat.m22 << " " << mat.m23 << "\n";
    cout << mat.m30 << " " << mat.m31 << " " << mat.m32 << " " << mat.m33 << "\n\n";
}

struct Segment{
    PVector p1,p2,p3,p4;
    PVector a,b,n;
};

static Segment genSegment(PVector& a,PVector& b,float thickness){
    PVector v = PVector::sub(a,b).normalize();
    PVector n = PVector(-v.y,v.x).mult(thickness);
    Segment s;
    s.n = n;
    s.a = a;
    s.b = b;
    s.p1 = PVector::add(a,n);
    s.p2 = PVector::sub(a,n);
    s.p3 = PVector::add(b,n);
    s.p4 = PVector::sub(b,n);
    return s;
}

static float angle(PVector& a,PVector& b,PVector& c){
    PVector x = PVector::sub(a,b);
    PVector y = PVector::sub(c,b);
    float d = PVector::dot(x,y);
    return acos(d / (x.mag() * y.mag()));
}

static PVector rotate(PVector &v,PVector &p, float theta,float radius){
    float s = sin(theta);
    float c = cos(theta);
    float newX = (v.x - p.x) * c - (v.y - p.y) * s + p.x;
    float newY = (v.x - p.x) * s + (v.y - p.y) * c + p.y;
    return PVector(newX,newY).setMag(radius);
}

static std::vector<PVector> triangles(PVector &c, float startAngle, float endAngle,float r,int l = 16){
    float step = (endAngle - startAngle) / (float)l;
    std::vector<PVector> list;
    for(float a = startAngle; a <= endAngle; a+= step){
        list.push_back(c);
        list.push_back(c);
        list.push_back(c);
    }
    return list;
}

void strokify(std::vector<PVector> points,bool loop = false,bool allowOverlap = false){
    float thickness = _strokeWidth / 2.f;
    PShape _shape;
    _shape.beginShape();
    _shape.noStroke();
    _shape.fill(strokeColor);
    std::vector<Segment> segments;
    if(points.size() > 1){
        for(int a = 0;a < points.size() - 1;a++){
            segments.push_back(genSegment(points[a],points[a+1],thickness));
        }
        if(loop){
            segments.push_back(genSegment(points.front(),points.back(),thickness));
        }else{
            if(_strokeCap == ROUND){

            }else if(_strokeCap == SQUARE){
                Segment& t = segments.back();
                PVector y = PVector(-t.n.y,t.n.x);
                t.p3.add(y);
                t.p4.add(y);

                Segment& u = segments.front();
                y = PVector(-u.n.y,u.n.x);
                u.p1.sub(y);
                u.p2.sub(y);
            }
        }
    }

    for(int a = 0;a < segments.size();a++){
        if(a >= 0 && a+1 != segments.size()){
            Segment _a = segments[a];
            Segment _b = segments[a+1];
            if(_strokeJoin == ROUND){

            }else if(_strokeJoin == MITER){
                PVector _pa = PVector::sub(_a.b,_a.p3).normalize();
                PVector _pb = PVector::sub(_a.b,_b.p1).normalize();
                PVector m = PVector::add(_pa,_pb);//.mult(thickness);
                std::cout << m.toString();
                //float l = m.
                _shape.vertex(_a.b);
                _shape.vertex(_a.p3);
                _shape.vertex(_b.p1);

                _shape.vertex(PVector::sub(_a.b,m));
                _shape.vertex(_a.p3);
                _shape.vertex(_b.p1);

            }else if(_strokeJoin == BEVEL){
                _shape.vertex(_a.b);
                _shape.vertex(_a.p3);
                _shape.vertex(_b.p1);
            }
        }
        _shape.vertex(segments[a].p1);
        _shape.vertex(segments[a].p2);
        _shape.vertex(segments[a].p3);
        _shape.vertex(segments[a].p3);
        _shape.vertex(segments[a].p2);
        _shape.vertex(segments[a].p4);
    }
    segments.clear();
    _shape.endShape();
    shape(_shape);
}

void strokify(PShape shape,bool loop = false){
    strokify(shape.vertices(),loop);
}

void beginShape(int mode){
    if(!tempBuilder){
        tempBuilder = true;
        tempShape = PShape();
        if(strokeDraw){
            tempShape.setStroke(strokeColor);
        }
        if(fillDraw){
            tempShape.setFill(fillColor);
        }
    }else{
        error("Close current shape");
    }
}
void vertex(float x,float y){
    vertex(x,y,0.f);
}
void vertex(float x,float y,float z){
    if(tempBuilder){
        tempShape.vertex(x,y,z);
    }else{
        error("vertex(...) can be used only between beginShape() and endshape() functions");
    }
}
void endShape(){
    if(tempBuilder){
        if(strokeDraw){
            strokify(tempShape);
        }
        if(fillDraw){
            shape(tempShape);
        }
        tempBuilder = false;
    }else{
        error("No shape is creating");
    }
}

void arc(float x, float y, float w, float h, float start, float stop){
    PShape s;
    float step = radians(1);
    s.beginShape();
    s.fill(fillColor);
    s.noStroke();
    if(start > stop){
        std::swap(start,stop);
    }
    do{
        s.vertex(x,y);
        s.vertex(x + cos(start)*w,y + sin(start)*h);
        start += step;
        s.vertex(x + cos(start)*w,y + sin(start)*h);
    }while(start < stop);
    s.endShape();
    shape(s);
}
void arc(float x, float y, float w, float h, float start, float stop, int mode){
    arc(x,y,w,h,start,stop);
}
void circle(float x, float y, float extent){
    ellipse(x,y,extent,extent);
}
void ellipse(float x, float y, float w, float h){
    int step = 10;
    PShape s;
    s.beginShape();
    s.fill(fillColor);
    s.noStroke();
    float d = 0.f;
    for(int a = 0;a < 360;a+=step){
        s.vertex(x,y);
        s.vertex(x + cos(d)*w,y + sin(d)*h);
        d = radians(a+step);
        s.vertex(x + cos(d)*w,y + sin(d)*h);
    }
    s.endShape();
    shape(s);
}
void line(float x1, float y1, float x2, float y2){
    std::vector<PVector> data = {
        PVector(x1,y1),
        PVector(x2,y2)
    };
    strokify(data);
}
void line(float x1, float y1, float z1, float x2, float y2, float z2){
    line(x1,y1,x2,y2);
}
void point(float x, float y){
    int t = fillColor.i;
    fill(strokeColor);
    circle(x,y,_strokeWidth/2.f);
    fill(t);
}
void point(float x, float y, float z){
    point(x,y);
}
void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    if(fillDraw){
        PShape s;
        s.beginShape();
        s.fill(fillColor);
        s.noStroke();
        s.vertex(x1,y1);
        s.vertex(x2,y2);
        s.vertex(x3,y3);
        s.vertex(x2,y2);
        s.vertex(x3,y3);
        s.vertex(x4,y4);
        s.endShape();
        shape(s);
    }
    if(strokeDraw){
        strokify({
            PVector(x1,y1),
            PVector(x2,y2),
            PVector(x3,y3),
            PVector(x4,y4)
        },true);
    }
}
void rect(float x, float y, float w, float h, float r){
    todo("rect(float x, float y, float w, float h, float r)");
    rect(x,y,w,h,r,r,r,r);
}
void rect(float x, float y, float w, float h, float tl, float tr, float br, float bl){
    todo("rect(float x, float y, float w, float h, float tl, float tr, float br, float bl)");
    rect(x,y,w,h);
}
void square(float x,float y,float s){
    rect(x,y,s,s);
}
void triangle(float x1,float y1,float x2,float y2,float x3,float y3){
    PShape s;
    s.beginShape();
    s.fill(fillColor);
    s.noStroke();
    s.vertex(x1,y1);
    s.vertex(x2,y2);
    s.vertex(x3,y3);
    s.endShape();
    shape(s);
}
void rect(float x, float y, float w, float h){
    if(fillDraw){
        PShape s;
        s.beginShape();
        s.fill(fillColor);
        s.noStroke();
        s.vertex(x,y);
        s.vertex(x+w,y);
        s.vertex(x,y+h);
        s.vertex(x+w,y);
        s.vertex(x,y+h);
        s.vertex(x+w,y+h);
        s.endShape();
        shape(s);
    }
    if(strokeDraw){
        strokify({
            PVector(x,y),
            PVector(x+w,y),
            PVector(x+w,y+h),
            PVector(x,y+h)
        },true);
    }
}

void frustum(float left,float right,float bottom,float top,float near,float far){
    mat.reset();
    mat.m00 = (2.f * near)/(right-left);
    mat.m02 = (right+left)/(right-left);
    mat.m11 = (2.f * near)/(top-bottom);
    mat.m12 = (top+bottom)/(top-bottom);
    mat.m22 = -(far+near)/(far-near);
    mat.m23 = -(2.0f * far * near) / (far - near);
    mat.m32 = -1.f;
    mat.m33 = 0.f;
}
void ortho(){
    ortho(-width/2, width/2, -height/2, height/2);
}
void ortho(float left,float right,float bottom,float top){
    ortho(left,right,bottom,top,0.1f,-0.1f);
}
void ortho(float left,float right,float bottom,float top,float near,float far){
    mat.reset();
    mat.m00 = 2.0f / (right - left);
    mat.m11 = 2.0f / (top - bottom);
    mat.m22 = -2.0f / (far - near);
    mat.m33 = 1.f;
    mat.m30 = -(right + left) / (right - left);
    mat.m31 = -(top + bottom) / (top - bottom);
    mat.m32 = -(far + near) / (far - near);
}
void perspective(){
    float cameraZ = ((height/2.0) / tan(PI*60.0/360.0));
    perspective(PI/3.0, (float)width/height, cameraZ/10.0, cameraZ*10.0);
}
void perspective(float fov, float aspect,float zNear,float zFar){
    mat.reset();
    float tanHalfFOV = tan(fov / 2.0f);
    float right = zNear * tanHalfFOV;
    float left = -right;
    float top = right / aspect;
    float bottom = -top;

    mat.m00 = 2.0f * zNear / (right - left);
    mat.m11 = 2.0f * zNear / (top - bottom);
    mat.m20 = (right + left) / (right - left);
    mat.m21 = (top + bottom) / (top - bottom);
    mat.m22 = -(zFar + zNear) / (zFar - zNear);
    mat.m23 = -1.0f;
    mat.m32 = -(2.0f * zFar * zNear) / (zFar - zNear);

}
// ============== classes ======================

color::color(int gray){
    this->r = gray / colMaxR;
    this->g = gray / colMaxG;
    this->b = gray / colMaxB;
    this->a = colMaxA;
    this->i = ((int)this->a << 24) | ((int)this->r << 16) | ((int)this->g << 8) | (int)this->b;
}
color::color(int gray, int alpha){
    this->r = gray  / colMaxR;
    this->g = gray  / colMaxG;
    this->b = gray  / colMaxB;
    this->a = alpha / colMaxA;
    this->i = ((int)this->a << 24) | ((int)this->r << 16) | ((int)this->g << 8) | (int)this->b;
}
color::color(int rgb, float alpha){
    this->r = ((rgb >> 16) & 0xFF) / colMaxR;
    this->g = ((rgb >> 8) & 0xFF) / colMaxG;
    this->b = ((rgb) & 0xFF) / colMaxB;
    this->a = colMaxA;
    this->i = ((int)this->a << 24) | ((int)this->r << 16) | ((int)this->g << 8) | (int)this->b;
}
color::color(int x, int y, int z){
    this->r = x / colMaxR;
    this->g = y / colMaxG;
    this->b = z / colMaxB;
    this->a = colMaxA;
    this->i = ((int)this->a << 24) | ((int)this->r << 16) | ((int)this->g << 8) | (int)this->b;
}
color::color(int x, int y, int z,int w){
    this->r = x / colMaxR;
    this->g = y / colMaxG;
    this->b = z / colMaxB;
    this->a = w / colMaxA;
    this->i = ((int)this->a << 24) | ((int)this->r << 16) | ((int)this->g << 8) | (int)this->b;

}
color& color::operator=(int argb){
    this->i = argb;
    this->a = ((argb >> 24) & 0xFF) / colMaxA;
    this->r = ((argb >> 16) & 0xFF) / colMaxR;
    this->g = ((argb >> 8) & 0xFF) / colMaxG;
    this->b = ((argb) & 0xFF) / colMaxB;
    return *this;
}
std::string color::toString(){
    std::stringstream ss;
    ss << "color(" << r << "," << g << "," << b << "," << a << "\n";
    ss << "0x" << std::hex  << i << ")\n";
    return ss.str();
}
PVector::PVector() : PVector(0.f,0.f,0.f){}
PVector::PVector(float _x,float _y) : PVector(_x,_y,0.f){}
PVector::PVector(float _x,float _y,float _z){
    this->x = _x;
    this->y = _y;
    this->z = _z;
}
std::string PVector::toString(){
    char buf[32];
    std::sprintf(buf,"[%.2f,%.2f,%.2f]",x,y,z);
    return std::string(buf);
}
PVector PVector::set(float _x,float _y,float _z){
    this->x = _x;
    this->y = _y;
    this->z = _z;
    return *this;
}
PVector PVector::set(float _x,float _y){
    return this->set(_x,_y,0.f);
}
PVector PVector::set(const PVector &v){
    return this->set(v.x,v.y,v.z);
}
PVector PVector::set(float v[3]){
    return this->set(v[0],v[1],v[2]);
}
PVector PVector::random2D(){
    return PVector(
        random(-1.f,1.f),
        random(-1.f,1.f)
    );
}
PVector PVector::random2D(PVector& target){
    target.set(
        random(-1.f,1.f),
        random(-1.f,1.f)
    );
    return target;
}
PVector PVector::random3D(){
    return PVector(
        random(-1.f,1.f),
        random(-1.f,1.f),
        random(-1.f,1.f)
    );
}
PVector PVector::random3D(PVector& target){
    target.set(
        random(-1.f,1.f),
        random(-1.f,1.f),
        random(-1.f,1.f)
    );
    return target;
}
PVector PVector::fromAngle(float angle){
    return PVector(sin(angle),cos(angle));
}
PVector PVector::fromAngle(float angle, PVector& target){
    return target.set(sin(angle),cos(angle));
}
PVector PVector::copy(){
    return PVector(this->x,this->y,this->z);
}
PVector PVector::copy() const{
    return PVector(this->x,this->y,this->z);
}
float PVector::mag(){
    return sqrt(x * x + y * y + z * z);
}
float PVector::magSq(){
    return x * x + y * y + z * z;
}
float PVector::mag() const{
    return sqrt(x * x + y * y + z * z);
}
float PVector::magSq() const{
    return x * x + y * y + z * z;
}
PVector PVector::add(const PVector &v){
    return this->add(v.x,v.y,v.z);
}
PVector PVector::add(float x, float y){
    return this->add(x,y,0.f);
}
PVector PVector::add(float x, float y, float z){
    this->x += x;
    this->y += y;
    this->z += z;
    return *this;
}
PVector PVector::add(const PVector& v1,const  PVector& v2){
    PVector t;
    return add(v1,v2,t);
}
PVector PVector::add(const PVector& v1,const  PVector& v2, PVector& target){
    target.set(v1);
    target.add(v2);
    return target;
}
PVector PVector::sub(const PVector& v){
    return this->sub(v.x,v.y,v.z);
}
PVector PVector::sub(float x, float y){
    return this->sub(x,y,0.f);
}
PVector PVector::sub(float x, float y, float z){
    this->x -= x;
    this->y -= y;
    this->z -= z;
    return *this;
}
PVector PVector::sub(const PVector &v1, const PVector &v2){
    PVector t;
    return PVector::sub(v1,v2,t);
}
PVector PVector::sub(const PVector& v1,const PVector& v2, PVector& target){
    target.set(v1);
    target.sub(v2);
    return target;
}
PVector PVector::mult(float n){
    this->x *= n;
    this->y *= n;
    this->z *= n;
    return *this;
}
PVector PVector::mult(const PVector& v, float n){
    PVector t = v.copy();
    t.mult(n);
    return t;
}
PVector PVector::mult(const PVector& v, float n, PVector& target){
    target.set(v);
    target.mult(n);
    return target;
}
PVector PVector::div(float n){
    this->x /= n;
    this->y /= n;
    this->z /= n;
    return *this;
}
PVector PVector::div(const PVector& v, float n){
    PVector t = v.copy();
    t.div(n);
    return t;
}
PVector PVector::div(const PVector& v, float n, PVector& target){
    target.set(v);
    target.div(n);
    return target;
}
float PVector::dist(const PVector& v){
    float dx = this->x - v.x;
    float dy = this->y - v.y;
    float dz = this->z - v.z;
    return std::sqrt(dx*dx+dy*dy+dz*dz);
}
float PVector::dist(const PVector& v1,const PVector& v2){
    PVector t = v1;
    return t.dist(v2);
}
float PVector::dot(const PVector& v){
    return this->dot(v.x,v.y,v.z);
}
float PVector::dot(float x, float y, float z){
    float dx = this->x * x;
    float dy = this->y * y;
    float dz = this->z * z;
    return dx+dy+dz;
}
float PVector::dot(const PVector& v1,const  PVector& v2){
    PVector t = v1;
    return t.dot(v2);
}
PVector PVector::cross(const PVector& v){
    float rX = this->y * v.z - this->z * v.y;
    float rY = this->z * v.x - this->x * v.z;
    float rZ = this->x * v.y - this->y * v.x;
    return PVector(rX,rY,rZ);
}
PVector PVector::cross(const PVector& v, PVector& target){
    PVector t = target.cross(v);
    return target.set(t);
}
PVector PVector::cross(const PVector& v1, const PVector& v2, PVector& target){
    target.set(v1);
    return target.cross(v2);
}
PVector PVector::normalize(){
    float m = this->mag();
    if(m != 0.0f){
        return this->div(m);
    }else{
        return *this;
    }
}
PVector PVector::normalize(PVector& target){
    return target.set(this->normalize());
}
PVector PVector::limit(float max){
    float m = this->mag();
    if(max < m){
        float scale = max / m;
        return this->mult(scale);
    }else{
        return *this;
    }
}
PVector PVector::setMag(float len){
    return this->normalize().mult(len);
}
PVector PVector::setMag(PVector& target,float len){
    return target.setMag(len);
}
float PVector::heading(){
    return std::atan2(this->y, this->x);
}
PVector PVector::rotate(float theta){
    float m = this->mag();
    double c = std::cos(theta);
    double s = std::sin(theta);
    this->x = x * c - y * s;
    this->y = x * s + y * c;
    return this->setMag(m);
}
PVector PVector::lerp(const PVector& v,float amt){
    return this->lerp(v.x,v.y,v.z,amt);
}
PVector PVector::lerp(const PVector& v1,const PVector& v2, float amt){
    PVector t = v1;
    return t.lerp(v2,amt);
}
PVector PVector::lerp(float x,float y,float z, float amt){
    this->x = this->x + amt * (x - this->x);
    this->y = this->y + amt * (y - this->y);
    this->z = this->z + amt * (z - this->z);
    return *this;
}
float PVector::angleBetween(const PVector& v1,const PVector& v2){
    float m1 = v1.mag();
    float m2 = v2.mag();
    float d = dot(v1,v2);
    return acos(d / (m1 * m2));
}
float* PVector::array(){
    return new float[3]{this->x,this->y,this->z};
}
bool PVector::equal(PVector v){
    return this->x == v.x && this->y == v.y && this->z == v.z;
}
std::string readFile(const std::string& filename){
    std::stringstream ss;
    std::ifstream inputFile(filename);
    if (!inputFile) {
        error("Failed to open file: " + filename);
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        ss << line;
    }
    inputFile.close();
    return ss.str();
}
PShader::PShader() : PShader(vertexCode.c_str(),fragmentCode.c_str()){/* ... */}
void PShader::create(const char* vert,const char* frag){
    if(gl != null){
        int v = gl->compileShader(vert,SHADER_VERTEX);
        int f = gl->compileShader(frag,SHADER_FRAGMENT);
        program = gl->linkProgram({v,f});
    }else{
        error("GL == NULL");
    }
}
PShader::PShader(const std::string& vert,const std::string& frag){
    create(readFile(vert).c_str(),readFile(frag).c_str());
}
PShader::PShader(const char* vert,const char* frag){
    this->create(vert,frag);
}
void PShader::set(const std::string &name, int x){
    gl->setUniform(program,name,x);
}
void PShader::set(const std::string &name, float x){
    gl->setUniform(program,name,x);
}
void PShader::set(const std::string &name, bool x){
    gl->setUniform(program,name,x);
}
void PShader::set(const std::string &name, int x, int y){
    gl->setUniform(program,name,x,y);
}
void PShader::set(const std::string &name, float x, float y){
    gl->setUniform(program,name,x,y);
}
void PShader::set(const std::string &name, bool x, bool y){
    gl->setUniform(program,name,x,y);
}
void PShader::set(const std::string &name, int x, int y, int z){
    gl->setUniform(program,name,x,y,z);
}
void PShader::set(const std::string &name, float x, float y, float z){
    gl->setUniform(program,name,x,y,z);
}
void PShader::set(const std::string &name, bool x, bool y, bool z){
    gl->setUniform(program,name,x,y,z);
}
void PShader::set(const std::string &name, int x, int y, int z, int w){
    gl->setUniform(program,name,x,y,z,w);
}
void PShader::set(const std::string &name, float x, float y, float z, float w){
    gl->setUniform(program,name,x,y,z,w);
}
void PShader::set(const std::string &name, bool x, bool y, bool z, bool w){
    gl->setUniform(program,name,x,y,z,w);
}
void PShader::set(const std::string &name, int* vec){
    gl->setUniform(program,name,vec);
}
void PShader::set(const std::string &name, float* vec){
    gl->setUniform(program,name,vec);
}
void PShader::set(const std::string &name, bool* vec){
    gl->setUniform(program,name,vec);
}
void PShader::set(const std::string &name, int* vec,int ncoords){
    gl->setUniform(program,name,vec,ncoords);
}
void PShader::set(const std::string &name, float* vec,int ncoords){
    gl->setUniform(program,name,vec,ncoords);
}
void PShader::set(const std::string &name, bool* vec,int ncoords){
    gl->setUniform(program,name,vec,ncoords);
}
void PShader::set(const std::string &name, PMatrix2D &mat){
    mat2Data* temp = new mat2Data();
    mat.get(temp);
    gl->setUniform(program,name,temp->data(),temp->size());
    delete(temp);
}
void PShader::set(const std::string &name, PMatrix3D &mat){
    mat4Data* temp = new mat4Data();
    mat.get(temp);
    gl->setUniform(program,name,temp->data(),temp->size());
    delete(temp);
}
void PShader::set(const std::string &name, PMatrix2D &mat, bool use3x3){
    if(use3x3){
        tempBuff(9);
        mat2Data* temp = new mat2Data();
        mat.get(temp);
        tmp[0] = temp->at(0);
        tmp[1] = temp->at(1);
        tmp[2] = temp->at(2);
        tmp[3] = temp->at(3);
        tmp[4] = temp->at(4);
        tmp[5] = temp->at(5);
        tmp[6] = 0.f;
        tmp[7] = 0.f;
        tmp[8] = 0.f;
        gl->setUniform(program,name,tmp,9);
        delete temp;
    }else{
        set(name,mat);
    }
}
void PShader::set(const std::string &name, PMatrix3D &mat, bool use3x3){
    if(use3x3){
        tempBuff(9);
        mat4Data* temp = new mat4Data();
        mat.get(temp);
        tmp[0] = temp->at(0);
        tmp[1] = temp->at(1);
        tmp[2] = temp->at(2);
        tmp[3] = temp->at(4);
        tmp[4] = temp->at(5);
        tmp[5] = temp->at(6);
        tmp[6] = temp->at(8);
        tmp[7] = temp->at(9);
        tmp[8] = temp->at(10);
        gl->setUniform(program,name,tmp,9);
        delete temp;
    }else{
        set(name,mat);
    }
}
void PShader::set(const std::string &name, PImage &tex){
    todo("PShader::set(const std::string&, PImage&)");
}
std::string PShader::toString(){
    std::stringstream ss;
    ss << "PShader(" << program << ")\n";
    return ss.str();
}
PMatrix2D::PMatrix2D() : PMatrix2D(0.f,0.f,0.f,0.f,0.f,0.f){}
PMatrix2D::PMatrix2D(mat2Data* mat){
    set(mat);
}
PMatrix2D::PMatrix2D(PMatrix2D* mat){
    mat2Data* data = new mat2Data;
    mat->get(data);
    set(data);
    delete data;
}
PMatrix2D::PMatrix2D(
    float a00,float a01,float a02,
    float a10,float a11,float a12
){
    set(a00,a01,a02,
        a10,a11,a12);
}
void PMatrix2D::reset(){
    set(1.f,0.f,0.f,
        0.f,1.f,0.f);
}
PMatrix2D* PMatrix2D::get(){
    return this;
}
mat2Data* PMatrix2D::get(mat2Data* target){
    if(target == null){
        target = new mat2Data;
    }
    float* p = target->data();
    *(p+0) = m00;
    *(p+1) = m01;
    *(p+2) = m02;
    *(p+3) = m10;
    *(p+4) = m11;
    *(p+5) = m12;
    return target;
}
void PMatrix2D::set(mat2Data* source){
    set(source->at(0),source->at(1),source->at(2),
        source->at(3),source->at(4),source->at(5));
}
void PMatrix2D::set(float m00, float m01, float m02,
                    float m10, float m11, float m12){
    this->m00 = m00;
    this->m01 = m01;
    this->m02 = m02;
    this->m10 = m10;
    this->m11 = m11;
    this->m12 = m12;
}
void PMatrix2D::translate(float tx, float ty){
    this->m02 = tx*m00 + ty*m01 + m02;
    this->m12 = tx*m10 + ty*m11 + m12;
}
void PMatrix2D::rotate(float angle){
    float s = sin(angle);
    float c = cos(angle);
    float temp1 = m00;
    float temp2 = m01;
    this->m00 =  c * temp1 + s * temp2;
    this->m01 = -s * temp1 + c * temp2;
    temp1 = m10;
    temp2 = m11;
    this->m10 =  c * temp1 + s * temp2;
    this->m11 = -s * temp1 + c * temp2;
}
void PMatrix2D::rotateZ(float angle){
    rotate(angle);
}
void PMatrix2D::scale(float s){
    scale(s, s);
}
void PMatrix2D::scale(float sx, float sy){
    this->m00 *= sx;
    this->m01 *= sy;
    this->m10 *= sx;
    this->m11 *= sy;
}
void PMatrix2D::shearX(float angle){
    float t = tan(angle);
    apply(1, 0, 1,
          t, 0, 0);
}
void PMatrix2D::shearY(float angle){
    float t = tan(angle);
    apply(1, 0, 1,
          0, t, 0);
}
void PMatrix2D::apply(PMatrix2D* source){
    apply(source->m00,source->m01,source->m02,
          source->m10,source->m11,source->m12);
}
void PMatrix2D::apply(float n00, float n01, float n02,
                      float n10, float n11, float n12){
    float t0 = m00;
    float t1 = m01;
    this->m00  = n00 * t0 + n10 * t1;
    this->m01  = n01 * t0 + n11 * t1;
    this->m02 += n02 * t0 + n12 * t1;
    t0 = m10;
    t1 = m11;
    this->m10  = n00 * t0 + n10 * t1;
    this->m11  = n01 * t0 + n11 * t1;
    this->m12 += n02 * t0 + n12 * t1;
}
void PMatrix2D::preApply(PMatrix2D* source){
    preApply(source->m00,source->m01,source->m02,
             source->m10,source->m11,source->m12);
}
void PMatrix2D::preApply(float n00, float n01, float n02,
              float n10, float n11, float n12){

    float t0 = m02;
    float t1 = m12;
    n02 += t0 * n00 + t1 * n01;
    n12 += t0 * n10 + t1 * n11;

    this->m02 = n02;
    this->m12 = n12;

    t0 = m00;
    t1 = m10;
    this->m00 = t0 * n00 + t1 * n01;
    this->m10 = t0 * n10 + t1 * n11;

    t0 = m01;
    t1 = m11;
    this->m01 = t0 * n00 + t1 * n01;
    this->m11 = t0 * n10 + t1 * n11;
}
PVector* PMatrix2D::mult(PVector* source, PVector* target){
    if (target == null) {
        target = new PVector();
    }
    target->x = m00*source->x + m01*source->y + m02;
    target->y = m10*source->x + m11*source->y + m12;
    return target;
}
float* PMatrix2D::mult(float* vec, float* out){
    if (out == null) {
        out = new float[2];
    }
    if (vec == out) {
        float tx = m00*vec[0] + m01*vec[1] + m02;
        float ty = m10*vec[0] + m11*vec[1] + m12;
        out[0] = tx;
        out[1] = ty;
    } else {
        out[0] = m00*vec[0] + m01*vec[1] + m02;
        out[1] = m10*vec[0] + m11*vec[1] + m12;
    }
    return out;
}
bool PMatrix2D::invert(){
    float d = determinant();
    if (abs(d) <= 0) {
        return false;
    }

    float t00 = m00;
    float t01 = m01;
    float t02 = m02;
    float t10 = m10;
    float t11 = m11;
    float t12 = m12;

    m00 =  t11 / d;
    m10 = -t10 / d;
    m01 = -t01 / d;
    m11 =  t00 / d;
    m02 = (t01 * t12 - t11 * t02) / d;
    m12 = (t10 * t02 - t00 * t12) / d;
    return true;
}
float PMatrix2D::determinant(){
    return m00 * m11 - m01 * m10;
}
std::string PMatrix2D::toString(){
    std::stringstream ss;
    ss << "PMatrix2D(" << m00 << ","<< m01 << ","<< m02 << "," << std::endl;
    ss << "          " << m10 << ","<< m11 << ","<< m12 << ")" << std::endl;
    return ss.str();
}
PMatrix3D::PMatrix3D(){
    reset();
}
PMatrix3D::PMatrix3D(mat4Data* mat){
    set(mat);
}
PMatrix3D::PMatrix3D(PMatrix3D* mat){
    set(mat);
}
PMatrix3D::PMatrix3D(float m00, float m01, float m02,
          float m10, float m11, float m12) {
    set(m00, m01, m02, 0,
        m10, m11, m12, 0,
        0,   0,   1,   0,
        0,   0,   0,   1);
}
PMatrix3D::PMatrix3D(
    float a00, float a01, float a02, float a03,
    float a10, float a11, float a12, float a13,
    float a20, float a21, float a22, float a23,
    float a30, float a31, float a32, float a33
){
    set(a00,a01,a02,a03,
        a10,a11,a12,a13,
        a20,a21,a22,a23,
        a30,a31,a32,a33);
}
void PMatrix3D::reset(){
    set(1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}
PMatrix3D* PMatrix3D::get(){
    return this;
}
mat4Data* PMatrix3D::get(mat4Data* target){
    if(target == null){
        target = new mat4Data;
    }
    float* p = target->data();
    *(p+0 ) = m00;
    *(p+1 ) = m01;
    *(p+2 ) = m02;
    *(p+3 ) = m03;
    *(p+4 ) = m10;
    *(p+5 ) = m11;
    *(p+6 ) = m12;
    *(p+7 ) = m13;
    *(p+8 ) = m20;
    *(p+9 ) = m21;
    *(p+10) = m22;
    *(p+11) = m23;
    *(p+12) = m30;
    *(p+13) = m31;
    *(p+14) = m32;
    *(p+15) = m33;
    return target;
}
void PMatrix3D::set(PMatrix3D* src){
    set(src->m00,src->m01,src->m02,src->m03,
        src->m10,src->m11,src->m12,src->m13,
        src->m20,src->m21,src->m22,src->m23,
        src->m30,src->m31,src->m32,src->m33);
}
void PMatrix3D::set(mat4Data* source){
    set(
        source->at(0), source->at(1),
        source->at(2), source->at(3),
        source->at(4), source->at(5),
        source->at(6), source->at(7),
        source->at(8), source->at(9),
        source->at(10),source->at(11),
        source->at(12),source->at(13),
        source->at(14),source->at(15)
    );
}
void PMatrix3D::set(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33
){
    this->m00 = m00; this->m01 = m01;
    this->m02 = m02; this->m03 = m03;
    this->m10 = m10; this->m11 = m11;
    this->m12 = m12; this->m13 = m13;
    this->m20 = m20; this->m21 = m21;
    this->m22 = m22; this->m23 = m23;
    this->m30 = m30; this->m31 = m31;
    this->m32 = m32; this->m33 = m33;
}
void PMatrix3D::translate(float tx, float ty){
    translate(tx,ty,0.f);
}
void PMatrix3D::translate(float tx, float ty, float tz){
    m03 += tx*m00 + ty*m01 + tz*m02;
    m13 += tx*m10 + ty*m11 + tz*m12;
    m23 += tx*m20 + ty*m21 + tz*m22;
    m33 += tx*m30 + ty*m31 + tz*m32;
}
void PMatrix3D::rotate(float angle){
    rotateZ(angle);
}
void PMatrix3D::rotateX(float angle){
    float c = cos(angle);
    float s = sin(angle);
    apply(1, 0, 0, 0,
          0, c, -s, 0,
          0, s, c, 0,
          0, 0, 0, 1);
}
void PMatrix3D::rotateY(float angle){
    float c = cos(angle);
    float s = sin(angle);
    apply(c, 0, s, 0,
          0, 1, 0, 0,
          -s, 0, c, 0,
          0, 0, 0, 1);
}
void PMatrix3D::rotateZ(float angle){
    float c = cos(angle);
    float s = sin(angle);
    apply(c, -s, 0, 0,
          s, c, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1);
}
void PMatrix3D::rotate(float angle, float v0, float v1, float v2){
    float norm2 = v0 * v0 + v1 * v1 + v2 * v2;
    if (norm2 < EPSILON) {
        // The vector is zero, cannot apply rotation.
        return;
    }

    if (abs(norm2 - 1) > EPSILON) {
        // The rotation vector is not normalized.
        float norm = sqrt(norm2);
        v0 /= norm;
        v1 /= norm;
        v2 /= norm;
    }

    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0f - c;

    apply((t*v0*v0) + c, (t*v0*v1) - (s*v2), (t*v0*v2) + (s*v1), 0,
          (t*v0*v1) + (s*v2), (t*v1*v1) + c, (t*v1*v2) - (s*v0), 0,
          (t*v0*v2) - (s*v1), (t*v1*v2) + (s*v0), (t*v2*v2) + c, 0,
          0, 0, 0, 1);
}
void PMatrix3D::scale(float s){
    scale(s, s, s);
}
void PMatrix3D::scale(float sx, float sy){
    scale(sx, sy, 1.f);
}
void PMatrix3D::scale(float x, float y, float z){
    m00 *= x;  m01 *= y;  m02 *= z;
    m10 *= x;  m11 *= y;  m12 *= z;
    m20 *= x;  m21 *= y;  m22 *= z;
    m30 *= x;  m31 *= y;  m32 *= z;
}
void PMatrix3D::shearX(float angle){
    float t = tan(angle);
    apply(1, t, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1);
}
void PMatrix3D::shearY(float angle){
    float t = tan(angle);
    apply(1, 0, 0, 0,
          t, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1);
}
void PMatrix3D::apply(PMatrix3D* source){
    apply(
        source->m00,source->m01,source->m02,source->m03,
        source->m10,source->m11,source->m12,source->m13,
        source->m20,source->m21,source->m22,source->m23,
        source->m30,source->m31,source->m32,source->m33
    );
}
void PMatrix3D::apply(
    float n00, float n01, float n02,
    float n10, float n11, float n12
){
    apply(  n00,n01,n02,0.f,
            n10,n11,n12,0.f,
            0.f,0.f,1.f,0.f,
            0.f,0.f,0.f,1.f);
}
void PMatrix3D::apply(
    float n00, float n01, float n02, float n03,
    float n10, float n11, float n12, float n13,
    float n20, float n21, float n22, float n23,
    float n30, float n31, float n32, float n33
){
    float r00 = m00*n00 + m01*n10 + m02*n20 + m03*n30;
    float r01 = m00*n01 + m01*n11 + m02*n21 + m03*n31;
    float r02 = m00*n02 + m01*n12 + m02*n22 + m03*n32;
    float r03 = m00*n03 + m01*n13 + m02*n23 + m03*n33;
    float r10 = m10*n00 + m11*n10 + m12*n20 + m13*n30;
    float r11 = m10*n01 + m11*n11 + m12*n21 + m13*n31;
    float r12 = m10*n02 + m11*n12 + m12*n22 + m13*n32;
    float r13 = m10*n03 + m11*n13 + m12*n23 + m13*n33;
    float r20 = m20*n00 + m21*n10 + m22*n20 + m23*n30;
    float r21 = m20*n01 + m21*n11 + m22*n21 + m23*n31;
    float r22 = m20*n02 + m21*n12 + m22*n22 + m23*n32;
    float r23 = m20*n03 + m21*n13 + m22*n23 + m23*n33;
    float r30 = m30*n00 + m31*n10 + m32*n20 + m33*n30;
    float r31 = m30*n01 + m31*n11 + m32*n21 + m33*n31;
    float r32 = m30*n02 + m31*n12 + m32*n22 + m33*n32;
    float r33 = m30*n03 + m31*n13 + m32*n23 + m33*n33;
    this->m00 = r00;this->m01 = r01;this->m02 = r02;this->m03 = r03;
    this->m10 = r10;this->m11 = r11;this->m12 = r12;this->m13 = r13;
    this->m20 = r20;this->m21 = r21;this->m22 = r22;this->m23 = r23;
    this->m30 = r30;this->m31 = r31;this->m32 = r32;this->m33 = r33;
}

void PMatrix3D::preApply(PMatrix3D* source){
    preApply(
        source->m00,source->m01,source->m02,source->m03,
        source->m10,source->m11,source->m12,source->m13,
        source->m20,source->m21,source->m22,source->m23,
        source->m30,source->m31,source->m32,source->m33
    );
}
void PMatrix3D::preApply(
    float n00, float n01, float n02,
    float n10, float n11, float n12
){
    preApply(
        n00,n01,n02,0.f,
        n10,n11,n12,0.f,
        0.f,0.f,1.f,0.f,
        0.f,0.f,0.f,1.f
    );
}
void PMatrix3D::preApply(
    float n00, float n01, float n02, float n03,
    float n10, float n11, float n12, float n13,
    float n20, float n21, float n22, float n23,
    float n30, float n31, float n32, float n33
){
    float r00 = n00*m00 + n01*m10 + n02*m20 + n03*m30;
    float r01 = n00*m01 + n01*m11 + n02*m21 + n03*m31;
    float r02 = n00*m02 + n01*m12 + n02*m22 + n03*m32;
    float r03 = n00*m03 + n01*m13 + n02*m23 + n03*m33;

    float r10 = n10*m00 + n11*m10 + n12*m20 + n13*m30;
    float r11 = n10*m01 + n11*m11 + n12*m21 + n13*m31;
    float r12 = n10*m02 + n11*m12 + n12*m22 + n13*m32;
    float r13 = n10*m03 + n11*m13 + n12*m23 + n13*m33;

    float r20 = n20*m00 + n21*m10 + n22*m20 + n23*m30;
    float r21 = n20*m01 + n21*m11 + n22*m21 + n23*m31;
    float r22 = n20*m02 + n21*m12 + n22*m22 + n23*m32;
    float r23 = n20*m03 + n21*m13 + n22*m23 + n23*m33;

    float r30 = n30*m00 + n31*m10 + n32*m20 + n33*m30;
    float r31 = n30*m01 + n31*m11 + n32*m21 + n33*m31;
    float r32 = n30*m02 + n31*m12 + n32*m22 + n33*m32;
    float r33 = n30*m03 + n31*m13 + n32*m23 + n33*m33;

    m00 = r00; m01 = r01; m02 = r02; m03 = r03;
    m10 = r10; m11 = r11; m12 = r12; m13 = r13;
    m20 = r20; m21 = r21; m22 = r22; m23 = r23;
    m30 = r30; m31 = r31; m32 = r32; m33 = r33;
}
PVector* PMatrix3D::mult(PVector* source, PVector* target){
    if (target == null) {
        target = new PVector();
    }
    target->set(m00*source->x + m01*source->y + m02*source->z + m03,
                m10*source->x + m11*source->y + m12*source->z + m13,
                m20*source->x + m21*source->y + m22*source->z + m23);
    return target;
}
float* PMatrix3D::mult(float* source, float* target, int s){
    if (target == null) {
        if(s < 3){
            target = new float[3];
            s = 3;
        } else if (s > 4){
            target = new float[4];
            s = 4;
        }
    }
    if (target != null) {
        if (s == 3) {
            target[0] = m00*source[0] + m01*source[1] + m02*source[2] + m03;
            target[1] = m10*source[0] + m11*source[1] + m12*source[2] + m13;
            target[2] = m20*source[0] + m21*source[1] + m22*source[2] + m23;
        }
        if (s == 4) {
            target[0] = m00*source[0] + m01*source[1] + m02*source[2] + m03*source[3];
            target[1] = m10*source[0] + m11*source[1] + m12*source[2] + m13*source[3];
            target[2] = m20*source[0] + m21*source[1] + m22*source[2] + m23*source[3];
            target[3] = m30*source[0] + m31*source[1] + m32*source[2] + m33*source[3];
        }
    }
    return target;
}
void PMatrix3D::transpose(){
    float temp;
    temp = m01; m01 = m10; m10 = temp;
    temp = m02; m02 = m20; m20 = temp;
    temp = m03; m03 = m30; m30 = temp;
    temp = m12; m12 = m21; m21 = temp;
    temp = m13; m13 = m31; m31 = temp;
    temp = m23; m23 = m32; m32 = temp;
}
float determinant3x3(float t00, float t01, float t02,
                     float t10, float t11, float t12,
                     float t20, float t21, float t22) {
    return (t00 * (t11 * t22 - t12 * t21) +
            t01 * (t12 * t20 - t10 * t22) +
            t02 * (t10 * t21 - t11 * t20));
}
bool PMatrix3D::invert(){
    float d = determinant();
    if (d == 0) {
        return false;
    }

    // first row
    float t00 =  determinant3x3(m11, m12, m13, m21, m22, m23, m31, m32, m33);
    float t01 = -determinant3x3(m10, m12, m13, m20, m22, m23, m30, m32, m33);
    float t02 =  determinant3x3(m10, m11, m13, m20, m21, m23, m30, m31, m33);
    float t03 = -determinant3x3(m10, m11, m12, m20, m21, m22, m30, m31, m32);

    // second row
    float t10 = -determinant3x3(m01, m02, m03, m21, m22, m23, m31, m32, m33);
    float t11 =  determinant3x3(m00, m02, m03, m20, m22, m23, m30, m32, m33);
    float t12 = -determinant3x3(m00, m01, m03, m20, m21, m23, m30, m31, m33);
    float t13 =  determinant3x3(m00, m01, m02, m20, m21, m22, m30, m31, m32);

    // third row
    float t20 =  determinant3x3(m01, m02, m03, m11, m12, m13, m31, m32, m33);
    float t21 = -determinant3x3(m00, m02, m03, m10, m12, m13, m30, m32, m33);
    float t22 =  determinant3x3(m00, m01, m03, m10, m11, m13, m30, m31, m33);
    float t23 = -determinant3x3(m00, m01, m02, m10, m11, m12, m30, m31, m32);

    // fourth row
    float t30 = -determinant3x3(m01, m02, m03, m11, m12, m13, m21, m22, m23);
    float t31 =  determinant3x3(m00, m02, m03, m10, m12, m13, m20, m22, m23);
    float t32 = -determinant3x3(m00, m01, m03, m10, m11, m13, m20, m21, m23);
    float t33 =  determinant3x3(m00, m01, m02, m10, m11, m12, m20, m21, m22);

    // transpose and divide by the determinant
    m00 = t00 / d;
    m01 = t10 / d;
    m02 = t20 / d;
    m03 = t30 / d;

    m10 = t01 / d;
    m11 = t11 / d;
    m12 = t21 / d;
    m13 = t31 / d;

    m20 = t02 / d;
    m21 = t12 / d;
    m22 = t22 / d;
    m23 = t32 / d;

    m30 = t03 / d;
    m31 = t13 / d;
    m32 = t23 / d;
    m33 = t33 / d;

    return true;
}
float PMatrix3D::determinant(){
    float f =
        m00
        * ((m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32)
           - m13 * m22 * m31
           - m11 * m23 * m32
           - m12 * m21 * m33);
    f -= m01
         * ((m10 * m22 * m33 + m12 * m23 * m30 + m13 * m20 * m32)
            - m13 * m22 * m30
            - m10 * m23 * m32
            - m12 * m20 * m33);
    f += m02
         * ((m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31)
            - m13 * m21 * m30
            - m10 * m23 * m31
            - m11 * m20 * m33);
    f -= m03
         * ((m10 * m21 * m32 + m11 * m22 * m30 + m12 * m20 * m31)
            - m12 * m21 * m30
            - m10 * m22 * m31
            - m11 * m20 * m32);
    return f;
}
std::string PMatrix3D::toString(){
    std::stringstream ss;
    ss << "PMatrix3D(" << m00 << ","<< m01 << ","<< m02 << "," << m03 << "," << std::endl;
    ss << "          " << m10 << ","<< m11 << ","<< m12 << "," << m13 << "," << std::endl;
    ss << "          " << m20 << ","<< m21 << ","<< m22 << "," << m23 << "," << std::endl;
    ss << "          " << m30 << ","<< m31 << ","<< m32 << "," << m33 << ")" << std::endl;
    return ss.str();
}
PImage::PImage(){
    this->format = ARGB;
}
PImage::PImage(int width, int height) : PImage(width,height,ARGB){}
PImage::PImage(int width, int height, int format){
    assert(format == ALPHA || format == ARGB || format == RGB);
    this->format = format;
    this->width = width;
    this->height = height;
    this->pixels = new int[width * height];
    this->length = width * height;
}
PImage::PImage(int width, int height, int format, int* pixels, int pwidth, int pheight){
    assert(format == ALPHA || format == ARGB || format == RGB);
    this->format = format;
    this->width = width;
    this->height = height;
    this->pixels = new int[width * height];
    this->length = width * height;
}
void PImage::loadPixels(){
    int l = width * height;
    if (pixels == null || length != l) {
        pixels = new int[l];
        length = l;
    }
}
void PImage::updatePixels(){
    updatePixels(0,0,width,height);
}
void PImage::updatePixels(int x,int y,int w,int h){
    todo("updatePixels(int x,int y,int w,int h)");
}
unsigned char* transformImageARGB(int* colours,int w, int h, int* s){
    *s = w * h * 4;
    unsigned char* buff = new unsigned char[w * h * 4];
    for(int a = 0;a < w * h;a++){
        buff[a*4 + 0] = (unsigned char)(colours[a] >> 24) & 0xFF;
        buff[a*4 + 1] = (unsigned char)(colours[a] >> 16) & 0xFF;
        buff[a*4 + 2] = (unsigned char)(colours[a] >>  8) & 0xFF;
        buff[a*4 + 3] = (unsigned char)(colours[a]      ) & 0xFF;
    }
    return buff;
}
void PImage::resize(int w,int h){
    todo("in development");
}
std::string PImage::toString(){
    std::string f;
    switch(this->format){
    case ALPHA:
        f = "ALPHA";
        break;
    case RGB:
        f = "RGB";
        break;
    case ARGB:
        f = "ARGB";
        break;
    default:
        f = "unknown";
    }
    std::stringstream ss;
    ss << "PImage(" << width << "x" << height << ", format=" << f << ")\n";
    return ss.str();
}
PShape::PShape(){/* *** */}
bool PShape::isVisible(){
    return this->visibility;
}
void PShape::setVisible(bool isVisible){
    this->visibility = isVisible;
}
void PShape::beginShape(){
    if(!newShape){
        newShape = true;
    }else{
        error("beginShape() must be called once");
    }
}
void PShape::endShape(bool close){
    if(newShape){
        newShape = false;
    }else{
        error("endShape() must be called once after beginShape()");
    }
}
int PShape::getChildCount(){
    return childs.size();
}
PShape PShape::getChild(int index){
    if(index < 0 || index > getChildCount())
        error("PShape::getChild(int index) -> wrong index");
    return childs[index];
}
PShape PShape::getChild(const std::string& name){
    for(PShape& child : childs){
        if(child.name == name){
            return child;
        }
    }
    return PShape();
}
void PShape::addChild(PShape& child){
    this->childs.push_back(child);
}
std::vector<PVector> PShape::vertices(){
    return this->vertexes;
}
int PShape::getVertexCount(){
    return vertexes.size();
}
PVector PShape::getVertex(int index){
    if(index < 0 || index > getVertexCount()){
        error("PShape::getVertex(int index) -> wrong index");
        return PVector();
    }
    return vertexes[index];
}
PVector PShape::getVertex(int index, PVector& vec){
    vec = getVertex(index);
    return vec;
}
void PShape::setVertex(int index, float x,float y){
    setVertex(index,PVector(x,y));
}
void PShape::setVertex(int index, float x,float y,float z){
    setVertex(index,PVector(x,y,z));
}
void PShape::setVertex(int index, PVector vec){
    if(index < 0 || index > getVertexCount()){
        error("PShape::setVertex(int index) -> wrong index");
        return;
    }
    vertexes[index] = vec;
}
void PShape::vertex(PVector vec){
    vertexes.push_back(vec);
}
void PShape::vertex(float x, float y){
    vertexes.push_back(PVector(x,y));
}
void PShape::vertex(float x, float y,float z){
    vertexes.push_back(PVector(x,y,z));
}
void PShape::setFill(color c){
    if(!isFill) isFill = true;
    fillColor = c;
}
void PShape::setStroke(color c){
    if(!isStroke) isStroke = true;
    strokeColor = c;
}
void PShape::fill(color c){
    if(!isFill) isFill = true;
    fillColor = c;
}
void PShape::stroke(color c){
    if(!isStroke) isStroke = true;
    strokeColor = c;
}
void PShape::noFill(){
    isFill = false;
}
void PShape::noStroke(){
    isStroke = false;
}
void PShape::translate(float x,float y){
    mat.translate(x,y);
}
void PShape::translate(float x,float y, float z){
    mat.translate(x,y,z);
}
void PShape::rotateX(float angle){
    mat.rotateX(angle);
}
void PShape::rotateY(float angle){
    mat.rotateY(angle);
}
void PShape::rotateZ(float angle){
    mat.rotateZ(angle);
}
void PShape::rotate(float angle){
    mat.rotate(angle);
}
void PShape::scale(float s){
    mat.scale(s);
}
void PShape::scale(float x,float y){
    mat.scale(x,y);
}
void PShape::scale(float x,float y,float z){
    mat.scale(x,y,z);
}
void PShape::resetMatrix(){
    mat.reset();
}
std::string PShape::toString(){
    std::stringstream ss;
    ss << "PShape(" << getChildCount() << "," << getVertexCount() << ")";
    return ss.str();
}

// ================== main driver ===================

int main(){
    gl = new GLBackend();
    //if(&settings){
    //    settings();
    //}
    setup();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if(smoothness > 0){
        glfwWindowHint(GLFW_SAMPLES, smoothness);
    }
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window){
        error("Failed to create the GLFW window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        error("Failed to initialize GLAD\n");
        return -1;
    }
    gl->init();
    sh = new PShader();
    if(smoothness > 0)
        gl->enable(F_MSAA);
    ortho(0,width,height,0);
    gl->viewport(width, height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h){
        gl->viewport(w, h);
        ortho(0,w,h,0);
        width = w;
        height = h;
    });

    double tmpX,tmpY;

    while (!glfwWindowShouldClose(window)){
        pmouseX = mouseX;
        pmouseY = mouseY;
        glfwGetCursorPos(window,&tmpX,&tmpY);
        mouseX = (int)tmpX;
        mouseY = (int)tmpY;
        gl->clearColor(bgColor.r,bgColor.g,bgColor.b,bgColor.a);
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    _data.clear();
    glfwTerminate();
    delete sh;
    delete gl;
    return 0;
}
