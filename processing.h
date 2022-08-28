/**
 *  ==== CProcessing ====
 *  @version 1.4 beta 10
 *  @author maksmakuta
 */

#ifndef PROCESSING
#define PROCESSING

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ctime>
#include <optional>
#include <stack>
#include <cmath>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define DOWN    GLFW_KEY_DOWN
#define UP      GLFW_KEY_UP
#define LEFT    GLFW_KEY_LEFT
#define RIGHT   GLFW_KEY_RIGHT
#define ESC     GLFW_KEY_ESCAPE

#define CENTER  0
#define RADIUS  1
#define CORNER  2
#define CORNERS 3

#define PI          3.14159265358979323846
#define HALF_PI     PI/2.0
#define QUARTER_PI  PI/4.0
#define TWO_PI      PI*2.0
#define TAU         TWO_PI

#define miterMinAngle 0.349066f
#define roundMinAngle 0.174533f

#define MITER   0
#define BEVEL   1

#define SQUARE  0
#define PROJECT 1

#define ROUND   2
/**
 * JOIN:
 *  - MITTER
 *  - BEVEL
 *  - ROUND
 * CAP:
 *  - SQUARE
 *  - PROJECT
 *  - ROUND
 */

typedef bool boolean;                   // Java boolean type
#define null nullptr;                   // Java null pointer type
//#define length(x) (sizeof(x)/sizeof(x[0]))
//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

extern inline void setup();                // setup function
extern inline void draw();                 // draw  function
extern inline void mousePressed();         // mouse pressed function
extern inline void mouseReleased();        // mouse released function
extern inline void mouseMoved();           // mouse moved function
extern inline void mouseDragged();         // mouse dragged function
#ifdef USE_KEYS
extern inline void keyPressed();           // key pressed function
#else
extern inline void keyPressed(){}          // key pressed function
#endif
extern inline void keyReleased();          // key released function
// ==============================================
std::string windowT = "";
GLFWwindow* window = null;          // window pointer
std::vector<std::string> args;      // program arguments
double mouseX = 0;                  // Mouse x coordinate
double mouseY = 0;                  // Mouse y coordinate
double pmouseX = 0;                 // Previous mouse x coordinate
double pmouseY = 0;                 // Previous mouse y coordinate
bool mousepressed = false;          // Whether any mouse button is pressed
bool keypressed = false;            // Whether a key was pressed
int mouseButton = 0;                // Which button is pressed
unsigned char key = 0;              // Which (ASCII) key was pressed
int keyCode = 0;                    // Code for the last pressed key
int screenWidth = 0;                // window width
int screenHeight = 0;               // window height
unsigned config = 0;                // configuration flags
double framerate = 0;               // Frames per second
double framedelay = 0.0;            // Delay in seconds between frame
int frameCount = 0;                 // frames since start
//std::vector<Style> styles;        // Stack of styles
//PixelColorBuffer pixels;          // virtual array of pixels to get and put from (operated thru backbuffer)
//PImage screenBuffer;              // buffer of current window
bool looping = true;                // true makes display call itself
bool redrawflag = false;            // to draw next frame immediately
int initialized = false;            // glfw initialized yet
int width  = 128;                   // window width
int height = 128;                   // window height

// ==============================================

    template<class T> T rand(T min, T max){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distr(min, max);
        return static_cast<T>(distr(gen));
    }

    double randomGaussian(){
        std::default_random_engine gen;
        std::normal_distribution<double> distr;
        return distr(gen);
    }

    template<class T> T map(T val,T rmin,T rmax,T min,T max){
            T OldRange = rmax - rmin;
            T NewRange = max - min;
            return (((val - rmin) * NewRange) / OldRange) + min;
    }

    template<class T> T constrain(T val,T min,T max){
           if(val <= min) return min;
           else if(val >= max) return max;
           else return val;
    }

    template<class T> T norm(T val,T min,T max){
        return map<T>(val,min,max,0.f,1.f);
    }

    template<class T> T random(T x)       { return rand<T>(0,x); }
    template<class T> T random(T x,T y)   { return rand<T>(x,y); }

    template<class T> T dist(T x1,T y1,T x2,T y2){
        T x = (x2 - x1) * (x2 - x1);
        T y = (y2 - y1) * (y2 - y1);
        return sqrt(x + y);
    }

    template<class T> T dist(T x1,T y1,T z1,
                  T x2,T y2,T z2){
        T x = (x2 - x1) * (x2 - x1);
        T y = (y2 - y1) * (y2 - y1);
        T z = (z2 - z1) * (z2 - z1);
        return sqrt(x + y + z);
    }

    template<class T> T lerp(T start,T end, T amt){
        return start + (end-start) * amt;
    }

    template<class T> T mag(T x,T y){
        return dist<T>(0,0,x,y);
    }

    template<class T> T sq(T val){
        return val * val;
    }

    template<class T> T degrees(T val){
        return val * (180.0/PI);
    }
    template<class T> T radians(T val){
        return val * (PI/180.0);
    }


    static int charHex(char _c){
        switch(_c) {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'a': case 'A': return 10;
            case 'b': case 'B': return 11;
            case 'c': case 'C': return 12;
            case 'd': case 'D': return 13;
            case 'e': case 'E': return 14;
            case 'f': case 'F': return 15;
            default: return  -1;
        }
    }

template<class T> std::vector<T> append(const std::vector<T> arr,T obj){
    arr.push_back(obj);
    return arr;
}
template<class T> inline void arrayCopy(const std::vector<T>& src,int srcPosition,
                                 const std::vector<T>& dst,int dstPosition,int length)	{

    std::copy(src.begin()+srcPosition,src.begin()+length,dst.begin() + dstPosition);

}
template<class T> inline void arrayCopy( const std::vector<T>&  src,const std::vector<T>&  dst, int length){
    arrayCopy(src,0,dst,0,length);
}
template<class T> inline void arrayCopy(const std::vector<T>& src, const std::vector<T>&  dst){
    arrayCopy(src,dst,src.size());
}
template<class T> std::vector<T> concat(const std::vector<T>& src, const std::vector<T>&  dst){
    src.insert(src.end(),dst.begin(),dst.end());
    return src;
}
template<class T> std::vector<T> expand(const std::vector<T>& list,int newSize = -1){
    int s = (int)(newSize == -1 ? list.size() * 2 : newSize);
    std::vector<T> tmp(s);
    std::copy(list.begin(),list.end(),tmp.begin());
    return tmp;
}
template<class T> std::vector<T> reverse(const std::vector<T>& list){
    std::vector<T> tmp(list.size());
    std::copy(list.end(),list.begin(),tmp.begin());
    return tmp;
}
template<class T> std::vector<T> shorten(const std::vector<T>& list){
    std::vector<T> tmp(list.size() - 1);
    std::copy(list.begin(),list.end()-1,tmp.begin());
    return tmp;
}
template<class T> std::vector<T> sort(const std::vector<T>& list,int count = 0){
    qsort(list,count == 0 ? list.size() : count,sizeof(T),
          [](const void * a, const void * b){
            return ( *(T*)a - *(T*)b );
          }
    );
    return list;
}
template<class T> std::vector<T> splice(const std::vector<T>& list,const std::vector<T>& val,int index){
    list.insert(list.begin() + index,val.begin(),val.end());
    return list;
}
template<class T> std::vector<T> splice(const std::vector<T>& list,T val,int index){
    list.insert(list.begin() + index,val);
    return list;
}
template<class T> std::vector<T> subset(const std::vector<T>& list,int start,int count = 0){
    return std::vector<T>(list.begin() + start,count == 0 ? list.end() : list.begin()+ start + count);
}

