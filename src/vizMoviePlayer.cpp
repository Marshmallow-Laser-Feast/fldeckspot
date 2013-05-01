//
//  vizMoviePlayer.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 27/04/2013.
//
//

#include "vizMoviePlayer.h"
#include "testApp.h"



//
//  vizMoviePlayer.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//
#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#define MAIN_SCREEN_WIDTH 1680
#define MAIN_SCREEN_HEIGHT 1080
#define DECK_WIDTH 1280
#define DECK_HEIGHT 1080

#include "vizMoviePlayer.h"

void vizMoviePlayer::setup(){
    _id = 5;
    smVizCount = 10;
    mPresetCount = 10;
    mSelectedPreset = 0;
    
    sprintf(mVizName, "vizMoviePlayer");
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
    
    //  - - - - - - - - - - vizMoviePlayer - - - - - - - - - - - - - - - - -
    gui->addWidgetDown(new ofxUILabel("vizMoviePlayer", OFX_UI_FONT_LARGE));
    
    
    gui->addToggle("blend add", &mbBlendModeAdd, dim, dim);
    gui->addSlider("width", 1.0, 300.0f, &videoWidth, length-xInit,dim);
    gui->addSlider("mReactionRadius", 1.0, 400.0f, &mReactionRadius, length-xInit,dim);
    gui->addSlider("mMovieSpeed", 1.0, 5.0f, &mMovieSpeed, length-xInit,dim);
    
    gui->addButton("Nike BEATS", false, dim, dim);
    gui->addButton("Nike TUNE", false, dim, dim);
    gui->addButton("Converse TUNE", false, dim, dim);
    gui->addButton("Adidas BEATS", false, dim, dim);
    
    gui->addWidgetDown(new ofxUILabel("bg color", OFX_UI_FONT_SMALL));
    
    m_imgGradient.allocate(162,81, OF_IMAGE_COLOR);
    m_imgGradient.loadImage("img/gradient_162x81.png");
    
    gui->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "SAMPLER"));
    gui->toggleVisible();
    
    ofAddListener(gui->newGUIEvent,this,&vizMoviePlayer::guiEvent);
    gui->loadSettings(mXmlPath);
    
     //load Videos
    nikeTuneDir = ofDirectory("nike_tune/");
    nikeBeatsDir = ofDirectory("nike_beats/");
    converseTuneDir = ofDirectory("converse_tune");
    adidasBeatsDir= ofDirectory("adidas_beats");
   

    nikeTuneDir.allowExt("mov");
    nikeBeatsDir.allowExt("mov");
    converseTuneDir.allowExt("mov");
    adidasBeatsDir.allowExt("mov");
    
    nikeTuneDir.listDir();
    nikeBeatsDir.listDir();
    converseTuneDir.listDir();
    adidasBeatsDir.listDir();
    
    
   
    mNumVideos = nikeTuneDir.numFiles();

    mTunePositions.push_back(make_pair(640, 270));
    mTunePositions.push_back(make_pair(640, 800));
    mTunePositions.push_back(make_pair(800, 123));
    mTunePositions.push_back(make_pair(467, 123));
    mTunePositions.push_back(make_pair(1016, 936));
    mTunePositions.push_back(make_pair(226, 936));
    mTunePositions.push_back(make_pair(226, 144));
    mTunePositions.push_back(make_pair(1022, 144));
    mTunePositions.push_back(make_pair(188, 384));
    mTunePositions.push_back(make_pair(1050, 384));
    mTunePositions.push_back(make_pair(1050, 694));
    mTunePositions.push_back(make_pair(190, 694));
    mTunePositions.push_back(make_pair(466, 961));
    mTunePositions.push_back(make_pair(800, 961));
    mTunePositions.push_back(make_pair(367, 540));
    mTunePositions.push_back(make_pair(900, 540));
    mTunePositions.push_back(make_pair(849, 747));
    mTunePositions.push_back(make_pair(442, 747));
    mTunePositions.push_back(make_pair(442, 328));
    mTunePositions.push_back(make_pair(834, 328));
    
    int w = 320;
    int h = 360;
    for (int j = 0; j<3; j++){
        for (int i = 0; i<4; i++){
            int x = i*w + w/2;
            int y = j*h + h/2;
            mBeatsPositions.push_back(make_pair(x, y));
        }
    }
   

    for(int i = 0; i <mBeatsPositions.size(); i++){
        Dot* dot = new Dot();
        dot->setup(i);
        dot->setHome(ofVec3f(mBeatsPositions.at(i).first, mBeatsPositions.at(i).second, 0));
        dot->setVideo( nikeBeatsDir.getPath(i));
        //dot->pauseVideo();
        nikeBeatsVideos.push_back(dot);
    }
        
    for(int i = 0; i <mBeatsPositions.size(); i++){
        Dot* dot = new Dot();
        dot->setup(i);
        dot->setHome(ofVec3f(mBeatsPositions.at(i).first, mBeatsPositions.at(i).second, 0));
        dot->setVideo( adidasBeatsDir.getPath(i));
        //dot->pauseVideo();
        adidasBeatsVideos.push_back(dot);
    }
    
    for(int i = 0; i <mTunePositions.size(); i++){
        Dot* dot = new Dot();
        dot->setup(i);
        dot->setHome(ofVec3f(mTunePositions.at(i).first, mTunePositions.at(i).second, 0));
        dot->setVideo( nikeTuneDir.getPath(int(ofRandom(mNumVideos))));
        //dot->pauseVideo();
        nikeTuneVideos.push_back(dot);
    }
        
    for(int i = 0; i <mTunePositions.size(); i++){
        Dot* dot = new Dot();
        dot->setup(i);
        dot->setHome(ofVec3f(mTunePositions.at(i).first, mTunePositions.at(i).second, 0));
        dot->setVideo( converseTuneDir.getPath(int(ofRandom(mNumVideos))));
        //dot->pauseVideo();
        converseTuneVideos.push_back(dot);
    }
    

    mOverlayMovie =  new ofVideoPlayer();
    mOverlayMovie->setPixelFormat(OF_PIXELS_RGBA);
   // mOverlayMovie->setLoopState(<#ofLoopType state#>)
    mOverlayMovie->loadMovie("overlay/Nike_Tune_Centre_001_loop.mov");
    mOverlayMovie->play();
    
    
}


