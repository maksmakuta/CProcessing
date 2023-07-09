#include "backend/gl/gl_backend.h"
#include "processing.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void todo(const std::string& text){
    std::cout << "TODO: " << text << std::endl;
}

// ================== variables =====================

backend *gl = null;

long _seed = 0L;    // random seed
long _pseed = 0L;   // perlin noise seed
int _poctaves = 4;  // perlin octaves
float _ppersisrtence = 0.5; // perlin persistance

// =================== methods ======================

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


int abs(int n){
    return std::abs(n);
}
float abs(float n){
    return std::abs(n);
}
float ceil(float n){
    return std::ceil(n);
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
float exp(float n) {
    return std::exp(n);
}
float lerp(float start, float end, float t){
    return start + t * (end - start);
}
float log(float n){
    return std::log(n);
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
float pow(float n,float e){
    return std::pow(n,e);
}
int round(float n){
    return std::roundf(n);
}
int floor(float n){
    return std::floor(n);
}
float sq(float n){
    return n*n;
}
float sqrt(float n){
    return std::sqrt(n);
}
float acos(float value){
    return std::acos(value);
}
float asin(float value){
    return std::asin(value);
}
float atan2(float y, float x){
    return std::atan2(y,x);
}
float atan(float value){
    return std::atan(value);
}
float cos(float angle){
    return std::cos(angle);
}
float degrees(float radians){
    return radians * (180.f / PI);
}
float radians(float degrees){
    return degrees * (PI / 180.f);
}
float sin(float angle){
    return std::sin(angle);
}
float tan(float angle){
    return std::tan(angle);
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


// ============== classes ======================

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
PVector PVector::set(PVector v){
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
float PVector::mag(){
    return sqrt(x * x + y * y + z * z);
}
float PVector::magSq(){
    return x * x + y * y + z * z;
}
PVector PVector::add(PVector& v){
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
PVector PVector::add(PVector& v1, PVector& v2){
    return v1.add(v2);
}
PVector PVector::add(PVector& v1, PVector& v2, PVector& target){
    target.set(v1);
    target.add(v2);
    return target;
}
PVector PVector::sub(PVector& v){
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
PVector PVector::sub(PVector& v1, PVector& v2){
    return v1.sub(v2);
}
PVector PVector::sub(PVector& v1, PVector& v2, PVector& target){
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
PVector PVector::mult(PVector& v, float n){
    PVector t = v.copy();
    t.mult(n);
    return t;
}
PVector PVector::mult(PVector& v, float n, PVector& target){
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
PVector PVector::div(PVector& v, float n){
    PVector t = v.copy();
    t.div(n);
    return t;
}
PVector PVector::div(PVector& v, float n, PVector& target){
    target.set(v);
    target.div(n);
    return target;
}
float PVector::dist(PVector& v){
    float dx = this->x - v.x;
    float dy = this->y - v.y;
    float dz = this->z - v.z;
    return std::sqrt(dx*dx+dy*dy+dz*dz);
}
float PVector::dist(PVector& v1,PVector& v2){
    return v1.dist(v2);
}
float PVector::dot(PVector& v){
    return this->dot(v.x,v.y,v.z);
}
float PVector::dot(float x, float y, float z){
    float dx = this->x * x;
    float dy = this->y * y;
    float dz = this->z * z;
    return dx+dy+dz;
}
float PVector::dot(PVector& v1, PVector& v2){
    return v1.dot(v2);
}
PVector PVector::cross(PVector& v){
    float rX = this->y * v.z - this->z * v.y;
    float rY = this->z * v.x - this->x * v.z;
    float rZ = this->x * v.y - this->y * v.x;
    return PVector(rX,rY,rZ);
}
PVector PVector::cross(PVector& v, PVector& target){
    PVector t = target.cross(v);
    return target.set(t);
}
PVector PVector::cross(PVector& v1, PVector& v2, PVector& target){
    return target.set(v1.cross(v2));
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

PVector PVector::lerp(PVector& v,float amt){

}
PVector PVector::lerp(PVector& v1, PVector& v2, float amt){

}
PVector PVector::lerp(float x,float y,float z, float amt){

}







// ================== main driver ===================
void ProcessInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){
    gl = new GLBackend();
    setup();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Construct the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Template", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Handle view port dimensions
    gl->viewport(800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
        gl->viewport( width, height);
    });

    // This is the render loop
    while (!glfwWindowShouldClose(window)){
        ProcessInput(window);

        gl->clearColor(1.00f, 0.2f, 0.9f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete gl;
    return 0;
}
