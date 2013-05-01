#pragma once

#include "ofMain.h"
#include "ofxTuio.h"
#include "Particle.h"
#include "ofxUI.h"
#include "vizCompassNeedles.h"
#include "vizDots.h"
#include "vizPhysics.h"
#include "vizConverse.h"
#include "vizMoviePlayer.h"
#include "ofxMidi.h"

class testApp : public ofBaseApp, public ofxMidiListener {
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
   
  
    //- - - - - - - OFX TUIO - - - - - - -
    ofxTuioCursor* getFirstCursor();
    ofxTuioClient   tuioClient;
    float tuioOffsetX, tuioOffsetY, mXOffset;
    
    
    //- - - - - - - OFX UI - - - - - - - -
    void guiEvent(ofxUIEventArgs &e);
    ofxUICanvas *gui;
    ofImage m_imgGradient;
    int mVizNum;
    float *buffer;
    ofxUIMovingGraph *mg;
    
    
    //- - - - - - - - -
    ofColor mBgColor, mAdidasColor, mConverseColor, mNikeColor;
    bool bUpdateTUIO, mbToggleCapture,
    mbShowCursor, mbToggleDots,
    mbToggleNeedles, mbTogglePhysics,
    mbToggleConverse, mbToggleMoviePlayer, mbToggle1440;
    
    //- - - - - - - Visualisations
    vizCompassNeedles* compassNeedles;
    vizDots* dots;
    vizPhysics* physicsViz;
    vizConverse* converseViz;
    vizMoviePlayer* moviePlayerViz;
    
    ofVideoPlayer mainVideo;
    bool doShowColorbars;
    ofImage imageColorBars;
    
    //ofxImageSequenceRecorder ImageRecorder;
    ofImage img;
    char mCapturePath[32] ;
    float mTakeNum;
    
    //- - - - - - - Visualisations
    void newMidiMessage(ofxMidiMessage& eventArgs);
	stringstream text;
	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
        
    //- - - - - - - SCENE RUNNER
    bool mbMainVideo;
    int  mMovieLength = 2300;
    int  mRanges [9] = {0,250, 588, 1065, 1299, 1536, 1742, 2350, mMovieLength};
    int  mLEDCues [4] = {0, 588, 1299, 1742};
  
    enum ModeName {
        kModeNikeBeats,
        kModeNikeTune,
        kModeConverseTune,
        kModeConverseFX,
        kModeAdidasBeats,
        kModeAdidasFX
        
    };
    ModeName mCurrentMode;
    int mCurrentFrame;
    float selectedFrame;
    bool setMode(ModeName newMode);
    ofxUISlider* movieSlider;
};
