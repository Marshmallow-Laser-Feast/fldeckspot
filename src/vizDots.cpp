//
//  vizDots.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//
#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#include "vizDots.h"

void vizDots::setup(){
        
    _id = 1;
    smVizCount = 10;
    mPresetCount = 10;
    mSelectedPreset = 0;
    
    sprintf(mVizName, "vizDots");
    ofLogNotice() << mVizName << "::setup()" << endl;
    
    sprintf(mVizIdName, "%d_%s", _id, mVizName);
    sprintf(mXmlPath, "GUI/%s%i.xml", mVizName, smVizCount);
    sprintf(mVizIdNameCount, "%s %i",mVizName, smVizCount);
    
    // - - - - - - - - - - OFX UI SETUP - - - - - - - - -  - - - - -
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 200-xInit;
    float dim = 16;
    
    ofColor cb = ofColor(0,0,0);
    ofColor co = ofColor(84,0,255);
    ofColor coh = ofColor(0,0,255);
    ofColor cf = ofColor(255,255,255);
    ofColor cfh = ofColor(128,255,128);
    ofColor cp = ofColor(0,0,255);
    ofColor cpo = ofColor(124,0,255);
    
    gui = new ofxUICanvas(400, 0, length+xInit, ofGetHeight()*.75);
    
    gui->setFont("GUI/CarroisGothic-Regular.ttf");                     //This loads a new font and sets the GUI font
    gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
    gui->setFontSize(OFX_UI_FONT_LARGE, 8); //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui->setFontSize(OFX_UI_FONT_SMALL, 6); //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
    
    
    gui->addWidgetDown(new ofxUILabel(mVizName, OFX_UI_FONT_MEDIUM));
    
    gui->addSlider("select preset", 0.0f, mPresetCount, &mSelectedPreset, length-xInit,dim);
    gui->addButton("load", false, dim, dim);
    gui->addButton("save", false, dim, dim);
    
    //gui->addButton("easeUp", &bEaseUp, dim, dim);
    
    //  - - - - - - - - - - DOTS - - - - - - - - - - - - - - - - -
    gui->addWidgetDown(new ofxUILabel("dots", OFX_UI_FONT_LARGE));
    
//    gui->addToggle("toggle idle", &mbToggleIdle, dim, dim);
//    gui->addSlider("idle rotation", 0.0, 360.0, &mIdleRotation, length-xInit,dim);
    gui->addToggle("blend add", &mbBlendModeAdd, dim, dim);
    gui->addSlider("alpha", 0.0, 255.0f, &mAlpha, length-xInit,dim);
    gui->addSlider("dot width", 1.0, 300.0f, &needleWidth, length-xInit,dim);
    gui->addSlider("dot height", 1.0, 300.0f, &needleHeight, length-xInit,dim);
    gui->addSlider("dot offset x", -50.0, 50.0f, &mNeedleOffset, length-xInit,dim);
   
    
    
    // gui->addToggle("constrain", &mbToggleConstrain, dim, dim);
//    gui->addSlider("grid width", 400.0, 4000.0, &gridWidth, length-xInit,dim);
//    gui->addSlider("grid height", 400.0, 4000.0, &gridHeight, length-xInit,dim);
//    gui->addButton("reset", false, dim, dim);
//    gui->addSlider("line offset",-40.0, 40.0, &mLineOffset, length-xInit,dim);
    
    
    gui->addWidgetDown(new ofxUILabel("bg color", OFX_UI_FONT_SMALL));
    
    m_imgGradient.allocate(162,81, OF_IMAGE_COLOR);
    m_imgGradient.loadImage("img/gradient_162x81.png");
    
    gui->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "SAMPLER"));
    gui->toggleVisible();
    
    ofAddListener(gui->newGUIEvent,this,&vizDots::guiEvent);
    gui->loadSettings(mXmlPath);
    
    
}



