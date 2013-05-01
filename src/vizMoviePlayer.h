//
//  vizMoviePlayer.h
//  Sketch06
//
//  Created by Moc O'Matics on 27/04/2013.
//
//

#ifndef __Sketch06__vizMoviePlayer__
#define __Sketch06__vizMoviePlayer__

#include <iostream>
//
//  vizDots.h
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//

#include <iostream>
#include "ofMain.h"
#include "Dot.h"
#include "ofxUI.h"
#include "ofxTuio.h"

class vizMoviePlayer {
public:
    void setup();
    
    void draw();
    void exit();
    
    void keyPressed(int key);
    void update();
    void update(ofVec3f target);
    void update(list<ofxTuioCursor*> cursors);
    void toggleInput(bool b);
    void buildGrid();
    // - - - - - - BASE GUI
    int _id;
        
    ofColor color;
    int gridResX = 20;
    int gridResY = 20;
    float gridWidth = 800;
    float gridHeight = 800;
    float mReactionRadius;
    
    bool mbFirstRun;
    
    
    vector<pair<int, int> > mTunePositions;
    vector<pair<int, int> > mBeatsPositions;

    float videoWidth, videoHeight, mMovieSpeed;
    int mouseX, mouseY;
    float mModeselection;
    
    // - - - - - - VIDEO PLAYER
    
//    enum ModeName {
//        kModeNikeBeats,
//        kModeNikeTune,
//        kModeConverseTune,
//        kModeConverseFX,
//        kModeAdidasBeats,
//        kModeAdidasFX
//        
//    };
    //testApp::ModeName mCurrentMode;
    //bool setMode(ModeName newMode);
    
    list<Dot*> nikeBeatsVideos; // 12 vids, ordered
    list<Dot*> nikeTuneVideos; // one video
    list<Dot*> converseTuneVideos; // one video
    list<Dot*> adidasBeatsVideos; //
    
    ofDirectory nikeTuneDir;
    ofDirectory nikeBeatsDir;
    ofDirectory converseTuneDir;
    ofDirectory adidasBeatsDir;
    
    
    int mNumVideos;
    int mNumPlayers;
    int mNumCursors;
    
    
    
    
    ofVideoPlayer* mOverlayMovie;
    bool mbBlendModeAdd;
    
    
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
    ofVec3f mTarget;
  
};

#endif /* defined(__Sketch06__vizMoviePlayer__) */
