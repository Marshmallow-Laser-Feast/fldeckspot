//
//  Particle.cpp
//  Sketch02
//
//  Created by Moc O'Matics on 24/04/2013.
//
//

#include "Particle.h"

//--------------------------------------------------------------
void Particle::setup(int id){
    ID = id;
//    cout << "Particle::setup() ID: " << ID << endl;
    target = home;
    current = home;
    
    color = ofColor(65,0,123);
    width = 40;
    height = 2;
    mEase = 7.0f;
    
}

//--------------------------------------------------------------
void Particle::update(){
    float rot;
    if (mbIdle) {
         rot = mIdleRotation;
    } else {
        ofVec3f distVec = target - current;
        rot = atan2( target.y - current.y, target.x - current.x)/PI *180;
        //rot = (mIdleRotation*(1-mStrength) + rot*mStrength)/2.0;
        
        
    }
    rotation = ofLerpDegrees(rotation, rot, 1/mEase);

    //width = distVec.length();
}

//--------------------------------------------------------------
void Particle::draw(){
    ofSetColor(color);
    //ofSphere(current, 20.0f);
    glPushMatrix();
    ofTranslate(home+ ofVec3f(tuioOffsetX, tuioOffsetY, 0));
    ofRotateZ(rotation);
    
    ofRect(mNeedleOffset, -height/2.0, width, height);
    glPopMatrix();
    //ofEllipse(ofPoint(current), width, height);
}

//--------------------------------------------------------------
void Particle::setIdle(bool idle){
//    cout << "Particle::setTarget() ID: " << p << endl;
    mbIdle = idle;
}

void Particle::setIdleRotation(float rot){
    //    cout << "Particle::setTarget() ID: " << p << endl;
    mIdleRotation = rot;
}
void Particle::setNeedleOffset(float offset){
    //    cout << "Particle::setNeedleOffset() ID: " << p << endl;
    mNeedleOffset = offset;
}
void Particle::setStrength(float s){
    //    cout << "Particle::setIdle() ID: " << p << endl;
    mStrength = s;
}

void Particle::setTarget(ofVec3f p){
    //    cout << "Particle::setIdle() ID: " << p << endl;
    target = p;
}

void Particle::setHome(ofVec3f h){
//    cout << "Particle::setHome() ID: " << h << endl;
    home = h;
}
void Particle::setWidth(float w){
    //    cout << "Particle::setWidth() ID: " << h << endl;
    width = w;
}
void Particle::setHeight(float h){
    //    cout << "Particle::setHeight() ID: " << h << endl;
    height = h;
}
void Particle::setColor(ofColor c){
    //    cout << "Particle::setColor() ID: " << h << endl;
    color = c;
}
void Particle::setEase(float ease){
    //    cout << "Particle::setColor() ID: " << h << endl;
    mEase = ease;
}

void Particle::setOffset(float offsetX, float offsetY){
    //    cout << "Particle::setHome() ID: " << h << endl;
    //tuioOffsetX = offsetX;
    //tuioOffsetY = offsetY;
}