//--------------------------------------------------------------
void vizDots::update( list<ofxTuioCursor*> cursors) {
    //gives better feeback when selecting presets, does not change the actual value though
    mSelectedPreset = int(mSelectedPreset);
    
    dots.clear();
    int count = 0;
    list<ofxTuioCursor*>::iterator tit;
    for (tit=cursors.begin(); tit != cursors.end(); tit++) {
        ofxTuioCursor* cursor = (*tit);
        float x = cursor->getX()*OUTPUT_WIDTH;
        float y = cursor->getY()*OUTPUT_HEIGHT;
        
        Dot* d = new Dot();
        
        d->setup(count);
        d->setHome(ofVec3f ( x, y, 0.0));
        d->setTarget(ofVec3f ( x, y, 0.0));
        d->setColor(ofColor(color.r, color.g, color.g, mAlpha));
        d->setWidth(needleWidth);
        d->setHeight(needleHeight);
        d->setIdle(mbToggleIdle);
        d->setIdleRotation(mIdleRotation);
        d->update();
        dots.push_back(d);
        
        //cursor->setTarget(target);
        //cursor->update();
        count ++;
    }
}

//--------------------------------------------------------------
void vizDots::update(ofVec3f target) {
    //gives better feeback when selecting presets, does not change the actual value though
    mSelectedPreset = int(mSelectedPreset);
    
    dots.clear();
    Dot* d = new Dot();
    
    d->setup(0);
    d->setHome(ofVec3f ( target.x, target.y, 0.0));
    d->setTarget(ofVec3f ( target.x, target.y, 0.0));
    d->setColor(ofColor(color.r, color.g, color.g, mAlpha));
    d->setWidth(needleWidth);
    d->setHeight(needleHeight);
    d->update();

    dots.push_back(d);
}
//--------------------------------------------------------------
void vizDots::draw(){
    ofPushMatrix();

    if (mbBlendModeAdd) ofEnableBlendMode(OF_BLENDMODE_ADD);
    list<Dot*>::iterator tit;
    for (tit=dots.begin(); tit != dots.end(); tit++) {
        Dot *dot = (*tit);
        dot->draw();
    }
    if (mbBlendModeAdd) ofDisableBlendMode();
    ofPopMatrix();
}

//--------------------------------------------------------------
void vizDots::keyPressed(int key) {
    ofLogNotice() << mVizIdName << ":: keyPressed:" << key << " _id: " << _id <<endl;
    if ( _id == key-49){
        ofLogNotice() << mVizIdName << ":: gui->toggleVisible();" << endl;
        gui->toggleVisible();
    }
}

//--------------------------------------------------------------
void vizDots::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogNotice() << "got event from: " << name << endl;
	
	if(name == "SAMPLER"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        color =  sampler->getColor();
    }
    
	//ofLogNotice() << mVizIdName << ":: got event from: " << name << endl;
    
    char selectedPreset [32];
    char selectedPresetPath [32];
    sprintf (selectedPreset, "%s%i", mVizName, int(mSelectedPreset));
    sprintf(selectedPresetPath, "GUI/%s.xml", selectedPreset);
    
    char selectedWeightPreset [32];
    char selectedWeightPresetPath [32];
    sprintf (selectedWeightPreset, "%s%i", mVizIdWeightName, int(mSelectedPreset));
    sprintf(selectedWeightPresetPath, "GUI/%s.xml", selectedWeightPreset);
    
    
	if(name == "load"){
        ofLogNotice() << mVizIdName << "guiEvent:: loadSettings:" << selectedPresetPath << endl;
        gui->loadSettings(selectedPresetPath);

        
    } else if(name == "save"){
        
        ofLogNotice() << mVizIdName << "guiEvent:: Save Settings:" << selectedPresetPath << endl;
        gui->saveSettings(selectedPresetPath);

    } else if (name == "grid width"){
        if(mbToggleConstrain) {
            gridHeight = gridWidth/mGridRatio;
        }
    }else if (name == "grid height"){
        if(mbToggleConstrain) {
            gridWidth = gridHeight*mGridRatio;
        }

    }
    else if (name == "reset") {
        gridHeight = gridWidth = 1280;
        mGridRatio = 1.0;
        
    } else if (name == "constrain") {
        mGridRatio = gridWidth/gridHeight;
    } else if (name == "line offset"){
        

    }
}
//--------------------------------------------------------------
void vizDots::exit() {
    ofLogNotice() << "vizDots::exit()" << endl;
    
    gui->saveSettings(mXmlPath);
    delete gui;
}