//--------------------------------------------------------------
void vizMoviePlayer::update() {
    mSelectedPreset = int(mSelectedPreset);
    
    if (mOverlayMovie->isLoaded()) mOverlayMovie->update();
    
    videoHeight = videoWidth;

    testApp* myApp = (testApp*)ofGetAppPtr();
    switch(myApp->mCurrentMode) {
        case testApp::kModeNikeBeats:
        {
            list<Dot*>::iterator tit;
            
            for (tit=nikeBeatsVideos.begin(); tit != nikeBeatsVideos.end(); tit++) {
                Dot* video = (*tit);
                video->update();
            }
        }
            
            break;
        case testApp::kModeNikeTune:
        {
            list<Dot*>::iterator tit2;
            for (tit2=nikeTuneVideos.begin(); tit2 != nikeTuneVideos.end(); tit2++) {
                Dot* video = (*tit2);
                video->update();
            }
        }
            
            break;
            
            
        case testApp::kModeConverseFX:
            
            break;
            
        case testApp::kModeAdidasBeats:
        {
            list<Dot*>::iterator tit3;
            for (tit3=adidasBeatsVideos.begin(); tit3 != adidasBeatsVideos.end(); tit3++) {
                Dot* video = (*tit3);
                video->update();
            }
        }
            
            break;
        case testApp::kModeConverseTune:
        {
            list<Dot*>::iterator tit4;
            for (tit4=converseTuneVideos.begin(); tit4 != converseTuneVideos.end(); tit4++) {
                Dot* video = (*tit4);
                video->update();
            }
        }
            
        case testApp::kModeAdidasFX:
            
            break;
        default:
            break;
    }}
    
