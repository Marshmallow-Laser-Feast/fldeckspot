//
//  OutputManager.h
//  Sketch06
//
//  Created by Memo Akten on 01/05/2013.
//
//


#pragma once

#include "ofMain.h"

class OutputManager {
public:
    bool enabled;
    
    struct Output {
        bool enabled;       // whether this output is enabled (i.e. drawn) or not
        ofRectangle rect;   // the rectangle (ie. position and size on screen) this output is drawn to
        ofVec3f rot;        // x, y, z rotation of this output
    };
    
    vector<Output> outputs; // all of our outputs
    
    //------------------------------------------------------------
    void setup(int numOutputs, int w, int h, int internalformat, int numSamples) {
        fboMain.allocate(w, h, internalformat, numSamples);
        fboMain.begin();
        ofClear(0);
        fboMain.end();
        
        for(int i=0; i<numOutputs; i++) {
            Output o;
            o.enabled = true;
            o.rect.set(0, 0, w, h);
            o.rot.set(0, 0, 0);
            outputs.push_back(o);
        }
        
        enabled = true;
    }
    
    //------------------------------------------------------------
    void draw() {
        if(enabled) {
            ofPushStyle();
            ofDisableAlphaBlending();
            glDisable(GL_DEPTH_TEST);
            ofSetColor(255, 255, 255);
            
            for(int i=0; i<outputs.size(); i++) {
                Output &o = outputs[i];
                if(o.enabled) {
                    ofPushMatrix();
                    ofTranslate(o.rect.x, o.rect.y);
                    fboMain.draw(0, 0, o.rect.width, o.rect.height);
                    ofPopMatrix();
                }
            }
            ofPopStyle();
        }
    }
    
    //------------------------------------------------------------
    void begin() {
        if(enabled) fboMain.begin();
    }
    
    //------------------------------------------------------------
    void end() {
        if(enabled) fboMain.end();
    }
    
protected:
    ofFbo fboMain;
    
    
};
