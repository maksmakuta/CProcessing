#ifndef PSKETCH_H
#define PSKETCH_H

class PSketch{
  public:
    virtual void setup() = 0;
    virtual void draw() = 0;

    virtual void keyPressed(){}
    virtual void keyReleased(){}
    virtual void keyTyped(){}

    virtual void mouseClicked(){}
    virtual void mouseDragged(){}
    virtual void mouseMoved(){}
    virtual void mousePressed(){}
    virtual void mouseReleased(){}
    virtual void mouseWheel(){}
};

#endif // PSKETCH_H