//--------------------------------------------------------------
void vizMoviePlayer::update( list<ofxTuioCursor*> cursors) {
   
    ofVec3f mouse = ofVec3f(mouseX, mouseY, 0);    
    testApp* myApp = (testApp*)ofGetAppPtr();
    switch(myApp->mCurrentMode) {
        case testApp::kModeNikeBeats:
        {
            int counter = 0;
            bool trigger;
            float dist, x, y;
            list<Dot*>::iterator tit0;
            
            for (tit0=nikeBeatsVideos.begin(); tit0 != nikeBeatsVideos.end(); tit0++) {
                trigger = false;
                Dot* video = (*tit0);
                ofVec3f moviePos = ofVec3f(mBeatsPositions.at(counter).first,mBeatsPositions.at(counter).second, 0);
                
                list<ofxTuioCursor*>::iterator tit;
                for (tit=cursors.begin(); tit != cursors.end(); tit++) {
                    ofxTuioCursor* cursor = (*tit);
                    x = cursor->getX()*OUTPUT_WIDTH;
                    y = cursor->getY()*OUTPUT_HEIGHT;
                    ofVec3f TCVec = ofVec3f(x,y,0);
                    
                    
                    dist = (TCVec-moviePos).length();
                    if (dist < mReactionRadius) {
                        trigger= true;
                    }
                    
                }
                
                if(trigger) video->playVideo();
                
                counter++;

            }
        }
            
            break;
        case testApp::kModeNikeTune:
        {
            int counter = 0;
            bool trigger;
            float dist, x, y;
            list<Dot*>::iterator tit2;
            for (tit2=nikeTuneVideos.begin(); tit2 != nikeTuneVideos.end(); tit2++) {
                trigger = false;
                Dot* video = (*tit2);
                ofVec3f moviePos = ofVec3f(mTunePositions.at(counter).first,mTunePositions.at(counter).second, 0);
                
                list<ofxTuioCursor*>::iterator tit;
                for (tit=cursors.begin(); tit != cursors.end(); tit++) {
                    ofxTuioCursor* cursor = (*tit);
                    x = cursor->getX()*OUTPUT_WIDTH;
                    y = cursor->getY()*OUTPUT_HEIGHT;
                    ofVec3f TCVec = ofVec3f(x,y,0);
                    
                    
                    dist = (TCVec-moviePos).length();
                    if (dist < mReactionRadius) {
                        trigger= true;
                    }
                    
                }
                
                if(trigger) video->playVideo();
                
                counter++;

            }
        }
            
            break;
            
            
        case testApp::kModeConverseFX:
            
            break;
            
        case testApp::kModeAdidasBeats:
        {
            int counter = 0;
            bool trigger;
            float dist, x, y;
            list<Dot*>::iterator tit3;
            for (tit3=adidasBeatsVideos.begin(); tit3 != adidasBeatsVideos.end(); tit3++) {
                trigger = false;
                Dot* video = (*tit3);
                ofVec3f moviePos = ofVec3f(mBeatsPositions.at(counter).first,mBeatsPositions.at(counter).second, 0);
                
                list<ofxTuioCursor*>::iterator tit;
                for (tit=cursors.begin(); tit != cursors.end(); tit++) {
                    ofxTuioCursor* cursor = (*tit);
                    x = cursor->getX()*OUTPUT_WIDTH;
                    y = cursor->getY()*OUTPUT_HEIGHT;
                    ofVec3f TCVec = ofVec3f(x,y,0);
                    
                    
                    dist = (TCVec-moviePos).length();
                    if (dist < mReactionRadius) {
                        trigger= true;
                    }
                    
                }
                
                if(trigger) video->playVideo();
                
                counter++;

            }
        }
            
            break;
        case testApp::kModeConverseTune:
        {
            int counter = 0;
            bool trigger;
            float dist, x, y;
            list<Dot*>::iterator tit4;
            for (tit4=converseTuneVideos.begin(); tit4 != converseTuneVideos.end(); tit4++) {
                trigger = false;
                Dot* video = (*tit4);
                ofVec3f moviePos = ofVec3f(mTunePositions.at(counter).first,mTunePositions.at(counter).second, 0);
                
                list<ofxTuioCursor*>::iterator tit;
                for (tit=cursors.begin(); tit != cursors.end(); tit++) {
                    ofxTuioCursor* cursor = (*tit);
                    x = cursor->getX()*OUTPUT_WIDTH;
                    y = cursor->getY()*OUTPUT_HEIGHT;
                    ofVec3f TCVec = ofVec3f(x,y,0);
                    
                    
                    dist = (TCVec-moviePos).length();
                    if (dist < mReactionRadius) {
                        trigger= true;
                    }
                    
                }
                
                if(trigger) video->playVideo();
                
                counter++;

            }
        }
            
        case testApp::kModeAdidasFX:
            
            break;
        default:
            break;
    }

}

