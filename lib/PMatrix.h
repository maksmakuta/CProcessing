#ifndef PMATRIX_H
#define PMATRIX_H

class iMatrix{
  public:
    void reset();
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
    void apply(float n00, float n01, float n02,
                      float n10, float n11, float n12);
    void apply(float n00, float n01, float n02, float n03,
                      float n10, float n11, float n12, float n13,
                      float n20, float n21, float n22, float n23,
                      float n30, float n31, float n32, float n33);
    void transpose();
    bool invert();
    float determinant();
};

class PMatrix : public iMatrix{

};

#endif // PMATRIX_H
