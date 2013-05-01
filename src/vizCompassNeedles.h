//
//  vizCompassNeedles.h
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//

#ifndef __Sketch06__vizCompassNeedles__
#define __Sketch06__vizCompassNeedles__

#include <iostream>
#include "ofMain.h"
#include "Particle.h"
#include "ofxUI.h"
#include "ofxTuio.h"


class vizCompassNeedles {
public:
    void setup();
    void update(ofVec3f target);
    void update( list<ofxTuioCursor*> cursors);
    void draw();
    void exit();
    void buildGrid();
    // - - - - - - BASE GUI
    int _id;
    
    // - - - - - - COMPASS NEEDLES
    list<Particle*> particles;
    
    ofColor color, mBgColor;
    int gridResX = 20;
    int gridResY = 20;
    float gridWidth = 800;
    float gridHeight = 800;
    
    
    float needleWidth, needleHeight, scaleX, scaleY, mIdleRotation, mGridRatio, mLineOffset, mNeedleOffset, mAlpha, mEase;
    int mouseX, mouseY;
    float mRange;
    bool bEaseUp, mbToggleIdle, mbBlendModeAdd, mbToggleConstrain;
    
    void keyPressed(int key);
    void setIdle(bool b);
    bool mbIdle;
    
    //- - - - - - - OFX UI - - - - - - - -
    void guiEvent(ofxUIEventArgs &e);
    ofxUICanvas *gui;
    ofImage m_imgGradient;
    char mVizIdNameCount[32];
    int smVizCount;
    int mPresetCount;
    float mSelectedPreset;
   
    char mVizName[32];
    char mVizIdName[32];
    char mXmlPath[32];
    char mVizIdWeightName[32];
};




#endif /* defined(__Sketch06__vizCompassNeedles__) */