tm getTime(){
   time_t tt;
   time( &tt );
   return (tm)*localtime( &tt );
}

// Returns the current day as a value from 1 - 31
int day(){
    return getTime().tm_mday;
}

// Returns the current hour as a value from 0 - 23
int hour(){
    return getTime().tm_hour;
}

// Returns the current minute as a value from 0 - 59
int minute(){
    return getTime().tm_min;
}

// Returns the current month as a value from 1 - 12
int month(){
    return getTime().tm_mon;
}

// Returns the current second as a value from 0 - 59
int second(){
    return getTime().tm_sec;
}

// Returns the current year as an integer (2003, 2004, 2005, etc)
int year(){
    return getTime().tm_year + 1900;
}

// Returns the number of milliseconds (thousandths of a second) since starting an applet
int millis(){
    return (int)(glfwGetTime() * 1000);
}

template<typename T> class Manager{
private:
    std::vector<T> images;
public:

    inline void add(T img){
        images.push_back(img);
    }

    T get(int id){
        return images[id];
    }

    T getLast(){
        return get(images.size() - 1);
    }

    inline void load(){
        for(T &img : images){
            img.load();
        }
    }

    inline void done(){
        images.clear();
    }
};

class color{
public:
    color() : color(0){/* ... */}
    color(int a) : color(a,a,a){/* ... */}
    color(int _r,int _g,int _b) : color(_r,_g,_b,1){/* ... */}
    color(int _r,int _g,int _b,int _a){
        this->r = (float)_r / 255.f;
        this->g = (float)_g / 255.f;
        this->b = (float)_b / 255.f;
        this->a = (float)_a / 255.f;
    }

    color(const std::string &h){
      int data[4];

      for(int & i : data)
          i = 255;

      switch (h.size() - 1) {
          case 3: // #abc | #rgb
              for(int i = 0;i < 3;i++){
                  data[i] = charHex(h[i+1]) * 16 + 15;
              }
              break;
          case 4: // #abcd | #rgba
              for(int i = 0;i < 4;i++){
                  data[i] = charHex( h[i+1]) * 16 + 15;
              }
              break;
          case 6: // #123456 | #rrggbb
              for(int i = 0;i < 3;i++){
                  data[i] = charHex(h[2*i+1]) * 16 + charHex(h[2*i+2]);
              }
              break;
          case 8: // #12345678 | #rrggbbaa
              for(int i = 0;i < 4;i++){
                  data[i] = charHex(h[2*i+1]) * 16 + charHex(h[2*i+2]);
              }
              break;
        }

        this->r = (float)data[0] / 255.f;
        this->g = (float)data[1] / 255.f;
        this->b = (float)data[2] / 255.f;
        this->a = (float)data[3] / 255.f;
    }


    color operator = (const std::string &shex){
        return color(shex.c_str());
    }

    std::vector<float> vector(){
        std::vector<float> v;
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        v.push_back(a);
        return v;
    }

    float r,g,b,a;
};

class PVector{
public:
    PVector() : PVector(0,0,0){/* ... */}

    PVector(float _x,float _y,float _z){
        this->x = _x;
        this->y = _y;
        this->z = _z;
    }

    PVector(float _x,float _y){
        this->x = _x;
        this->y = _y;
        this->z = 0.f;
    }

    PVector set(float _x,float _y){
        this->x = _x;
        this->y = _y;
        this->z = 0.f;
        return *this;
    }

    PVector set(float _x,float _y,float _z){
        this->x = _x;
        this->y = _y;
        this->z = _z;
        return *this;
    }

    PVector set(PVector obj){
        this->x = obj.x;
        this->y = obj.y;
        this->z = obj.z;
        return *this;
    }

    PVector set(float* obj){
        this->x = obj[0];
        this->y = obj[1];
        this->z = obj[2] ? obj[2] : 0.f;
        return *this;
    }

    PVector fromAngle(float angle){
        return PVector(sin(angle),cos(angle));
    }

    PVector copy(){
        return *this;
    }

    float* get(float* arr,int s){
        if(s == 3)
        arr[0] = x;
        arr[1] = y;
        arr[2] = s == 3 ? x : 0.f;
        return arr;
    }

    float mag(){
        return sqrt(x * x + y * y + z * z);
    }

    float magSq() {
       return (x*x + y*y + z*z);
    }

    PVector add(PVector p){
        this->x += p.x;
        this->y += p.y;
        this->z += p.z;
        return *this;
    }

    PVector add(float _x,float _y){
        this->x += _x;
        this->y += _y;
        return *this;
    }

    PVector add(float _x,float _y,float _z){
        this->x += _x;
        this->y += _y;
        this->z += _z;
        return *this;
    }

    static PVector add(PVector a, PVector b){
        return a.add(b);
    }


    PVector sub(PVector p){
        this->x -= p.x;
        this->y -= p.y;
        this->z -= p.z;
        return *this;
    }

    PVector sub(float _x,float _y){
        this->x -= _x;
        this->y -= _y;
        return *this;
    }

    PVector sub(float _x,float _y,float _z){
        this->x -= _x;
        this->y -= _y;
        this->z -= _z;
        return *this;
    }

    static PVector sub(PVector a, PVector b){
        return a.sub(b);
    }

    PVector mult(float s){
        this->x *= s;
        this->y *= s;
        this->z *= s;
        return *this;
    }

    static PVector mult(PVector a, float s){
        return a.mult(s);
    }

    PVector div(float s){
        this->x /= s;
        this->y /= s;
        this->z /= s;
        return *this;
    }

    static PVector div(PVector a, float s){
        return a.div(s);
    }

    float dist(PVector a,PVector b){
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float dz = a.z - b.z;
        return (float) sqrt(dx*dx + dy*dy + dz*dz);
    }

    float dist(PVector p){
        return dist(*this,p);
    }

    float dot(float _x,float _y,float _z){
        return x * _x +y * _y +z * _z;
    }

    float dot(PVector p){
        return dot(*this,p);
    }

