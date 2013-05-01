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
#include "OutputManager.h"

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
    ofxUIScrollableCanvas *gui;
    ofImage m_imgGradient;
    int mVizNum;
    float *buffer;
    ofxUIMovingGraph *mg;
    ofxUIToggle* vizNeedlesToggle;
    ofxUIToggle* vizConverseToggle;
    ofxUIToggle* vizPhysicsToggle;
    ofxUIToggle* vizDotsToggle;
    ofxUIToggle* vizMoviePlayerToggle;
    
    //- - - - - - - - -
    ofColor mBgColor;
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
    
    ofImage img;
    char mCapturePath[32] ;
    float mTakeNum;
    
    // - - --  LED
    bool setLEDColor(ofColor color);
    ofColor mStartColor, mAdidasColor, mConverseColor, mNikeColor, mCurrentColor, mEndColor, mTargetColor;
    int  mLEDCues [6] = {0, 588, 1299, 1742, 2000, 2350};
    
    
    //- - - - - - - MIDI
    void newMidiMessage(ofxMidiMessage& eventArgs);
	stringstream text;
	ofxMidiIn midiIn;
    ofxMidiOut midiOut;
	ofxMidiMessage midiMessage;
        
    //- - - - - - - SCENE RUNNER
    bool mbMainVideo;
    int  mMovieLength = 2300;
    int  mRanges [9] = {0,250, 588, 1065, 1299, 1536, 1742, 2350, mMovieLength};
   
    OutputManager outputManager;
    
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
