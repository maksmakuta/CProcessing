#ifndef PROCESSING_H
#define PROCESSING_H

/**
 * processing v.2.0-beta1
 * @author Maks Makuta
 */

#define null nullptr

#define PI 3.14159265358979323846
#define HALP_PI PI/2.0
#define TWO_PI 2.0*PI
#define TAU TWO_PI
#define QUARTER_PI PI/4

// ==================== classes =====================

class PVector{
public:
    float x,y,z;

    PVector();
    PVector(float _x,float _y);
    PVector(float _x,float _y,float _z);

    void set(float _x,float _y,float _z);
    void set(float _x,float _y);
    void set(PVector v);
    void set(float v[3]);

    static PVector random2D();
    static PVector random2D(PVector& target);

    static PVector random3D();
    static PVector random3D(PVector& target);

    static PVector fromAngle(float angle);
    static PVector fromAngle(float angle, PVector& target);

    PVector copy();
    float mag();
    float magSq();

    PVector add(PVector& v);
    PVector add(float x, float y);
    PVector add(float x, float y, float z);
    static PVector add(PVector& v1, PVector& v2);
    static PVector add(PVector& v1, PVector& v2, PVector& target);

    PVector sub(PVector& v);
    PVector sub(float x, float y);
    PVector sub(float x, float y, float z);
    static PVector sub(PVector& v1, PVector& v2);
    static PVector sub(PVector& v1, PVector& v2, PVector& target);

    PVector mult(float n);
    static PVector mult(PVector& v, float n);
    static PVector mult(PVector& v, float n, PVector& target);

    PVector div(float n);
    static PVector div(PVector& v, float n);
    static PVector div(PVector& v, float n, PVector& target);

    float dist(PVector& v);
    static float dist(PVector& v1,PVector& v2);

    float dot(PVector& v);
    float dot(float x, float y, float z);
    static float dot(PVector& v1, PVector& v2);

    PVector cross(PVector& v);
    static PVector cross(PVector& v, PVector& target);
    static PVector cross(PVector& v1, PVector& v2, PVector& target);

    PVector normalize();
    PVector normalize(PVector& target);

    PVector limit(float max);

    PVector setMag(float len);
    static PVector setMag(PVector& target,float len);

    float heading();
    PVector rotate(float theta);

    PVector lerp(PVector& v,float amt);
    PVector lerp(PVector& v1, PVector& v2, float amt);
    PVector lerp(float x,float y,float z, float amt);

};


// ================== public functions ===================
void setup();
void draw();
void keyPressed();

#endif