//--------------------------------------------------------------
void vizMoviePlayer::update(ofVec3f t) {
    
    int counter = 0;
    bool trigger = false;
    float dist, x, y;

    testApp* myApp = (testApp*)ofGetAppPtr();
    switch(myApp->mCurrentMode) {
        case testApp::kModeNikeBeats:
        {
            list<Dot*>::iterator tit4;
            
            for (tit4=nikeBeatsVideos.begin(); tit4 != nikeBeatsVideos.end(); tit4++) {
                
                Dot* video = (*tit4);
                video->setSpeed(mMovieSpeed);
                
                ofVec3f moviePos = ofVec3f(mBeatsPositions.at(counter).first,mBeatsPositions.at(counter).second, 0);
                dist = (t-moviePos).length();
                if (dist < mReactionRadius) video->playVideo();
                counter++;
                
            }
        }

            break;
        case testApp::kModeNikeTune:
        {
            list<Dot*>::iterator tit4;
            
            for (tit4=nikeTuneVideos.begin(); tit4 != nikeTuneVideos.end(); tit4++) {
                
                Dot* video = (*tit4);
                video->setSpeed(mMovieSpeed);
                
                ofVec3f moviePos = ofVec3f(mTunePositions.at(counter).first,mTunePositions.at(counter).second, 0);
                dist = (t-moviePos).length();
                if (dist < mReactionRadius) video->playVideo();
                counter++;
                
            }
        }

            
            break;
            
            
        case testApp::kModeConverseFX:
            
            break;
            
        case testApp::kModeAdidasBeats:
        
            {
                list<Dot*>::iterator tit4;
                
                for (tit4=adidasBeatsVideos.begin(); tit4 != adidasBeatsVideos.end(); tit4++) {
                    
                    Dot* video = (*tit4);
                    video->setSpeed(mMovieSpeed);
                    
                    ofVec3f moviePos = ofVec3f(mBeatsPositions.at(counter).first,mBeatsPositions.at(counter).second, 0);
                    dist = (t-moviePos).length();
                    if (dist < mReactionRadius) video->playVideo();
                    counter++;
                    
                }
            }

        
            break;
        case testApp::kModeConverseTune:
        { list<Dot*>::iterator tit4;
            
            for (tit4=converseTuneVideos.begin(); tit4 != converseTuneVideos.end(); tit4++) {
                
                Dot* video = (*tit4);
                video->setSpeed(mMovieSpeed);
                
                ofVec3f moviePos = ofVec3f(mTunePositions.at(counter).first,mTunePositions.at(counter).second, 0);
                dist = (t-moviePos).length();
                if (dist < mReactionRadius) video->playVideo();
                counter++;

            }}
            
        case testApp::kModeAdidasFX:
            
            break;
        default:
            break;
    }
    /*
    list<Dot*>::iterator tit;
    for (tit=videos.begin(); tit != videos.end(); tit++) {
        Dot* video = (*tit);
        video->setSpeed(mMovieSpeed);
        
        ofVec3f moviePos = ofVec3f(mTunePositions.at(counter).first,mTunePositions.at(counter).second, 0);
        
        dist = (t-moviePos).length();
        if (dist < mReactionRadius) video->playVideo();
        counter++;
    }*/
}
//--------------------------------------------------------------
void vizMoviePlayer::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,255,255);
    if (mbBlendModeAdd) ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableAlphaBlending();
    
    testApp* myApp = (testApp*)ofGetAppPtr();
    switch(myApp->mCurrentMode) {
        case testApp::kModeNikeBeats:
        {
            list<Dot*>::iterator tit;
        
            for (tit=nikeBeatsVideos.begin(); tit != nikeBeatsVideos.end(); tit++) {
                Dot* video = (*tit);
                video->draw();
            }
        }

            break;
        case testApp::kModeNikeTune:
        {
            list<Dot*>::iterator tit2;
            for (tit2=nikeTuneVideos.begin(); tit2 != nikeTuneVideos.end(); tit2++) {
                Dot* video = (*tit2);
                video->draw();
            }
        }
        
            break;
            
            
        case testApp::kModeConverseFX:
            
            break;
            
        case testApp::kModeAdidasBeats:
        {
            list<Dot*>::iterator tit3;
            for (tit3=adidasBeatsVideos.begin(); tit3 != adidasBeatsVideos.end(); tit3++) {
                Dot* video = (*tit3);
                video->draw();
            }
        }
            
            break;
        case testApp::kModeConverseTune:
            {
                list<Dot*>::iterator tit4;
            for (tit4=converseTuneVideos.begin(); tit4 != converseTuneVideos.end(); tit4++) {
                Dot* video = (*tit4);
                video->draw();
            }
            }
            
        case testApp::kModeAdidasFX:
            
            break;
        default:
            break;
    }

        
    if (mOverlayMovie->isLoaded()) mOverlayMovie -> draw(640-mOverlayMovie->width/2,540-mOverlayMovie->height/2);
    ofDisableAlphaBlending();
   
    if (mbBlendModeAdd) ofDisableBlendMode();
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void vizMoviePlayer::keyPressed(int key) {
    ofLogNotice() << mVizIdName << ":: keyPressed:" << key << " _id: " << _id <<endl;
    if ( _id == key-49){
        ofLogNotice() << mVizIdName << ":: gui->toggleVisible();" << endl;
        gui->toggleVisible();
    }
    //get random element
    int rand = int(ofRandom(mNumVideos));
    ofLogNotice() << mVizIdName << "Play Video: " << rand << endl;
    /*
    int counter = 0;
    list<Dot*>::iterator tit;
    for (tit=videos.begin(); tit != videos.end(); tit++) {
        Dot* video = (*tit);
        if (counter == rand) video -> playVideo();
        counter ++;
    }*/
}
//--------------------------------------------------------------
void vizMoviePlayer::guiEvent(ofxUIEventArgs &e){
    testApp* myApp = (testApp*)ofGetAppPtr();
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
        
    } else if (name == "Nike TUNE"){
        myApp->mCurrentMode =  testApp::kModeNikeTune;
        
        list<Dot*>::iterator tit;
        for (tit=nikeTuneVideos.begin(); tit != nikeTuneVideos.end(); tit++) {
            Dot* video = (*tit);
            video->playVideo();
        }

       
    } else if (name == "Nike BEATS"){
       myApp->mCurrentMode =  testApp::kModeNikeBeats;
        list<Dot*>::iterator tit;
        for (tit=nikeBeatsVideos.begin(); tit != nikeBeatsVideos.end(); tit++) {
            Dot* video = (*tit);
            video->playVideo();
        }
    } else if (name == "Converse TUNE"){
        myApp->mCurrentMode =  testApp::kModeConverseTune;
        list<Dot*>::iterator tit;
        for (tit=converseTuneVideos.begin(); tit != converseTuneVideos.end(); tit++) {
            Dot* video = (*tit);
            video->playVideo();
        }
    } else if (name == "Adidas BEATS"){
        myApp->mCurrentMode =  testApp::kModeAdidasBeats;
        list<Dot*>::iterator tit;
        for (tit=adidasBeatsVideos.begin(); tit != adidasBeatsVideos.end(); tit++) {
            Dot* video = (*tit);
            video->playVideo();
        }
    }
}
//- - - - - - - 

//--------------------------------------------------------------
//bool vizMoviePlayer::setMode(ModeName newMode) {
//    if(mCurrentMode != newMode) {
//        
//        ofLogNotice() << " vizMoviePlayer: MODE SWITCH: mCurrentMode: " << mCurrentMode << " " << " new Mode: " << newMode << endl;
//        mCurrentMode = newMode;
//        return true;
//    }
//    return false;
//}

//--------------------------------------------------------------
void vizMoviePlayer::exit() {
    ofLogNotice() << "vizMoviePlayer::exit()" << endl;
    delete mOverlayMovie;
    gui->saveSettings(mXmlPath);
    delete gui;
}

