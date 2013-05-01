//
//  vizDots.h
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//

#ifndef __Sketch06__vizDots__
#define __Sketch06__vizDots__

#include <iostream>
#include "ofMain.h"
#include "Dot.h"
#include "ofxUI.h"
#include "ofxTuio.h"

class vizDots {
public:
    void setup();
    
    void draw();
    void exit();
    
    void keyPressed(int key);

    void update(ofVec3f target);
    void update(list<ofxTuioCursor*> cursors);
    
    void buildGrid();
    // - - - - - - BASE GUI
    int _id;
    // - - - - - - DOTS
    list<Dot*> dots;
    
    ofColor color;
    int gridResX = 20;
    int gridResY = 20;
    float gridWidth = 800;
    float gridHeight = 800;
    
    
    float needleWidth, needleHeight, scaleX, scaleY, mIdleRotation, mGridRatio, mLineOffset, mNeedleOffset,mAlpha;
    int mouseX, mouseY;
    float easeW;
    bool bEaseUp, mbToggleIdle, mbBlendModeAdd, mbToggleConstrain;
    
        
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

#endif /* defined(__Sketch06__vizDots__) */
