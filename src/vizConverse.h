//
//  vizConverse.h
//  Sketch06
//
//  Created by Moc O'Matics on 26/04/2013.
//
//

#ifndef __Sketch06__vizConverse__
#define __Sketch06__vizConverse__

#include <iostream>

#include <iostream>
#include "ofMain.h"
#include "ofxUI.h"
#include "ofxTuio.h"
#include "MSAPhysics3D.h"
#include "ofxMidi.h"
class vizConverse {
public:
    void setup();
    
    void draw();
    void drawFBO();
    void exit();
    
    void keyPressed(int key);
    void update();
    void update(ofVec3f target);
    void update(list<ofxTuioCursor*> cursors);
    void mouseMoved(int x, int y );
    void mouseReleased(int x, int y, int button);
    // - - - - - - BASE GUI
    int _id;
    
    ofColor color;
    
    float needleWidth, needleHeight, scaleX, scaleY, mIdleRotation, mSphereSize, mLineOffset, mNeedleOffset,mAlpha;
    int mouseX, mouseY;
    float easeW;
    bool bEaseUp, mbToggleIdle, mbBlendModeAdd, mbToggleConstrain;
  
    
    //- - - - - - - OFX UI - - - - - - - -
    void guiEvent(ofxUIEventArgs &e);
    
    //ofxUICanvas *gui;
    ofxUIScrollableCanvas *gui;
    ofxUICanvas *gui2;
    
    ofImage m_imgGradient;
    char mVizIdNameCount[32];
    int smVizCount;
    int mPresetCount;
    float mSelectedPreset;
    
    char mVizName[32];
    char mVizIdName[32];
    char mXmlPath[32];
    char mXmlPath2[32];
    char mVizIdWeightName[32];
    
    float radius, innerRadius;
    
    // - - - - - - - VARS
    bool mbCamTop, mbTogglePhysics, mbShowDots, mbToggleOverlayImage, mbToggleShape, mbToggleOverlayMovie, mbToggleCamRot;
    float mGravity, mDrag, 
          mCamTopY, mSpringRestLength, mSpringStrength, mIntensity, mRange, mRadius, mOffset,
    mInnerStripeRadiusMax, mInnerStripeRadiusMin,
    mOuterStripeRadiusMin, mOuterStripeRadiusMax,
    mOuterStripeThickness, mInnerStripeThickness, mSecondRingStripeThickness,
    mSecondRingOffsetAngle, mStripeOffset,
    mSecondRingRadiusMin, mSecondRingRadiusMax,
    mStripeOffsetAngle, mLineStepInside, mLineStepOutside, mLineThickness;
    
    ofColor deepRed = ofColor(176, 8, 31);
    ofColor blue = ofColor(39, 41, 115);
    ofColor grey = ofColor(178, 178, 178);
    ofColor mBgColor, mStripeInsideColor, mStripeOutsideColor, mPhysicsLineColor, mPhysicsColor;
    // - - - - - - - - CAMERA
    ofEasyCam cam;
    ofCamera camTop;
    ofFbo rgbaFbo;
    ofVec3f camTopIdleRotation;
    float mCamOrbitLongitude;
    // - - - - - - - - PHYSICS
    msa::physics::World3D		physicsConverse;
    msa::physics::Particle3D *center;
    vector< msa::physics::Particle3D*> outsideParticles;
    vector< msa::physics::Particle3D*> insideParticles;
    vector< msa::physics::Particle3D*> topParticles;
    
    
    float lastBeat, mCamRotX,mCamRotY, mCamRotZ;
    
    void addParticleCircle();
    void addRandomForce(float f);
    void addOutsideForce(ofVec3f target, float intensity, float range);
    void setSpringStrength(vector<msa::physics::Spring3D*>* group, float strength);
    void setSpringRestLength(vector<msa::physics::Spring3D*>* group, float restLength);
    void setRadius(float radius);
    void drawStripe (float x, float length,float thickness, float angle, ofColor color);
    void loadSettings(int id);
    
    vector< msa::physics::Spring3D*> centerSprings;
    vector< msa::physics::Spring3D*> outsideSprings;
    vector< msa::physics::Spring3D*> neighbourSprings;
    vector< msa::physics::Spring3D*> topRingSprings;
    
    ofVideoPlayer* mOverlayMovie;
    ofImage* mOverlayImage;
    
    list<ofxTuioCursor*> tuioCursors;
    
    void updateMidi (ofxMidiMessage m);

};
#endif /* defined(__Sketch06__vizConverse__) */
