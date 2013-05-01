#include "testApp.h"

#define GUI_WIDTH 180
#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#define MAIN_SCREEN_WIDTH 1680
#define MAIN_SCREEN_HEIGHT 1050
//--------------------------------------------------------------
void testApp::setup(){
     // - - - - - - - - - - OF VARS - - - - - - - - -  - - - - -
    ofEnableSmoothing();
    ofSetCircleResolution(60);
    ofSetVerticalSync(false);   // put this on gui or keypress
    ofSetFrameRate(50); // put this on gui or keypress (25 or 50)
    //ofToggleFullscreen();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // - - - - - - - - - - TWEENZOR SETUP - - - - - - - - -  - - - - -
	Tweenzor::init();
    // - - - - - - - - - - TUIO SETUP - - - - - - - - -  - - - - -
    tuioClient.start(3333);
    ofAddListener(tuioClient.cursorAdded,this,&testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::tuioUpdated);
    
        
    // - - - - - - - - - - OFX UI SETUP - - - - - - - - -  - - - - -
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = GUI_WIDTH-xInit;
   	float dim = 16;
    
    ofColor cb = ofColor(0,0,0);
    ofColor co = ofColor(84,0,255);
    ofColor coh = ofColor(0,0,255);
    ofColor cf = ofColor(255, 255,255);
    ofColor cfh = ofColor(128,255,128);
    ofColor cp = ofColor(0,0,255);
    ofColor cpo = ofColor(124,0,255);
    
    gui = new ofxUICanvas(1280, 0, length+xInit, 1080);
    
    gui->setFont("GUI/CarroisGothic-Regular.ttf");                     //This loads a new font and sets the GUI font
    gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
    gui->setFontSize(OFX_UI_FONT_LARGE, 12); //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui->setFontSize(OFX_UI_FONT_SMALL, 6); //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
    gui->addWidgetDown(new ofxUILabel("MLF", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUILabel("Footlocker v4", OFX_UI_FONT_SMALL));
    gui->addSpacer(length-xInit, 1);
    
   gui->addSlider("TUIO offsetX", -200.0, 200.0, &tuioOffsetX, length-xInit,dim);
    gui->addSlider("TUIO offsetY", -200.0, 200.0, &tuioOffsetY, length-xInit,dim);
    
    //gui->addSlider("mXOffset", -500, 500, &mXOffset, length-xInit,dim);
    //gui->addSlider("TUIO scaleY", .5, 2.0, &tuioScaleY, length-xInit,dim);
    //gui->addButton("reset TUIO offset", false, dim, dim);
    
    // - - - - - -  - - - - APP CONTROL - - - - - - - - - - - - - -
    gui->addToggle("toggle TUIO", &bUpdateTUIO, dim, dim);
    gui->addToggle("toggle Cursor", &mbShowCursor, dim, dim);
    
    gui->addWidgetDown(new ofxUILabel("visualisations", OFX_UI_FONT_SMALL));
    gui->addToggle("toggle Needles", &mbToggleNeedles, dim, dim);
    gui->addToggle("toggle Dots", &mbToggleDots, dim, dim);
    gui->addToggle("toggle Physics", &mbTogglePhysics, dim, dim);
    gui->addToggle("toggle Converse", &mbToggleConverse, dim, dim);
    gui->addToggle("toggle MoviePlayer", &mbToggleMoviePlayer, dim, dim);
    
    gui->addToggle("toggle 1440", &mbToggle1440, dim, dim);
    
    gui->addToggle("Show Main Video", &mbMainVideo, dim, dim);
    
    movieSlider =  gui->addSlider("scrub", 0, mMovieLength, &selectedFrame, length-xInit,dim);
    movieSlider->setLabelPrecision(0);
    gui->addButton("prev range", false, dim, dim);
    gui->addButton("next range", false, dim, dim);
    gui->addButton("play", false, dim*2, dim*2);
    gui->addButton("pause", false, dim*2, dim*2);
    
    //  - - - - - - - - - - FPS DISPLAY - - - - - - - - - - - - - - - - -
    vector<float> buffer;
    for(int i = 0; i < 256; i++)
    {
        buffer.push_back(0.0);
    }
    
    
    
    
    //- - - - - - - - - - Visualisations - - - - - - - - - - - - - - - - -
    compassNeedles = new vizCompassNeedles();
    compassNeedles->setup();
    
    dots = new vizDots();
    dots->setup();
    
    physicsViz = new vizPhysics();
    physicsViz->setup();
    
    converseViz = new vizConverse();
    converseViz->setup();
    
    moviePlayerViz = new vizMoviePlayer();
    moviePlayerViz->setup();
    

    mainVideo.setPixelFormat(OF_PIXELS_RGBA);
    mainVideo.loadMovie("master overlay/Square_Output_Master_Edit_001.mov");
    mainVideo.setLoopState(OF_LOOP_NONE);
    
    // - - - - - - MOVIE GRABBING - - - - - - -
    //sprintf(mCapturePath, "capturing/take_%i", int(mTakeNum));
    
    // print input ports to console
	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well
	
	// open port by number (you may need to change this)
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	
	// add testApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(true);
    
    
    // - - - - - - SCENE RUNNER
    // 1:32 = 92* 25 = 2300
   
    gui->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_SMALL));
    mg = (ofxUIMovingGraph *) gui->addWidgetDown(new ofxUIMovingGraph(length-xInit, 120, buffer, 256, 0, 400, "MOVING GRAPH"));
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
    
    
    doShowColorbars = false;
    imageColorBars.loadImage("ColorBars.tif");

    
    
}
//------------------------------------------------------------
bool testApp::setMode(ModeName newMode) {
    if(mCurrentMode != newMode) {
      
        ofLogNotice() << " MODE SWITCH: mCurrentMode: " << mCurrentMode << " " << " new Mode: " << newMode << endl;
        mCurrentMode = newMode;
        switch(mCurrentMode) {
            case kModeNikeBeats:
                //movieplayer - load movie grid;
               
                mbToggleMoviePlayer = true;
                
                mbToggleDots = false;
                mbToggleConverse = false;
                mbToggleNeedles = false;
                mbTogglePhysics = false;
                
                break;
            case kModeNikeTune:
                //movieplayer + physics - load yellow buttons
                 physicsViz->loadSettings(10);
                mbToggleMoviePlayer = true;
                mbTogglePhysics = true;
                
                mbToggleDots = false;
                mbToggleConverse = false;
                mbToggleNeedles = false;
                
                break;
            case kModeConverseTune:
                //movieplayer + physics - load red buttons
                physicsViz->loadSettings(10);
                mbTogglePhysics = true;
                mbToggleMoviePlayer = true;
                
                mbToggleConverse = false;
                mbToggleDots = false;
                mbToggleNeedles = false;
                
                break;
                
                
            case kModeConverseFX:
                
                //load circular setting
                mbToggleConverse = true;
                converseViz->loadSettings(3);
                
                mbToggleDots = false;
                mbToggleMoviePlayer = false;
                mbToggleNeedles = false;
                mbTogglePhysics = false;
                break;
                
            case kModeAdidasBeats:
                //movieplayer - load movie grid
                
                
                mbToggleMoviePlayer = true;
                
                mbToggleDots = false;
                mbToggleConverse = false;
                mbToggleNeedles = false;
                mbTogglePhysics = false;
                
                break;
            case kModeAdidasFX:
                //stripey physics
                //load setting, change texture
                physicsViz->loadSettings(1);
                mbTogglePhysics = true;
                
                mbToggleMoviePlayer = false;
                mbToggleDots = false;
                mbToggleConverse = false;
                mbToggleNeedles = false;
                
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

//--------------------------------------------------------------


void testApp::update(){
    //weenzor::update( ofGetElapsedTimeMillis() );
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    // - - - - - - - - -
    /*
     kModeNikeBeats,
     kModeNikeTune,
     kModeConverseTune,
     kModeConverseFX,
     kModeAdidasBeats,
     kModeAdidasFX
     */
    if(mbMainVideo) {
    mCurrentFrame = mainVideo.getCurrentFrame();
    if (mainVideo.isPlaying())   movieSlider->setValue(mCurrentFrame);
    
    if     (ofInRange(mCurrentFrame, mRanges[1], mRanges[2])) setMode(kModeNikeBeats);
    else if(ofInRange(mCurrentFrame, mRanges[2], mRanges[3])) setMode(kModeNikeTune);
    else if(ofInRange(mCurrentFrame, mRanges[3], mRanges[4])) setMode(kModeConverseTune);
    else if(ofInRange(mCurrentFrame, mRanges[4], mRanges[5])) setMode(kModeConverseFX);
    else if(ofInRange(mCurrentFrame, mRanges[5], mRanges[6])) setMode(kModeAdidasBeats);
    else if(ofInRange(mCurrentFrame, mRanges[6], mRanges[7])) setMode(kModeAdidasFX);
    }
    
    
    
    //  - - - - - - - - - - OFX UI - - - - - - - - - - - - - - - - -
    mg->addPoint(ofGetFrameRate());
        
    //  - - - - - - - - - - OFX TUIO - - - - - - - - - - - - - - - - -
    ofVec3f target;
    list<ofxTuioCursor*> l;
    if (bUpdateTUIO ) {
        tuioClient.getMessage();
        l = tuioClient.getTuioCursors();
        if (l.empty()){
            target = ofVec3f(1280/2, 1080/2, 0);
            if (mbToggleNeedles) compassNeedles->setIdle(true);
        } else {
            if (mbToggleNeedles) compassNeedles->setIdle(false);
            // get first cursor
            ofxTuioCursor* cursor = getFirstCursor();
            target = ofVec3f(cursor->getX()*OUTPUT_WIDTH, cursor->getY()*OUTPUT_HEIGHT,0.0f);
            ofLogNotice() << "cursor->getX()*OUTPUT_WIDTH: " << cursor->getX()*OUTPUT_WIDTH << endl;
            
        }
    } else if (!bUpdateTUIO){
        // if no TUIO available, switch to mouse
        target = ofVec3f(mouseX, mouseY, 0);
    }
    
    //  - - - - - - - - - - Visualisations- - - - - - - - - - - - - - - - - 
    if (mbToggleNeedles){
        if (bUpdateTUIO) {
            compassNeedles->update(l);
        } else {
            compassNeedles->update(target);
        }
        //compassNeedles->update();
    }
    
    if (mbToggleDots){
        if (bUpdateTUIO) {
            dots->update(l);
        } else {
            dots->update(target);
        }
    }
    if(mbTogglePhysics) {
     
        if (bUpdateTUIO) {
            physicsViz->update(l);
        } else {
            physicsViz->update(target);
        }
        physicsViz->update();
    }
    if(mbToggleConverse) {
        
        if (bUpdateTUIO) {
            converseViz->update(l);
        } else {
            converseViz->update(target);
        }
        converseViz->update();
    }
    if(mbToggleMoviePlayer) {
        if (bUpdateTUIO) {
            moviePlayerViz->update(l);
        } else {
            moviePlayerViz->update(target);
        }
        moviePlayerViz -> update();
    }
//    if(mbToggleCapture){
//        img.grabScreen(MAIN_SCREEN_WIDTH, 0, OUTPUT_WIDTH, OUTPUT_HEIGHT);
//    }
     if(mbTogglePhysics && mbToggleConverse) {
        // physicsViz->setTextureFbo(converseViz->rgbaFbo);
     }
    

    if(mbMainVideo) mainVideo.update();
}
//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    //  - - - - - - - - - - Visualisations- - - - - - - - - - - - - - - - -
    glPushMatrix();
    bool b = ofGetWindowMode() == OF_FULLSCREEN;
    if (b) {
        if(mbToggle1440) {
            ofTranslate(0, 0);
        } else {
            ofTranslate(MAIN_SCREEN_WIDTH, 0);
        }
    }
  
    if(mbToggleNeedles) ofTranslate(tuioOffsetX,tuioOffsetY);
    if(mbToggleNeedles) compassNeedles->draw();
    if(mbToggleConverse) converseViz->draw();
    if(mbTogglePhysics) physicsViz->draw();
    if(mbToggleMoviePlayer) moviePlayerViz->draw();
    if(mbToggleDots) dots->draw();
   
//    if(mbShowCursor) {
//        ofPushStyle();
//        ofSetColor(255);
//        ofSphere(mouseX, 0, mouseY, 4);
//        ofPopStyle();
//    }
//    if (mouseX > MAIN_SCREEN_WIDTH){
//        ofHideCursor();
//    } else {
//        ofShowCursor();
//    }
    
    // TODO: add gui options to disable etc
    // TODO: add keyboard shortcuts to rewind, pause, play video, jump forward or backwawrd by 1 second
    // TODO: display video time in gui
    ofSetColor(255, 255, 255);
    ofEnableAlphaBlending();
    if (mbMainVideo) {
        mainVideo.draw(0, 0);
    }
    
    if(doShowColorbars) {
        ofDisableAlphaBlending();
        ofSetColor(255, 255, 255);
        imageColorBars.draw(0, 0);
    }

    glPopMatrix();
    
    ofSetColor(255);
    //cout << "midiMessage.value: " << midiMessage.value << endl;
   // draw midi
    //ofRect(ofGetHeight()-20, 20, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
}
//--------------------------------------------------------------
ofxTuioCursor* testApp::getFirstCursor(){
    list<ofxTuioCursor*> listC = tuioClient.getTuioCursors();
    return listC.front();
}
//--------------------------------------------------------------
void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}
//--------------------------------------------------------------
void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}
//--------------------------------------------------------------
void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
     cout << "testApp::keyPressed key: " << key << endl;
    switch(key) {
        case 'c':
            doShowColorbars ^= true;
            break;
            
        case ',':
        case '<':
            mainVideo.setFrame(mainVideo.getCurrentFrame()-25);
            break;
            
        case '.':
        case '>':
            mainVideo.setFrame(mainVideo.getCurrentFrame()+25);
            break;
            
        case 'w':
            mainVideo.setFrame(0);
            break;
            
    }
    
    if (key == 9) gui->toggleVisible();
    if (key == 102) ofToggleFullscreen();
    if (key == 109) mbShowCursor = !mbShowCursor;
    if (key == 32) {
        if(mainVideo.isPlaying()) {
            mainVideo.stop();
        } else {
            mainVideo.play();
        }
        
    }
    // left, right 356, 358
    if (mbToggleNeedles) compassNeedles -> keyPressed(key);
    if(mbToggleDots) dots -> keyPressed(key);
    if(mbTogglePhysics) physicsViz ->keyPressed(key);
    if(mbToggleConverse) converseViz -> keyPressed(key);
    if(mbToggleMoviePlayer) moviePlayerViz -> keyPressed(key);
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
    cout << "testApp::keyReleased key: " << key << endl;
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    mouseX  = x;
    mouseY = y;
    if(mbTogglePhysics) physicsViz ->mouseMoved(x, y);
    if(mbToggleConverse) converseViz -> mouseMoved(x,y);
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(mbTogglePhysics) physicsViz ->mouseDragged(x, y, button);
}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
   
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if(mbTogglePhysics) physicsViz->mouseReleased(x, y, button);
    if(mbToggleConverse) converseViz->mouseReleased(x, y, button);
}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogNotice() << "got event from: " << name << endl;
	
	if(name == "SAMPLER"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mBgColor =  sampler->getColor();
    } else if (name == "toggle Mouse"){
        mbShowCursor = !mbShowCursor;
    } else if (name == "reset TUIO offset"){
        
        tuioOffsetY = tuioOffsetX = 0;
    } else if (name == "prev range"){
       
    } else if (name == "next range"){
        
    } else if (name == "scrub"){
        if(mbMainVideo) {
            mainVideo.stop();
            
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            ofLogNotice() << "SCRUBBING: Frame: " << int(slider->getScaledValue()) << endl;
            
            mainVideo.setFrame(int(slider->getScaledValue()));
        }
        
    } else if (name == "pause"){
        mainVideo.stop();
    } else if (name == "play"){
        mainVideo.play();
    }
        
}
//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
    converseViz->updateMidi(msg);
}
//--------------------------------------------------------------
void testApp::exit() {
    ofLogNotice() << "testApp::exit()" << endl;
    compassNeedles -> exit();
    dots -> exit();
    
    converseViz ->exit();
    physicsViz->exit();
    moviePlayerViz->exit();
    gui->saveSettings("GUI/guiSettings.xml");
    ofLogNotice() << "testApp::exit() XML settings saved." << endl;
    // clean up
	midiIn.closePort();
	midiIn.removeListener(this);
    delete gui;
    delete[] buffer;
    Tweenzor::destroy();
    ofExit();
}