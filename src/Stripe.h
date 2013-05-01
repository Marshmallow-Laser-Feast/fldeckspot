//
//  Stripe.h
//  Sketch06
//
//  Created by Moc O'Matics on 26/04/2013.
//
//

#ifndef __Sketch06__Stripe__
#define __Sketch06__Stripe__

#include <iostream>
#include "ofMain.h"


class Stripe {
public:
    void setup(int id);
    void update();
    void draw();
    
    void setHome(ofVec3f h);
    void setWidth(float w);
    void setHeight(float h);
    
    void setIdleWidth(float w);
    void setIdleHeight(float h);
    
    void setColor(ofColor c);
    void setIdleRotation(float rot);
    void setEase(float ease);
    void setTarget(ofVec3f p);
    int ID;
    
    bool mbIdle;
    
    ofVec3f target, current, home, vel, acc;
    
    float x, y, width, height, mIdleRotation, rotation, mEase;
    float mIdleWidth, mIdleHeight;
    ofColor color;
    
};

#endif /* defined(__Sketch06__Stripe__) */