    float dot(PVector a,PVector b){
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static PVector cross(PVector a,PVector b){
        float crossX = a.y * b.z - b.y * a.z;
        float crossY = a.z * b.x - b.z * a.x;
        float crossZ = a.x * b.y - b.x * a.y;
        return PVector(crossX, crossY, crossZ);
    }

    PVector cross(PVector a){
        return cross(*this,a);
    }

    PVector normalize() {
        float m = mag();
        if (m != 0 && m != 1) {
          div(m);
        }
        return *this;
    }

    PVector limit(float max) {
        if (magSq() > max*max) {
          normalize();
          mult(max);
        }
        return *this;
    }

    PVector setMag(float len) {
        normalize();
        mult(len);
        return *this;
    }

    float heading() {
       return atan2(y, x);
    }

    PVector setHeading(float angle) {
        float m = mag();
        x = (float) (m * cos(angle));
        y = (float) (m * sin(angle));
        return *this;
      }

    PVector rotate(float theta) {
       float temp = x;
       x = x * cos(theta) - y * sin(theta);
       y = temp * sin(theta) + y * cos(theta);
       return *this;
     }

    //static PVector lerp(PVector v1,PVector v2, float amt) {
    //    return v1.lerp(v2,amt);
    //}

    //PVector lerp(PVector v, float amt) {
    //    this->x = lerp<float>((float)this->x,(float)v.x,amt);
    //    this->y = lerp<float>((float)this->y,(float)v.y,amt);
    //    this->z = lerp<float>((float)this->z,(float)v.z,amt);
    //    return *this;
    //}

    static float angleBetween(PVector v1, PVector v2) {
        if (v1.x == 0 && v1.y == 0 && v1.z == 0 ) return 0.0f;
        if (v2.x == 0 && v2.y == 0 && v2.z == 0 ) return 0.0f;

        double dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        double v1mag = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
        double v2mag = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
        double amt = dot / (v1mag * v2mag);
        if (amt <= -1) {
            return PI;
        } else if (amt >= 1) {
            return 0;
        }
        return (float) acos(amt);
      }

    float* array(){
      float *array = new float[3];
      array[0] = x;
      array[1] = y;
      array[2] = z;
      return array;
    }

    static PVector random2D(){
        PVector p(random<float>(-1,1),random<float>(-1,1));
        return p.normalize();
    }

    static bool equal(const PVector& a,const PVector& b){
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    float x,y,z;
};

PVector createVector(float x,float y,float z){
    return PVector(x,y,z);
}

PVector createVector(float x,float y){
    return createVector(x,y,0);
}


enum SHAPE_TYPE{
    POINTS          = GL_POINTS         ,
    LINES           = GL_LINES          ,
    LINE_LOOP       = GL_LINE_LOOP      ,
    TRIANGLES       = GL_TRIANGLES      ,
    TRIANGLE_FAN    = GL_TRIANGLE_FAN   ,
    TRIANGLE_STRIP  = GL_TRIANGLE_STRIP ,
    QUADS           = GL_QUADS          ,
    QUAD_STRIP      = GL_QUAD_STRIP     ,
    DEFAULT         = GL_POLYGON
};

struct vertex{
    float x,y,z;
    float s,t;
};

class PShape{
private:
    SHAPE_TYPE stype;
    color f,s;
    bool isFill;
    std::vector<vertex> verData;
public:
    PShape() : PShape(DEFAULT){ }

    PShape(SHAPE_TYPE st){
        this->stype = st;
    }

    inline void push(float a,float b,float c){
        verData.push_back({a,b,c,0.f,0.f});
    }

    inline void push(float x,float y,float a,float b){
        verData.push_back({x,y,0,a,b});
    }

    inline void setColorF(const color& _c){
        this->isFill = true;
        this->f = _c;
    }

    color getColorF() const {
        return this->f;
    }

    inline void setColorS(const color& _c){
        this->isFill = false;
        this->s = _c;
    }

    color getColorS() const {
        return this->s;
    }

    bool fill() const{
        return isFill;
    }

    inline void push(const PVector& v){
        push(v.x,v.y,v.z);
    }

    inline void add(const PShape& p){
        std::vector<vertex> tmp = p.data();
        if(!tmp.empty()){
            verData.insert(verData.end(),tmp.begin(),tmp.end());
        }
    }

    vertex at(int p){
        if(p >= 0 && p < this->size())
            return verData[p];
        else{
            std::cerr << "Undefined position\n";
            return {0,0,0,0,0};
        }
    }

    int size() const{
        return verData.size();
    }

    SHAPE_TYPE type() const{
        return stype;
    }

    inline void type(SHAPE_TYPE s){
        stype = s;
    }

    std::vector<vertex> data() const{
        return verData;
    }

    inline void data(const std::vector<vertex>& d) {
        this->verData = d;
    }

    bool loop(){
        auto a = createVector(verData[0].x,verData[0].y,verData[0].z);
        auto b = createVector(  verData[verData.size() - 1].x,
                                verData[verData.size() - 1].y,
                                verData[verData.size() - 1].z
                );
        return PVector::equal(a,b);
    }

    inline void done(){
        verData.clear();
    }
};

struct LineSegment {
    LineSegment(const PVector &a, const PVector &b) : a(a), b(b) {}
    PVector a, b;

    LineSegment operator+(const PVector &toAdd) const {
        return {PVector::add(a, toAdd), PVector::add(b, toAdd)};
    }
    LineSegment operator-(const PVector &toRemove) const {
        return {PVector::sub(a, toRemove), PVector::sub(b, toRemove)};
    }
    PVector normal() const {
        auto dir = direction();
        return {-dir.y, dir.x};
    }
    PVector direction(bool normalized = true) const {
        auto vec = PVector::sub(b, a);
        return normalized ? vec.normalize() : vec;
    }

    static std::optional<PVector> intersection(const LineSegment &a, const LineSegment &b, bool infiniteLines) {
        // calculate un-normalized direction vectors
        auto r = a.direction(false);
        auto s = b.direction(false);

        auto originDist = PVector::sub(b.a, a.a);

        auto uNumerator = originDist.cross(r);
        auto denominator = r.cross(s);

        if (std::abs(denominator.z) < 0.0001f) {
            // The lines are parallel
            return std::nullopt;
        }

        // solve the intersection positions
        auto u = uNumerator.z / denominator.z;
        auto t = PVector::cross(originDist, s).z / denominator.z;

        if (!infiniteLines && (t < 0 || t > 1 || u < 0 || u > 1)) {
            // the intersection lies outside of the line segments
            return std::nullopt;
        }

        // calculate the intersection point
        // a.a + r * t;
        return PVector::add(a.a, PVector::mult(r, t));
    }
};

struct PolySegment {
    PolySegment(const LineSegment &center, float thickness) :
            center(center),
            edge1(center + PVector::mult(center.normal(), thickness)),
            edge2(center - PVector::mult(center.normal(), thickness)) {}
    LineSegment center, edge1, edge2;
};
PShape createTriangleFan(PVector connectTo, PVector origin,
                         PVector start, PVector end, bool clockwise) {

    PShape vertices;
        auto point1 = PVector::sub(start, origin);
        auto point2 = PVector::sub(end, origin);

        // calculate the angle between the two points
        auto angle1 = atan2(point1.y, point1.x);
        auto angle2 = atan2(point2.y, point2.x);

        // ensure the outer angle is calculated
        if (clockwise) {
            if (angle2 > angle1) {
                angle2 = angle2 - 2 * PI;
            }
        } else {
            if (angle1 > angle2) {
                angle1 = angle1 - 2 * PI;
            }
        }

        auto jointAngle = angle2 - angle1;

        // calculate the amount of triangles to use for the joint
        auto numTriangles = std::max(1, (int) std::floor(std::abs(jointAngle) / roundMinAngle));

        // calculate the angle of each triangle
        auto triAngle = jointAngle / numTriangles;

        PVector startPoint = start;
        PVector endPoint;
        for (int t = 0; t < numTriangles; t++) {
            if (t + 1 == numTriangles) {
                // it's the last triangle - ensure it perfectly
                // connects to the next line
                endPoint = end;
            } else {
                auto rot = (t + 1) * triAngle;

                // rotate the original point around the origin
                endPoint.x = std::cos(rot) * point1.x - std::sin(rot) * point1.y;
                endPoint.y = std::sin(rot) * point1.x + std::cos(rot) * point1.y;

                // re-add the rotation origin to the target point
                endPoint = PVector::add(endPoint, origin);
            }

            // emit the triangle
            vertices.push(startPoint);
            vertices.push(endPoint  );
            vertices.push(connectTo );

            startPoint = endPoint;
        }

        //std::cout << "Size -> "<< vertices.size() << "\n";
        return vertices;
    }


PShape createJoint( const PolySegment &segment1, const PolySegment &segment2,
                    int jointStyle, PVector &end1, PVector &end2,
                    PVector &nextStart1, PVector &nextStart2,
                    bool allowOverlap) {

    PShape vertices;
        // calculate the angle between the two line segments
        auto dir1 = segment1.center.direction();
        auto dir2 = segment2.center.direction();

        auto angle = PVector::angleBetween(dir1, dir2);

        // wrap the angle around the 180° mark if it exceeds 90°
        // for minimum angle detection
        auto wrappedAngle = angle;
        if (wrappedAngle > PI / 2) {
            wrappedAngle = PI - wrappedAngle;
        }

        if (jointStyle == MITER && wrappedAngle < miterMinAngle) {
            jointStyle =  BEVEL;
        }

        if (jointStyle == MITER) {
            // calculate each edge's intersection point
            // with the next segment's central line
            auto sec1 = LineSegment::intersection(segment1.edge1, segment2.edge1, true);
            auto sec2 = LineSegment::intersection(segment1.edge2, segment2.edge2, true);

            end1 = sec1 ? *sec1 : segment1.edge1.b;
            end2 = sec2 ? *sec2 : segment1.edge2.b;

            nextStart1 = end1;
            nextStart2 = end2;

        } else {
            // joint style is either BEVEL or ROUND

            // find out which are the inner edges for this joint
            auto x1 = dir1.x;
            auto x2 = dir2.x;
            auto y1 = dir1.y;
            auto y2 = dir2.y;

            auto clockwise = x1 * y2 - x2 * y1 < 0;

            const LineSegment *inner1, *inner2, *outer1, *outer2;

            // as the normal vector is rotated counter-clockwise,
            // the first edge lies to the left
            // from the central line's perspective,
            // and the second one to the right.
            if (clockwise) {
                outer1 = &segment1.edge1;
                outer2 = &segment2.edge1;
                inner1 = &segment1.edge2;
                inner2 = &segment2.edge2;
            } else {
                outer1 = &segment1.edge2;
                outer2 = &segment2.edge2;
                inner1 = &segment1.edge1;
                inner2 = &segment2.edge1;
            }

            // calculate the intersection point of the inner edges
            auto innerSecOpt = LineSegment::intersection(*inner1, *inner2, allowOverlap);

            auto innerSec = innerSecOpt
                            ? *innerSecOpt
                            // for parallel lines, simply connect them directly
                            : inner1->b;

            // if there's no inner intersection, flip
            // the next start position for near-180° turns
            PVector innerStart;
            if (innerSecOpt) {
                innerStart = innerSec;
            } else if (angle > PI / 2) {
                innerStart = outer1->b;
            } else {
                innerStart = inner1->b;
            }

            if (clockwise) {
                end1 = outer1->b;
                end2 = innerSec;

                nextStart1 = outer2->a;
                nextStart2 = innerStart;

            } else {
                end1 = innerSec;
                end2 = outer1->b;

                nextStart1 = innerStart;
                nextStart2 = outer2->a;
            }

            // connect the intersection points according to the joint style

            if (jointStyle == BEVEL) {
                // simply connect the intersection points
                vertices.push(outer1->b);
                vertices.push(outer2->a);
                vertices.push(innerSec);

            } else if (jointStyle == ROUND) {
                // draw a circle between the ends of the outer edges,
                // centered at the actual point
                // with half the line thickness as the radius
                vertices.add(createTriangleFan(innerSec, segment1.center.b, outer1->b, outer2->a, clockwise));
            }
        }

        return vertices;
    }

PShape strokify(PShape contour, float w, int cap, int join,bool allowOverlap = false){
    PShape data(TRIANGLES);
    data.setColorS(contour.getColorS());
    bool loop = contour.loop();
    // operate on half the thickness to make our lives easier
    w /= 2;

    // create poly segments from the points
    std::vector<PolySegment> segments;
    for (size_t i = 0; i + 1 < contour.size(); i++) {
        auto point1 = contour.at(i);
        auto point2 = contour.at(i + 1);

        auto p1 = createVector(point1.x,
                               point1.y,
                               point1.z);
        auto p2 = createVector(point2.x,
                               point2.y,
                               point2.z);

        // to ainline void division-by-zero errors,
        // only create a line segment for non-identical points
        if (!PVector::equal(p1, p2)) {
            segments.emplace_back(LineSegment(p1, p2), w);
        }
    }

    if (segments.empty()) {
        // handle the case of insufficient input points
        return contour;
    }

    PVector nextStart1;
    PVector nextStart2;
    PVector start1;
    PVector start2;
    PVector end1;
    PVector end2;

    // calculate the path's global start and end points
    auto &firstSegment = segments[0];
    auto &lastSegment = segments[segments.size() - 1];

    auto pathStart1 = firstSegment.edge1.a;
    auto pathStart2 = firstSegment.edge2.a;
    auto pathEnd1 = lastSegment.edge1.b;
    auto pathEnd2 = lastSegment.edge2.b;

    if (loop) {
            data.add(createJoint(lastSegment, firstSegment, join,
                        pathEnd1, pathEnd2, pathStart1, pathStart2, allowOverlap));
    }else {
        if (cap == SQUARE) {
            pathStart1 = PVector::sub(pathStart1, PVector::mult(firstSegment.edge1.direction(), w));
            pathStart2 = PVector::sub(pathStart2, PVector::mult(firstSegment.edge2.direction(), w));
            pathEnd1   = PVector::add(pathEnd1,   PVector::mult(lastSegment.edge1.direction(),  w));
            pathEnd2   = PVector::add(pathEnd2,   PVector::mult(lastSegment.edge2.direction(),  w));

        } else if (cap == ROUND) {
            data.add(createTriangleFan(firstSegment.center.a, firstSegment.center.a,
                              firstSegment.edge1.a, firstSegment.edge2.a, false));
            data.add(createTriangleFan(lastSegment.center.b, lastSegment.center.b,
                              lastSegment.edge1.b, lastSegment.edge2.b, true));
        }
    }

    // generate mesh data for path segments
    for (size_t i = 0; i < segments.size(); i++) {
        auto &segment = segments[i];

        // calculate start
        if (i == 0) {
            // this is the first segment
            start1 = pathStart1;
            start2 = pathStart2;
        }

        if (i + 1 == segments.size()) {
            // this is the last segment
            end1 = pathEnd1;
            end2 = pathEnd2;

        } else {
          data.add(createJoint(segment, segments[i + 1], join,
                      end1, end2, nextStart1, nextStart2, allowOverlap));
        }

        // emit vertices
       data.push(start1);
       data.push(start2);
       data.push(end1  );

       data.push(end1  );
       data.push(start2);
       data.push(end2  );

        start1 = nextStart1;
        start2 = nextStart2;
    }

    return data;
}
class PShader{
private:
    GLuint programS;
    GLuint vertS,fragS;
public:
    PShader(){}

    inline void loadFile(const std::string& vertex,const std::string& fragment){
        this->loadCode(load(vertex),load(fragment));
    }

    GLuint programID() const {return this->programS;}

    GLuint loc(const std::string& name){
        return glGetUniformLocation(programID(), name.c_str());
    }
    GLuint attrLoc(const std::string& name){
        return glGetAttribLocation(programID(),name.c_str());
    }
    inline void umat4(const std::string& name,const glm::mat4& matrix){
        glUniformMatrix4fv(loc(name),1, GL_FALSE, glm::value_ptr(matrix));
    }
    inline void uint(const std::string& name,int value){
        glUniform1i(loc(name), value);
    }
    inline void uvec4(const std::string& name,const glm::vec4& vec){
        glUniform4fv(loc(name),1,glm::value_ptr(vec));
    }
    inline void uvec3(const std::string& name,const glm::vec3& vec){
        glUniform3fv(loc(name),1,glm::value_ptr(vec));
    }
    inline void uvec2(const std::string& name,const glm::vec2& vec){
        glUniform2fv(loc(name),1,glm::value_ptr(vec));
    }

    inline void loadCode(const std::string& vertex,const std::string& fragment){
        this->vertS = makeShader(GL_VERTEX_SHADER  ,  vertex.c_str());
        this->fragS = makeShader(GL_FRAGMENT_SHADER,fragment.c_str());
        this->programS = makeProgram(vertS,fragS);
    }

    static PShader files(const std::string& vertex,const std::string& fragment){
        PShader s;
        s.loadFile(vertex,fragment);
        return s;
    }

    static PShader code(const std::string& vertex,const std::string& fragment){
        PShader s;
        s.loadCode(vertex,fragment);
        return s;
    }

    inline void done(){
        glDeleteShader(fragS);
        glDeleteShader(vertS);
        glDeleteProgram(programS);
    }

    static PShader P5(){
        static const std::string frag =
                "#version 330 core\n"
                "uniform int call;\n"
                "uniform sampler2D texID;\n"
                "uniform vec4 color1;\n"
                "uniform vec4 color2;\n"
                "in vec2 fTex;\n"
                "out vec4 pColor;\n"
                "void main(void){\n"
                "   if(call == 0) {\n"          // color
                "       pColor = color1;\n"
                "   } else if(call == 1) {\n"   // texture
                "       pColor = texture(texID,fTex);\n"
                "   } else if(call == 2) {\n"   // text
                "       pColor = vec4(color1.r,color1.g,color1.b,texture(texID, fTex).r);"
                "   } else if(call == 3) {\n"   // linear gradient h
                "       pColor = mix(color1,color2,smoothstep(0.0, 1.0, fTex.x));"
                "   } else if(call == 4) {\n"   // linear gradient v
                "       pColor = mix(color1,color2,smoothstep(0.0, 1.0, fTex.y));"
                "   } else if(call == 5) {\n"   // radial gradient
                "       float mixValue = length(fTex-vec2(0.5,0.5));"
                "       pColor = mix(color1,color2,mixValue);"
                "   } else {"
                "       pColor = vec4(0.1,0.0,0.0,1.0);\n"
                "   }\n"
                "}\n";
        static const std::string vert =
                "#version 330 core\n"
                "layout (location = 0) in vec3 ver;\n"
                "layout (location = 1) in vec2 tex;\n"
                "uniform mat4 matrix;\n"
                "out vec2 fTex;\n"
                "void main(void){\n"
                "   fTex = tex;\n"
                "   gl_Position = matrix * vec4(ver,1.0f);\n"
                "}\n";
        return PShader::code(vert,frag);
    }

private:
    bool isShaderOK(GLuint shader){
        int  success; char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "Shader ERROR: \n" << infoLog << std::endl;
        }
        return success;
    }

    bool isProgramOK(GLuint program){
        int  success; char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "Program ERROR: \n" << infoLog << std::endl;
        }
        return success;
    }

