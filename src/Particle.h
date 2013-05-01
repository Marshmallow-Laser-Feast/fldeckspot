//
//  Particle.h
//  Sketch02
//
//  Created by Moc O'Matics on 24/04/2013.
//
//

#ifndef __Sketch02__Particle__
#define __Sketch02__Particle__

#include <iostream>
#include "ofMain.h"


class Particle {
public:
    void setup(int id);
    void update();
    void draw();
    
    void setTarget(ofVec3f p);
    void setHome(ofVec3f h);
    void setWidth(float w);
    void setHeight(float h);
    void setColor(ofColor c);
    void setOffset(float offsetX, float offsetY);
    void setIdle(bool idle);
    void setIdleRotation(float rot);
    void setNeedleOffset(float offset);
    void setEase(float ease);
    void setStrength(float s);
    int ID;
    
    bool mbIdle;
    
    ofVec3f target, current, home, vel, acc;
    
    float x, y, width, height, mIdleRotation, rotation, tuioOffsetX, tuioOffsetY, mNeedleOffset, mEase, mStrength;
    ofColor color;
     
    
    
    
};

#endif /* defined(__Sketch02__Particle__) */
