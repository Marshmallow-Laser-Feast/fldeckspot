//
//  Dot.h
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//

#ifndef __Sketch06__Dot__
#define __Sketch06__Dot__

#include <iostream>
#include "ofMain.h"
#include "ofxTweenzor.h"

class Dot {
public:
    void setup(int id);
    void update();
    void draw();
    
    void setTarget(ofVec3f p);
    void setHome(ofVec3f h);
    void setWidth(float w);
    void setHeight(float h);
    void setColor(ofColor c);
    void setIdle(bool idle);
    void setIdleRotation(float rot);
    void setVideo(string path);
    void setVideo(bool bVideo);
    void setSpeed(float speed);
    void playVideo();
    void pauseVideo();
    int ID;
    ofVideoPlayer* mVideoPlayer;
    bool mbIdle, mbEasing, mbVideo, mbLoaded;
       
    ofVec3f target, current, home, vel, acc;
    
    float x, y, width, height, mIdleRotation, rotation, mEaseProgress, mMovieSpeed;
    ofColor color;
    
    void onComplete(float* arg);
    void exit();
    
};

#endif /* defined(__Sketch06__Dot__) */