    GLuint makeShader(int type,const char* code){
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &code, NULL);
        glCompileShader(shader);
        return isShaderOK(shader) ? shader : 0;
    }

    GLuint makeProgram(GLuint vert,GLuint frag){
        GLuint program = glCreateProgram();
        glAttachShader(program, vert);
        glAttachShader(program, frag);
        glLinkProgram(program);
        return isProgramOK(program) ? program : 0;
    }

    std::string load(const std::string& fname){
        std::stringstream ss;
        std::ifstream f(fname);
        if(f.is_open()){
            ss << f.rdbuf();
            f.close();
        }
        return ss.str();
    }
};
class PImage{
private:
    unsigned int textureID = 0;
    std::string fname;
    bool loaded = false;
    int w,h,c;
public:
    PImage(){}
    PImage(const std::string& name){
        this->fname = name;
    }

    PImage(unsigned int tex):PImage("lol"){
        this->textureID = tex;
        this->loaded = true;
    }

    inline void load(){
        if(loaded != true){
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //stbi_set_flip_vertically_on_load(true);
            auto data = stbi_load(fname.c_str(), &w, &h, &c, 0);
            if (data){
                printf("ID -> %i\nc -> %i\n",textureID,c);
                float img[] = { GL_RED,GL_RG,GL_RGB,GL_RGBA };
                glTexImage2D(GL_TEXTURE_2D, 0, img[c-1], w, h, 0, img[c-1], GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                this->loaded = true;
            }else{
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
        }
    }

    inline void create(int w, int h, unsigned char* data,int img = GL_ALPHA){
        if(loaded != true){
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
           if (data){
                printf("ID -> %i\n",textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, img, w, h, 0, img, GL_UNSIGNED_BYTE, data);
                //glGenerateMipmap(GL_TEXTURE_2D);
                this->loaded = true;
            }else{
                std::cout << "Failed to load texture" << std::endl;
            }
        }
    }

    bool isLoaded() const{
        return this->loaded;
    }

    std::string getName() const{
        return this->fname;
    }

    unsigned int getID() const{
        return this->textureID;
    }

    int getWidth() const{
        return this->w;
    }

    int getHeight() const{
        return this->h;
    }

    inline void done(){
        glDeleteTextures(1,&textureID);
    }

    ~PImage(){
        done();
    }
};

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class PFont{
private:
    bool loaded = false;
public:
    PFont() : PFont("/usr/share/fonts/TTF/OpenSans-Regular.ttf"){/* ... */}

    PFont(std::string fname){
        this->fontName = fname;
    }

    inline void init(float s){
        if(!loaded){
            FT_Library ft;
            if (FT_Init_FreeType(&ft)){
                std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
                return;
            }

	        if (fontName.empty()){
                std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
                return;
            }else{
                std::cout << fontName << std::endl;
            }

	        // load font as face
            FT_Face face;
            if (FT_New_Face(ft, fontName.c_str(), 0, &face)) {
                std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
                return;
            }else{
                // set size to load glyphs as
                FT_Set_Pixel_Sizes(face, 0, s);

                // disable byte-alignment restriction
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                // load first 128 characters of ASCII set
                for (unsigned int c = 0; c < 128; c++){
                    // Load character glyph 
                    if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
                        std::cout << "ERROR::FREETYTPE: Failed to load Glyph (" << c << ")" << std::endl;
                        continue;
                    }
                    // generate texture
                    unsigned int texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                    );
                    // set texture options
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    // now store character for later use
                    Character character = {
                        texture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        static_cast<unsigned int>(face->glyph->advance.x)
                    };
                    Characters.insert(std::pair<char, Character>(c, character));
                }
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            // destroy FreeType once we're finished
            loaded = true;
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }
    }

    bool isLoaded() const {
        return this->loaded;
    }

    std::string fontName;
    std::map<GLchar, Character> Characters;
};
// ==============================================

Manager<PImage> *tex;

color fillColor,strokeColor,bg;
bool fillFlag,strokeFlag,builder;
PShape tmp;
PFont curr;
std::stack<glm::mat4> matrices;
glm::mat4 matrix;
float strokeWidth,defSize;
int cap,join,rMode,eMode,call = 0;
unsigned int textureID;
PShader sh;

// ==============================================


inline glm::vec4 vec(const color& c){
    return {c.r,c.g,c.b,c.a};
}

inline void initGL(){
    fillColor = color(255);
    strokeColor  = color(255);
    bg  = color(150);
    fillFlag = false;
    strokeFlag = false;
    builder = false;
    strokeWidth = 1.0f;
    cap = 0;join = 0;
    sh = PShader::P5();
    rMode = 0;
    eMode = 0;
    curr = PFont();
}

inline void doneGL(){
    while (!matrices.empty()) {
          matrices.pop();
    }
    sh.done();
}

inline void draw(const PShape& s){/*
    printf("call = %i\n",call);
    printf("texID = %i\n",textureID);*/
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * s.size(), s.data().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureID);
    glUseProgram(sh.programID());
    sh.umat4("matrix",matrix);
    sh.uvec4("color1",s.fill() ? vec(s.getColorF()) : vec(s.getColorS()));
    sh.uvec4("color2",s.fill() ? vec(strokeColor) : vec(fillColor));
    sh.uint ("call",call);
    glDrawArrays(s.type(), 0, s.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
    if(call != 0)
        call = 0;
    if(textureID != 0)
        textureID = 0;
    std::flush(std::cout);
}

inline void resetMatrix(){
    matrix = glm::mat4(1.0f);
}

inline void pushMatrix(){
    matrices.push(matrix);
}

inline void popMatrix(){
    if(matrices.size() > 0){
        matrix = matrices.top();
        matrices.pop();
    }else{
        matrix = glm::ortho(0,width,height,0,-1,1);
    }
}

inline void translate(float x,float y,float z = 0.0f){
    matrix = glm::translate(matrix,glm::vec3{x,y,z});
}

inline void rotate(float a){
    matrix = glm::rotate(matrix,a,glm::vec3{0.f,0.f,1.f});
}

inline void background(color c){
    bg = c;
}
inline void background(int r,int g,int b,int a){
    bg = color(r,g,b,a);
}
inline void background(int r,int g,int b){
    background(r,g,b,255);
}
inline void background(int v,int a){
    background(v,v,v,a);
}
inline void background(int v){
    background(v,255);
}
inline void noFill(){
    fillFlag = false;
    fillColor = color(0);
}
inline void fill(color c){
    fillFlag = true;
    fillColor = c;
}
inline void fill(int r,int g,int b,int a){
    fillFlag = true;
    fillColor = color(r,g,b,a);
}
inline void fill(int r,int g,int b){
    fill(r,g,b,255);
}
inline void fill(int v,int a){
    fill(v,v,v,a);
}
inline void fill(int v){
    fill(v,255);
}
inline void noStroke(){
    strokeFlag = false;
    strokeColor = color(0);
}
inline void stroke(color c){
    strokeFlag = true;
    strokeColor = c;
}
inline void stroke(int r,int g,int b,int a){
    strokeFlag = true;
    strokeColor = color(r,g,b,a);
}
inline void stroke(int r,int g,int b){
    stroke(r,g,b,255);
}
inline void stroke(int v,int a){
    stroke(v,v,v,a);
}
inline void stroke(int v){
    stroke(v,255);
}

inline void strokeWeight(float w){
    strokeWidth = w;
}

inline void strokeJoin(int j){
    join = j;
}

inline void strokeCap(int c){
    cap = c;
}

inline void begin(float w,float h){
    glViewport(0,0,w,h);
    matrix = glm::ortho(0.f,w,h,0.f,-1.f,1.f);
}

inline void end(){ /* ... */ }

inline void beginShape(SHAPE_TYPE t = DEFAULT){
    builder = true;
    tmp = PShape(t);
}

inline void vertex(float x,float y,float z = 0.0f){
    if(builder)
        tmp.push(x,y,z);
    else
        std::cerr << "vertex() must be inside shapeBegin() and shapeEnd() functions\n";
}

inline void endShape(){
    builder = false;
    if(fillFlag)
        tmp.setColorF(fillColor);
    if(strokeFlag)
        tmp.setColorS(strokeColor);
    draw(tmp);
}

inline void arc(float x,float y,float w,float h,float b,float e,float step){
    float d = radians(step);
    if(fillFlag){
        tmp = PShape(TRIANGLE_FAN);
        tmp.setColorF(fillColor);
        tmp.push(x,y,0.f);
        for(float a = b; a <= e;a += d){
            float _x = x + w * sin(a);
            float _y = y + h * cos(a);
            tmp.push(_x,_y,0.f);
        }
        draw(tmp);
    }
    if(strokeFlag){
        tmp = PShape(TRIANGLES);
        tmp.setColorS(strokeColor);
        for(float a = b; a <= e;a += d){
            float _x = x + w * sin(a);
            float _y = y + h * cos(a);
            tmp.push(_x,_y,0.f);
        }
        draw(strokify(tmp,strokeWidth,cap,join));
    }
}

// Draws an arc in the display window
inline void arc(float x,float y,float w,float h,float b,float e){
    arc(x,y,w,h,b,e,5.f);
}
// Draws a circle to the screen
inline void circle(float x,float y,float r){
    arc(x,y,r,r,0,TWO_PI);
}

inline void ellipseMode (unsigned mode) {
    assert (mode == CENTER || mode == RADIUS || mode == CORNER || mode == CORNERS);
    eMode = mode;
}

// Draws an ellipse (oval) in the display window
inline void ellipse(float x,float y,float w,float h){
    arc(x,y,w,h,0,TWO_PI);
}

// Draws a line (a direct path between two points) to the screen
inline void line(float x1,float y1,float x2,float y2){
    tmp = PShape(LINES);
    tmp.setColorS(strokeColor);
    tmp.push(x1,y1,0.f);
    tmp.push(x2,y2,0.f);
    draw(strokify(tmp,strokeWidth,cap,join));
    //delete tmp;
}

// Draws a point, a coordinate in space at the dimension of one pixel
inline void point(float x,float y){
    color tmp = fillColor;
    fill(strokeColor);
   circle(x, y, strokeWidth);
    fill(tmp);
}

// A quad is a quadrilateral, a four sided polygon
inline void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    if(fillFlag){
        tmp = PShape(QUADS);
        tmp.setColorF(fillColor);
        tmp.push(x1,y1,0.f);
        tmp.push(x2,y2,0.f);
        tmp.push(x3,y3,0.f);
        tmp.push(x4,y4,0.f);
        draw(tmp);
    }
    if(strokeFlag){
        tmp = PShape(TRIANGLES);
        tmp.setColorS(strokeColor);
        tmp.push(x1,y1,0.f);
        tmp.push(x2,y2,0.f);
        tmp.push(x3,y3,0.f);
        tmp.push(x4,y4,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
    }
}

inline void rectMode (unsigned mode){
    assert (mode == CENTER || mode == RADIUS || mode == CORNER || mode == CORNERS);
    rMode = mode;
}

// Draws a rectangle to the screen
inline void rect(float x,float y,float w,float h){
    if(fillFlag){
        tmp = PShape(TRIANGLE_STRIP);
        tmp.setColorF(fillColor);
        tmp.push(x,y,0.f);
        tmp.push(x+w,y,0.f);
        tmp.push(x,y+h,0.f);
        tmp.push(x+w,y+h,0.f);
        draw(tmp);
    }
    if(strokeFlag){
        tmp = PShape();
        tmp.setColorS(strokeColor);
        tmp.push(x,y,0.f);
        tmp.push(x+w,y,0.f);
        tmp.push(x+w,y+h,0.f);
        tmp.push(x,y+h,0.f);
        tmp.push(x,y,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
    }
}

inline void arc(PShape& sh,float x,float y,float r, float b,float e){

    float dr;
    if(r >= 0 && r <= 50)
        dr = 10.f;
    else if(r > 50 && r <= 100)
        dr = 5.f;
    else
        dr = 2.f;
    float d = radians(dr);
    for(float a = b; a <= e;a += d){
        float _x = x + r * sin(a);
        float _y = y + r * cos(a);
        float _s = (_x/r + 1)*0.5;
        float _t = (_y/r + 1)*0.5;
        sh.push(_x,_y,_s,_t);
    }
}

inline void arc(PShape& sh,float x,float y,float w,float h,float r, float b,float e){

    float dr;
    if(r >= 0 && r <= 50)
        dr = 10.f;
    else if(r > 50 && r <= 100)
        dr = 5.f;
    else
        dr = 2.f;
    float d = radians(dr);
    for(float a = b; a <= e;a += d){
        float _x = x + r * sin(a);
        float _y = y + r * cos(a);
        float _s = map(_x,0.f,w,0.f,1.f);
        float _t = map(_y,0.f,h,0.f,1.f);
        sh.push(_x,_y,_s,_t);
    }
}

inline void rect(float x,float y,float w,float h,float rtl,float rtr,float rbl,float rbr){
    if(fillFlag){
        tmp = PShape(SHAPE_TYPE::TRIANGLE_FAN);
        tmp.setColorF(fillColor);
        tmp.push(x+w/2,y+h/2,0.f);
        arc(tmp,x+w-rbr ,y+h-rbr,rbr,0       ,PI/2      ); //br
        arc(tmp,x+w-rtr ,y+rtr  ,rtr,PI/2    ,PI        ); //tr
        arc(tmp,x+rtl   ,y+rtl  ,rtl,PI      ,(3*PI)/2  ); //tl
        arc(tmp,x+rbl   ,y+h-rbl,rbl,(3*PI)/2,2*PI      ); //bl
        tmp.push(x+w-rbr ,y+h,0.f);
        draw(tmp);
    }
    if(strokeFlag){
        tmp = PShape(SHAPE_TYPE::TRIANGLES);
        tmp.setColorS(strokeColor);
        arc(tmp,x+w-rbr ,y+h-rbr,rbr,0       ,PI/2      ); //br
        arc(tmp,x+w-rtr ,y+rtr  ,rtr,PI/2    ,PI        ); //tr
        arc(tmp,x+rtl   ,y+rtl  ,rtl,PI      ,(3*PI)/2  ); //tl
        arc(tmp,x+rbl   ,y+h-rbl,rbl,(3*PI)/2,2*PI      ); //bl
        tmp.push(x+w-rbr ,y+h,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
    }
}

inline void rect(float x,float y,float w,float h,float r){
    rect(x,y,w,h,r,r,r,r);
}

// Draws a square to the screen
inline void square(float x,float y,float r){
    rect(x,y,r,r);
}

// A triangle is a plane created by connecting three points
inline void triangle(float x1,float y1,float x2,float y2,float x3,float y3){
    if(fillFlag){
        beginShape(TRIANGLES);
        tmp.setColorF(fillColor);
        vertex(x1,y1);
        vertex(x2,y2);
        vertex(x3,y3);
        endShape();
    }
    if(strokeFlag){
        tmp = PShape(TRIANGLES);
        tmp.setColorS(strokeColor);
        tmp.push(x1,y1,0.f);
        tmp.push(x2,y2,0.f);
        tmp.push(x3,y3,0.f);
        draw(strokify(tmp,strokeWidth,cap,join));
        //delete tmp;
    }
}

//Evaluates the Bezier at point t for points a, b, c, d
float bezierPoint(float a,float b,float c,float d,float t){
    return  (powf((1.0 - t),3) * a) +
            (3.0 * t * pow((1.0 - t),2)*b) +
            (3.0 * t * t * (1.0 - t) *c) +
            (t*t*t*d);
}

//Draws a Bezier curve on the screen
inline void bezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    tmp = PShape(LINES);
    tmp.setColorS(strokeColor);
    float dt = 0.01f;
    for(float t = 0.0;t < 1.0f;t += dt){
        float _x1 = bezierPoint(x1,x2,x3,x4,t);
        float _y1 = bezierPoint(y1,y2,y3,y4,t);
        tmp.push(_x1,_y1,0.f);
        _x1 = bezierPoint(x1,x2,x3,x4,t+dt);
        _y1 = bezierPoint(y1,y2,y3,y4,t+dt);
        tmp.push(_x1,_y1,0.f);
    }
    draw(strokify(tmp,strokeWidth,cap,join));
    //delete tmp;
}

inline void image(const PImage& img,float x,float y,float w,float h){
    tmp = PShape(TRIANGLE_STRIP);
    tmp.push(x  ,y  ,0.f,0.f);
    tmp.push(x+w,y  ,1.f,0.f);
    tmp.push(x  ,y+h,0.f,1.f);
    tmp.push(x+w,y+h,1.f,1.f);
    call = 1;
    textureID = img.getID();
    draw(tmp);
}

inline void image(const PImage& img,float x,float y,float w,float h,float rtl,float rtr,float rbl,float rbr){
    tmp = PShape(SHAPE_TYPE::TRIANGLE_FAN);
    tmp.push(w/2,h/2,0.5f,0.5f);
    arc(tmp,w-rbr ,h-rbr,w,h,rbr,0       ,PI/2    ); //br
    arc(tmp,w-rtr ,rtr  ,w,h,rtr,PI/2    ,PI      ); //tr
    arc(tmp,rtl   ,rtl  ,w,h,rtl,PI      ,(3*PI)/2); //tl
    arc(tmp,rbl   ,h-rbl,w,h,rbl,(3*PI)/2,2*PI    ); //bl
    tmp.push(w-rbr ,h,tmp.at(1).s,tmp.at(1).t);
    call = 1;
    textureID = img.getID();
    pushMatrix();
    translate(x,y);
    draw(tmp);
    popMatrix();
}

inline void image(const PImage& img,float x,float y,float w,float h,float r){
  image(img,x,y,w,h,r,r,r,r);
}

inline void image(const PImage& img,float x,float y,float r){
    image(img,x-r,y-r,2*r,2*r,r);
}

inline void image(const PImage& img,float x,float y){
    image(img,x,y,img.getWidth(),img.getHeight());
}

// ================================================================================

inline void textFont(PFont fnt,float size = 16.f){
    if(defSize != size)
        defSize = size;
    if(curr.fontName != fnt.fontName)
        curr = fnt;
    if(!curr.isLoaded())
        curr.init(defSize);
}

inline void text(std::string msg,float x,float y){
    curr.init(defSize);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float scale = 1.0f;
    for(char c : msg){
        Character ch = curr.Characters[c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y + (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        tmp = PShape(TRIANGLE_STRIP);
        tmp.setColorF(fillColor);
        tmp.push(xpos  ,ypos  ,0.f,1.f);
        tmp.push(xpos+w,ypos  ,1.f,1.f);
        tmp.push(xpos  ,ypos-h,0.f,0.f);
        tmp.push(xpos+w,ypos-h,1.f,0.f);
        call = 2;
        textureID = ch.TextureID;
        draw(tmp);
        x += (ch.Advance >> 6) * scale;
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

inline float textWidth(std::string& msg){
    float s = 0.f;
    float scale = 1.0f;
    for(char c : msg){
        Character ch = curr.Characters[c];
        s += ch.Size.x * scale + (ch.Advance >> 6) * scale;
    }
    return s;
}

inline void frameRate(int fr){
    if(fr != 0 || fr < 0)
        framerate = 1.0/fr;
    else
        frameRate(30);
}

inline void antialiasing(boolean active){
    if(active)
        config |= (1 << 2);
}

inline void title(const std::string& t){
    if(window == nullptr)
        windowT = t;
    else
        glfwSetWindowTitle(window,windowT.c_str());
}

inline void fullScreen(){
    config |= 1 << 1;
}

inline void size(int w,int h)  {
    if(window == nullptr){
        width  = w;
        height = h;
    }else{
        glfwSetWindowSize(window,w,h);
    }
}

inline void noLoop(){
    looping = false;
}

inline void err(int a,const char* b){
    printf("GLFW Error %i: \n%s\n",a,b);
}
inline void onKey(GLFWwindow* w, int _key, int _scancode, int _action, int _mods){
    key = _key;

#ifdef USE_KEYS
    keypressed = (bool)(_action == GLFW_PRESS);
    if(keypressed){
        keyCode = _key;
        keyPressed();
    }
    keypressed = false;
#endif
}

PImage loadImage(const std::string& name){
    tex->add(PImage(name));
    return tex->getLast();
}

int main(int argc, char** argv){

    tex = new Manager<PImage>();

    for(int i = 1; i < argc; i++)
        args.push_back(std::string(argv[i]));

    for(std::string s : args)
        std::cout << s << "\n";

    title(" ");
    frameRate(60);
    antialiasing(true);
    setup();

    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(config & (1 << 2)){
        glfwWindowHint(GLFW_SAMPLES,4);
        printf("Antialiasing : enabled\n");
    }
    if(config & (1 << 1))
        glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(),&screenWidth,&screenHeight,&width,&height);

    window = glfwCreateWindow(width,height,windowT.c_str(),config & (1 << 1) ? glfwGetPrimaryMonitor() : NULL,NULL);

    if(!window){
        err(0,"Window = 0");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); //this takes 100% cpu | but get v-sync (when interval == 0)
    glfwSetErrorCallback(err);
    glfwSetKeyCallback(window,onKey);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSize(window,width,height);

    if(config & (1 << 2))
        glEnable(GL_MULTISAMPLE);  

    initialized = true;
    initGL();
    glEnable(GL_TEXTURE);
    glEnable(GL_SCISSOR_TEST);
    double lasttime = glfwGetTime();
    tex->load();
    #ifdef FPS
    int fps = 0;
    double fpsTime = 0.0;
    #endif
    while(!glfwWindowShouldClose(window)){
        double now = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r,bg.g,bg.b,bg.a);
        pmouseX = mouseX;
        pmouseY = mouseY;
        glfwGetCursorPos(window,&mouseX,&mouseY);
        int w,h;
        glfwGetWindowSize(window,&w,&h);
        if(w < 128) w = 128;width  = w;
        if(h < 128) h = 128;height = h;
        glfwSetWindowSize(window,w,h);
        glScissor(0,0,w,h);

        if(looping){
            begin(w,h);
            draw();
            end();
            while (glfwGetTime() < lasttime + framerate) {}
            lasttime += framerate;
            glfwSwapBuffers(window);
            frameCount++;            
            #ifdef FPS
            ++fps;
            if(glfwGetTime() - fpsTime >= 5.0){
                printf("FPS : %f\n",(double)fps/5.0);
                std::flush(std::cout);
                fps = 0;
                fpsTime = glfwGetTime();
            }
            #endif
        }

        key = 0;
        glfwPollEvents();
    }
    doneGL();
    glDisable(GL_TEXTURE);
    glDisable(GL_SCISSOR_TEST);
    args.clear();
    tex->done();
    glfwTerminate();

    return 0;
}

#endif
