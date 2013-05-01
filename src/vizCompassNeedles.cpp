//
//  vizCompassNeedles.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//

#include "vizCompassNeedles.h"

#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#define MAIN_SCREEN_WIDTH 1680
#define MAIN_SCREEN_HEIGHT 1080
#define DECK_WIDTH 1280
#define DECK_HEIGHT 1080

void vizCompassNeedles::setup(){
    
    _id = 0;
    smVizCount = 10;
    mPresetCount = 10;
    mSelectedPreset = 0;
    
    sprintf(mVizName, "vizCompassNeedles");
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

    gui = new ofxUICanvas(200, 0, length+xInit, 1080);

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

    //  - - - - - - - - - - NEEDLES - - - - - - - - - - - - - - - - -
    gui->addWidgetDown(new ofxUILabel("needles", OFX_UI_FONT_LARGE));
    
    gui->addToggle("toggle idle", &mbToggleIdle, dim, dim);
    gui->addSlider("idle rotation", 0.0, 360.0, &mIdleRotation, length-xInit,dim);
    gui->addToggle("blend add", &mbBlendModeAdd, dim, dim);
    gui->addSlider("alpha", 0.0, 255.0f, &mAlpha, length-xInit,dim);
    gui->addSlider("needle width", 1.0, 300.0f, &needleWidth, length-xInit,dim);
    gui->addSlider("needle height", 1.0, 200.0f, &needleHeight, length-xInit,dim);
    gui->addSlider("needle offset x", -50.0, 50.0f, &mNeedleOffset, length-xInit,dim);
    gui->addSlider("ease", 1.0f, 50.0f, &mEase, length-xInit,dim);
    gui->addSlider("mRange", 50.0, 800.0, &mRange, length-xInit,dim);
    gui->addToggle("constrain", &mbToggleConstrain, dim, dim);
    gui->addSlider("grid width", 400.0, 4000.0, &gridWidth, length-xInit,dim);
    gui->addSlider("grid height", 400.0, 4000.0, &gridHeight, length-xInit,dim);
    gui->addButton("reset", false, dim, dim);
    gui->addSlider("line offset",-40.0, 40.0, &mLineOffset, length-xInit,dim);

    
    gui->addWidgetDown(new ofxUILabel("color", OFX_UI_FONT_SMALL));
    m_imgGradient.allocate(162,81, OF_IMAGE_COLOR);
    m_imgGradient.loadImage("img/gradient_162x81.png");
    gui->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "color"));
    
    gui->addWidgetDown(new ofxUILabel("bg color", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "bg color"));
    
    ofAddListener(gui->newGUIEvent,this,&vizCompassNeedles::guiEvent);
    gui->loadSettings(mXmlPath);
    gui->toggleVisible();
    gridWidth = ofGetScreenWidth();
    gridHeight = ofGetScreenHeight();
    
    
    
    //TODO make grid res flexible
    gridResX = 50;
    gridResY = 50;
    mbIdle = false;
    buildGrid();
}
//--------------------------------------------------------------
void vizCompassNeedles::buildGrid() {
    particles.clear();
    // - - - - - - BUILD GRID - - - - -
    int counter = 0;
    for (int i = 0; i<= gridResX; i++){
        for (int j = 0; j<= gridResX; j++){
            Particle* p = new Particle();
            // add line offset every second row
            float offsetX =  j%2?mLineOffset:0.0;
            p->setHome(ofVec3f ( gridWidth/gridResX*i+offsetX, gridHeight/gridResY*j, 0.0));
            p->setup(counter);
            particles.push_back(p);
            counter ++;
        }
    }

}
//--------------------------------------------------------------
void vizCompassNeedles::update(ofVec3f target) {
    //gives better feeback when selecting presets, does not change the actual value though
    mSelectedPreset = int(mSelectedPreset);
    list<Particle*>::iterator tit;
    for (tit=particles.begin(); tit != particles.end(); tit++) {
        Particle *particle = (*tit);
        particle->setTarget(target);
        particle->setColor(ofColor(color.r, color.g, color.g, mAlpha));
        particle->setWidth(needleWidth);
        particle->setHeight(needleHeight);
        particle->setIdle(mbIdle);
        particle->setIdleRotation(mIdleRotation);
        particle->setNeedleOffset(mNeedleOffset);
        particle->setEase(mEase);
        particle->update();
    }

    
}
//--------------------------------------------------------------
void vizCompassNeedles::update( list<ofxTuioCursor*> cursors) {
    //ofLogNotice() << "vizCompassNeedles::update( list<ofxTuioCursor*> c)" <<endl;
   
    list<Particle*>::iterator tit;
    for (tit=particles.begin(); tit != particles.end(); tit++) {
        Particle *particle = (*tit);
        
        float distBefore;
        ofVec3f target;
        list<ofxTuioCursor*>::iterator tit;
        for (tit=cursors.begin(); tit != cursors.end(); tit++) {
            ofxTuioCursor *cursor = (*tit);
            float x = cursor->getX()*OUTPUT_WIDTH;
            float y = -cursor->getY()*OUTPUT_HEIGHT*(-1);
            ofVec3f cVec = ofVec3f(x, y, 0);
            float distance = (particle->home - cVec).length();
            //ofLogNotice() << "vizCompassNeedles::update( list<ofxTuioCursor*> c): distance: " << distance <<endl;
            if( distance < mRange && distance < distBefore){
                //ofLogNotice() << "vizCompassNeedles::update( list<ofxTuioCursor*> c): Found close Tuio Cursor" <<endl;
                target = cVec;
                particle->setStrength(1.0-distance/mRange);
            } else {
                particle->setIdle(mbIdle);
                particle->setStrength(0);
            }
             distBefore =  distance;
        }
        
        particle->setTarget(target);
        particle->setColor(ofColor(color.r, color.g, color.g, mAlpha));
        particle->setWidth(needleWidth);
        particle->setHeight(needleHeight);
        if (target == ofVec3f(0,0,0)){
            particle->setIdle(mbIdle);
        }
        particle->setIdleRotation(mIdleRotation);
        particle->setNeedleOffset(mNeedleOffset);
        particle->setEase(mEase);
        particle->update();
    }

}
//--------------------------------------------------------------
void vizCompassNeedles::draw(){
    ofPushMatrix();
    ofPushStyle();
    glDisable(GL_DEPTH_TEST);

    ofSetColor(mBgColor);
    ofRect(0,0,DECK_WIDTH, DECK_HEIGHT );
    //ofScale(scaleX, scaleY);
    if (mbBlendModeAdd) ofEnableBlendMode(OF_BLENDMODE_ADD);
    list<Particle*>::iterator tit;
    for (tit=particles.begin(); tit != particles.end(); tit++) {
        Particle *particle = (*tit);
        particle->draw();
    }
    if (mbBlendModeAdd) ofDisableBlendMode();
    ofPopMatrix();
}
//--------------------------------------------------------------
void vizCompassNeedles::keyPressed(int key) {
    ofLogNotice() << mVizIdName << ":: keyPressed:" << key << " _id: " << _id <<endl;
    if ( _id == key-49){
            ofLogNotice() << mVizIdName << ":: gui->toggleVisible();" << endl;
            gui->toggleVisible();
           // gui2->toggleVisible();
    }
}
//--------------------------------------------------------------
void vizCompassNeedles::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogNotice() << "got event from: " << name << endl;
	
	
	//ofLogNotice() << mVizIdName << ":: got event from: " << name << endl;
   
        char selectedPreset [32];
        char selectedPresetPath [32];
        sprintf (selectedPreset, "%s%i", mVizName, int(mSelectedPreset));
        sprintf(selectedPresetPath, "GUI/%s.xml", selectedPreset);
        
        char selectedWeightPreset [32];
        char selectedWeightPresetPath [32];
        sprintf (selectedWeightPreset, "%s%i", mVizIdWeightName, int(mSelectedPreset));
        sprintf(selectedWeightPresetPath, "GUI/%s.xml", selectedWeightPreset);
    
    
	if(name == "color"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        color =  sampler->getColor();
    } else if(name == "bg color"){
        ofxUIImageSampler *sampler2 = (ofxUIImageSampler *) e.widget;
        mBgColor =  sampler2->getColor();
        
    } else if(name == "load"){
        ofLogNotice() << mVizIdName << "guiEvent:: loadSettings:" << selectedPresetPath << endl;
        gui->loadSettings(selectedPresetPath);
        // gui2->loadSettings(selectedWeightPresetPath);
        
    } else if(name == "save"){
        
        ofLogNotice() << mVizIdName << "guiEvent:: Save Settings:" << selectedPresetPath << endl;
        gui->saveSettings(selectedPresetPath);
        //ofLogNotice() << mVizIdName << "guiEvent:: Save Weight Settings:" << selectedWeightPresetPath << endl;
        //gui2->saveSettings(selectedWeightPresetPath);
    } else if (name == "grid width"){
        if(mbToggleConstrain) {
            gridHeight = gridWidth/mGridRatio;
        }
        buildGrid();
    }else if (name == "grid height"){
        if(mbToggleConstrain) {
            gridWidth = gridHeight*mGridRatio;
        }
        buildGrid();
    }
    else if (name == "reset") {
        gridHeight = gridWidth = 1280;
        mGridRatio = 1.0;
        buildGrid();
    } else if (name == "constrain") {
        mGridRatio = gridWidth/gridHeight;
    } else if (name == "line offset"){
        
        buildGrid();
    } else if (name == "toggle idle"){
    
        mbIdle = mbToggleIdle;
    }
}
//--------------------------------------------------------------
void vizCompassNeedles::exit() {
    ofLogNotice() << "vizCompassNeedles::exit()" << endl;
    
    gui->saveSettings(mXmlPath);
    delete gui;
}

void vizCompassNeedles::setIdle(bool b){
    if (mbToggleIdle) mbIdle = b;

}