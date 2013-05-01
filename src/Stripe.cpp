//
//  Stripe.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 26/04/2013.
//
//

#include "Stripe.h"
//--------------------------------------------------------------
void Stripe::setup(int id){
    ID = id;
}

//--------------------------------------------------------------
void Stripe::update(){
    rotation = mIdleRotation;
    //width += (width- mIdleWidth)/mEase;
    //height += (mIdleHeight-height)/mEase;
}

//--------------------------------------------------------------
void Stripe::draw(){
    ofSetColor(color);

    //ofRect(0, -height/2.0, width, height);
    ofPushMatrix();
    ofTranslate(home);
    ofRotateZ(mIdleRotation);
    ofRect(0, -height/2.0, width, height);
    ofPopMatrix();
    //ofEllipse(ofPoint(current), width, height);
}

//--------------------------------------------------------------
void Stripe::setIdleRotation(float rot){
    //cout << "Stripe::setIdleRotation() ID: " << rot << endl;
    mIdleRotation = rot;
}

void Stripe::setTarget(ofVec3f p){
    //    cout << "Stripe::setIdle() ID: " << p << endl;
    target = p;
}

void Stripe::setHome(ofVec3f h){
    //cout << "Stripe::setHome() ID: " << h << endl;
    home = h;
}
void Stripe::setIdleWidth(float w){
    //cout << "Stripe::setIdleWidth() ID: " << ID << " " << w << endl;
    mIdleWidth = w;
}
void Stripe::setWidth(float w){
    //cout << "Stripe::setWidth() ID: " << ID << " " << w << endl;
    width = w;
}
void Stripe::setHeight(float h){
    //    cout << "Stripe::setHeight() ID: " << h << endl;
    height = h;
}
void Stripe::setIdleHeight(float h){
    //    cout << "Stripe::setIdleHeight() ID: " << h << endl;
    mIdleHeight = h;
}
void Stripe::setColor(ofColor c){
    //    cout << "Stripe::setColor() ID: " << h << endl;
    color = c;
}
void Stripe::setEase(float ease){
    //    cout << "Stripe::setColor() ID: " << h << endl;
    mEase = ease;
}