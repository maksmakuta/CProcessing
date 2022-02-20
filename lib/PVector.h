#ifndef PVECTOR_H
#define PVECTOR_H

#include <cmath>
#include "PMath.h"

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

    PVector cross(PVector a,PVector b){
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

    PVector lerp(PVector v, float amt) {
        this->x = PMath::lerp<float>(this->x,v.x,amt);
        this->y = PMath::lerp<float>(this->y,v.y,amt);
        this->z = PMath::lerp<float>(this->z,v.z,amt);
        return *this;
      }

    PVector lerp(PVector v1,PVector v2, float amt) {
        return v1.lerp(v2,amt);
    }

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
        PVector p(PMath::random<float>(-1,1),PMath::random<float>(-1,1));
        return p.normalize();
    }

    float x,y,z;
};

PVector createVector(float x,float y,float z){
    return PVector(x,y,z);
}

PVector createVector(float x,float y){
    return createVector(x,y,0);
}

bool equal(const PVector& a,const PVector& b){
    return a.x == b.x && a.y == b.y && a.z == b.z;
}


#endif
