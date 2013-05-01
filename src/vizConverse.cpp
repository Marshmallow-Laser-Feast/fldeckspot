//
//  vizConverse.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 26/04/2013.
//
//

#include "vizConverse.h"

#define MIN_BOUNCE				0.2
#define MAX_BOUNCE				0.9

#define	GRAVITY					0.01

#define SECTOR_COUNT			1		// currently there is a bug at sector borders

#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#define MAIN_SCREEN_WIDTH 1680
#define MAIN_SCREEN_HEIGHT 1080
#define DECK_WIDTH 1280
#define DECK_HEIGHT 1080

#define NUM_STRIPES 100

void vizConverse::setup(){
    
    _id = 3;
    smVizCount = 10;
    mPresetCount = 10;
    mSelectedPreset = 0;
    
    sprintf(mVizName, "vizConverse");
    ofLogNotice() << mVizName << "::setup()" << endl;
    
    sprintf(mVizIdName, "%d_%s", _id, mVizName);
    sprintf(mXmlPath, "GUI/%s%i.xml", mVizName, smVizCount);
    sprintf(mXmlPath2, "GUI/%s%i_2.xml", mVizName, smVizCount);
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
    
    m_imgGradient.allocate(162,81, OF_IMAGE_COLOR);
    m_imgGradient.loadImage("img/gradient_162x81.png");
    
    gui = new ofxUIScrollableCanvas(800+dim, 0, length+xInit, 1080);
    gui->setScrollAreaToScreen();
    gui->setScrollableDirections(false, true);
    
    
    gui->setFont("GUI/CarroisGothic-Regular.ttf");                     //This loads a new font and sets the GUI font
    gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
    gui->setFontSize(OFX_UI_FONT_LARGE, 8); //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui->setFontSize(OFX_UI_FONT_SMALL, 6); //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
    
    //  - - - - - - - - - - CONVERSE - - - - - - - - - - - - - - - - -
    gui->addWidgetDown(new ofxUILabel(mVizName, OFX_UI_FONT_LARGE));
    
    gui->addSlider("select preset", 0.0f, mPresetCount, &mSelectedPreset, length-xInit,dim);
    gui->addButton("load", false, dim, dim);
    gui->addButton("save", false, dim, dim);

    gui->addToggle("mbCamTop", &mbCamTop, dim, dim);
    gui->addSlider("mCamTopY", -100.0, -2000.0, &mCamTopY, length-xInit,dim);
    gui->addToggle("mbToggleShape", &mbToggleShape, dim, dim);
    gui->addToggle("mbToggleOverlayImage", &mbToggleOverlayImage, dim, dim);
    gui->addToggle("mbToggleOverlayMovie", &mbToggleOverlayMovie, dim, dim);
    gui->addSlider("mCamRotZ", 0, 360, &mCamRotZ, length-xInit,dim);
    gui->addSlider("mCamRotY", 0, 360, &mCamRotY, length-xInit,dim);
    gui->addSlider("mCamRotX", 0, 360, &mCamRotX, length-xInit,dim);
    
    gui->addSlider("mOuterStripeRadiusMin", 0, 1000.0, &mOuterStripeRadiusMin, length-xInit,dim);
    gui->addSlider("mOuterStripeRadiusMax", -10.0, 10.0, &mOuterStripeRadiusMax, length-xInit,dim);
    gui->addSlider("mInnerStripeRadiusMin", 0, 1000.0, &mInnerStripeRadiusMin, length-xInit,dim);
    gui->addSlider("mInnerStripeRadiusMax", -10.0, 10.0, &mInnerStripeRadiusMax, length-xInit,dim);
    gui->addSlider("mSecondRingRadiusMin", 0.0, 1000, &mSecondRingRadiusMin, length-xInit,dim);
    gui->addSlider("mSecondRingRadiusMax", -10.0, 10, &mSecondRingRadiusMax, length-xInit,dim);
    
    gui->addSlider("mOuterStripeThickness", 0.01, 30.0, &mOuterStripeThickness, length-xInit,dim);
    gui->addSlider("mInnerStripeThickness", 0.01, 30.0, &mInnerStripeThickness, length-xInit,dim);
    gui->addSlider("mSecondRingStripeThickness", 0.01, 30.0, &mSecondRingStripeThickness, length-xInit,dim);
    
    gui->addSlider("mOffset", 0,300, &mOffset, length-xInit,dim);
    float anglePerStripe = 360.0/NUM_STRIPES;
    gui->addSlider("mSecondRingOffsetAngle", 0, 90.0, &mSecondRingOffsetAngle, length-xInit,dim);
    gui->addSlider("mStripeOffsetAngle", -anglePerStripe,anglePerStripe, &mStripeOffsetAngle, length-xInit,dim);

    gui->addSlider("mStripeOffset", 0,20, &mStripeOffset, length-xInit,dim);
    gui->addToggle("mbToggleCamRot", &mbToggleCamRot, dim, dim);
    
    gui->addSlider("mRange", 1.0, 400.0, &mRange, length-xInit,dim);
    gui->addSlider("mIntensity", 0.0, 200.0, &mIntensity, length-xInit,dim);
    
    
    //gui->addWidgetDown(new ofxUILabel("Springs", OFX_UI_FONT_SMALL));
    
    
    //  - - - - - - - - - - GUI 2 - - - - - - - - - - - - - - - - -
    gui2 = new ofxUICanvas(1000+dim, 0, length+xInit, 1080);
    
    gui2->setFont("GUI/CarroisGothic-Regular.ttf");                     //This loads a new font and sets the GUI font
    gui2->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
    gui2->setFontSize(OFX_UI_FONT_LARGE, 8); //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui2->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui2->setFontSize(OFX_UI_FONT_SMALL, 6); //SUPER IMPORTANT NOTE: CALL THESE FUNTIO
    
    gui2->addWidgetDown(new ofxUILabel("mBgColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mBgColor"));
    
   
    gui2->addWidgetDown(new ofxUILabel("mStripeOutsideColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mStripeOutsideColor"));

    gui2->addWidgetDown(new ofxUILabel("mStripeInsideColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mStripeInsideColor"));
    
    gui2->addWidgetDown(new ofxUILabel("mPhysicsColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mPhysicsColor"));
    
    gui2->addWidgetDown(new ofxUILabel("mPhysicsLineColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mPhysicsLineColor"));
   
    

    gui2->addSlider("mLineStepInside", 1, 10, &mLineStepInside, length-xInit,dim);
    gui2->addSlider("mLineStepOutside", 1, 10, &mLineStepOutside, length-xInit,dim);
    gui2->addSlider("mSphereSize", 1, 20, &mSphereSize, length-xInit,dim);
    gui2->addSlider("mLineThickness", 0, 10, &mLineThickness, length-xInit,dim);
    gui2->addToggle("toggle physics", &mbTogglePhysics, dim, dim);
    gui2->addSlider("gravity", 0, 10, &mGravity, length-xInit,dim);
    gui2->addSlider("mRadius", 10, 300, &mRadius, length-xInit,dim);
    gui2->addSlider("drag", 0.01, 1.0, &mDrag, length-xInit,dim);
    gui2->addSlider("strength", 0.001, 1.0, &mSpringStrength, length-xInit,dim);
    gui2->addSlider("rest length", 10, 300, &mSpringRestLength, length-xInit,dim);
    
    
    gui->toggleVisible();
    ofAddListener(gui->newGUIEvent,this,&vizConverse::guiEvent);
    
    gui2->toggleVisible();
    ofAddListener(gui2->newGUIEvent,this,&vizConverse::guiEvent);
    
    
    //  - - - - - - - - - - PHYSICS - - - - - - - - - - - - - - - - -
    //	physics.verbose = true;			// dump activity to log
	physicsConverse.setGravity(ofVec3f(0, GRAVITY, 0));
	// set world dimensions, not essential, but speeds up collision
	//physics.setWorldSize(ofVec3f(-width/2, -height, -width/2), ofVec3f(width/2, height, width/2));
	physicsConverse.setSectorCount(SECTOR_COUNT);
    //physicsConverse.setDrag(0.97f);
	//physicsConverse.setDrag(1);		// FIXTHIS
	physicsConverse.disableCollision();

    //  - - - - - - - - - - FBO - - - - - - - - - - - - - - - - -
    
    // TV RENDER
//    rgbaFbo.allocate(DECK_WIDTH, DECK_HEIGHT, GL_RGBA, 4);
    //rgbaFbo.allocate(1920, 1080, GL_RGBA, 4);
//    rgbaFbo.begin();
//    ofClear(255,128,128,0);
//    rgbaFbo.end();
    
    //  - - - - - - - - - - CAMERA - - - - - - - - - - - - - - - - -
   
    camTop.setOrientation(camTopIdleRotation);
    cam.enableMouseInput();
   // camTopTarget = ofVec3f(0,0,0);
    
      
    //  - - - - - - - - - - OVERLAY - - - - - - - - - - - - - - - - -
    mOverlayMovie =  new ofVideoPlayer();
    mOverlayMovie->setPixelFormat(OF_PIXELS_RGBA);
    // mOverlayMovie->setLoopState()
    mOverlayMovie->loadMovie("overlay/Nike_Tune_Centre_001_loop.mov");
    mOverlayMovie->play();

    mOverlayImage = new ofImage();
    mOverlayImage->loadImage("overlay/Converse_Effects_Gfx_002.png");
    
    
    //  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    addParticleCircle();
    

     //  - - - - - - - - - - Load Settings - - - - - - - - - - - - - - - - -
    gui->loadSettings(mXmlPath);
    gui2->loadSettings(mXmlPath2);

}
//--------------------------------------------------------------
void vizConverse::addParticleCircle() {
    cout << "vizConverse::addParticleGrid(): "<< endl;
    
    
    
    float mass		= 1;
	float bounce	= ofRandom(MIN_BOUNCE, MAX_BOUNCE);
	float posY  = 0;
       
    //center particle
    center = physicsConverse.makeParticle(ofVec3f(0,0,0));
    center->setMass(mass)->setBounce(bounce)->setRadius(radius)->disableCollision()->makeFree();
    center->makeFixed();
    
    
    //circle particles
    for (int i=0; i<NUM_STRIPES; i++){
        cout << "create particle " << i << endl;
        float x = sin(float(i)/NUM_STRIPES*TWO_PI) * radius;
        float z = cos(float(i)/NUM_STRIPES*TWO_PI) * radius;
        
        ofVec3f aPos = ofVec3f(x,mSpringRestLength,z);
        msa::physics::Particle3D *a = physicsConverse.makeParticle(aPos);
        a->setMass(mass)->setBounce(bounce)->setRadius(radius)->disableCollision()->makeFree();
        
        //msa::physics::Spring3D *spring =   physicsConverse.makeSpring(center, a, .8, radius);
        
        //centerSprings.push_back(spring);
        insideParticles.push_back(a);
        
    }
    //top ring particles
    for (int i=0; i<NUM_STRIPES; i++){
        cout << "create particle " << i << endl;
        float x = sin(float(i)/NUM_STRIPES*TWO_PI) * radius;
        float z= cos(float(i)/NUM_STRIPES*TWO_PI) * radius;
        
        ofVec3f aPos = ofVec3f(x,0,z);
        msa::physics::Particle3D *a = physicsConverse.makeParticle(aPos);
        a->setMass(mass)->setBounce(bounce)->setRadius(radius)->disableCollision()->makeFixed();
        
        msa::physics::Spring3D *spring =   physicsConverse.makeSpring(a, insideParticles.at(i), mSpringStrength, mSpringRestLength);
        
        topRingSprings.push_back(spring);
        topParticles.push_back(a);
        
    }
    
    //create springs with each neighbour
    for (int i=0; i<NUM_STRIPES; i++){
        msa::physics::Particle3D *a = insideParticles.at(i);
        msa::physics::Particle3D *b;
        
        
        if(i < insideParticles.size()-1){
            b = insideParticles.at(i+1);
        } else {
            // close circle
            if(mbToggleShape) b = insideParticles.at(0);
        }
        float dist = (a->getPosition() - b->getPosition()).length();
        
        msa::physics::Spring3D *spring = physicsConverse.makeSpring(a, b, .01, dist );
        neighbourSprings.push_back(spring);
        
    }/*
      //create outside particles
      for (int i=0; i<NUM_STRIPES; i++){
      float x2 = sin(float(i)/NUM_STRIPES*TWO_PI) * radius*2 ;
      float y2 = cos(float(i)/NUM_STRIPES*TWO_PI) * radius*2 ;
      
      ofVec3f outsidePos = ofVec3f(x2,y2,0);
      msa::physics::Particle3D *outside = physics.makeParticle(outsidePos);
      outside->setMass(mass)->setBounce(bounce)->setRadius(radius)->disableCollision();
      outside->makeFixed();
      
      msa::physics::Spring3D *spring =   physics.makeSpring(outside, insideParticles.at(i), .01, radius );
      outsideSprings.push_back(spring);
      outsideParticles.push_back(outside);
      }*/
}
//--------------------------------------------------------------
void vizConverse::setSpringStrength(vector<msa::physics::Spring3D*>* group, float strength){
    for (int i = 0; i< group->size(); i++){
        msa::physics::Spring3D* s = group->at(i);
        s->setStrength(strength);
    }
}
//--------------------------------------------------------------
void vizConverse::setSpringRestLength(vector<msa::physics::Spring3D*>* group, float restLength){
    for (int i = 0; i< group->size(); i++){
        msa::physics::Spring3D* s = group->at(i);
       s->setRestLength(restLength);
    }
}
//--------------------------------------------------------------
void vizConverse::setRadius(float radius){
    if (mbToggleShape){
        for (int i = 0; i< topParticles.size(); i++){
            msa::physics::Particle3D* p = topParticles.at(i);
            float x = sin(float(i)/NUM_STRIPES*TWO_PI) * radius;
            float z = cos(float(i)/NUM_STRIPES*TWO_PI) * radius;
            
            ofVec3f target = ofVec3f(x,mSpringRestLength,z);
            p->moveTo(target);
        }
    } else {
        for (int i = 0; i< topParticles.size(); i++){
            msa::physics::Particle3D* p = topParticles.at(i);
            float x = 0;
            float z = float(i)*OUTPUT_HEIGHT/NUM_STRIPES-OUTPUT_HEIGHT/2;
            
            ofVec3f target = ofVec3f(x,mSpringRestLength,z);
            p->moveTo(target);
        }
    }
}
//--------------------------------------------------------------
void vizConverse::update( ) {
    
    //  - - - - - - - - - - CAMERA - - - - - - - - - - - - - - - - -
    //camTop.setOrientation(camTopIdleRotation);
    // ofVec3f camTopTargetPos = ofVec3f(mouseX,mCamTopY, mouseY);
    ofVec3f camTopHomePos = ofVec3f();
    //mCamTopY = ofLerp(camTop.getPosition().y, mCamTopY, .2);
    camTopHomePos.y = mCamTopY;
    
    if(mbToggleCamRot){
        if(ofGetElapsedTimeMillis()-lastBeat > 857.14285714){
            //add random force 70bpm
            addRandomForce(ofRandom(10,30));
            lastBeat = ofGetElapsedTimeMillis();
        }
        camTopIdleRotation = ofVec3f(mCamRotX,mCamRotY,mCamRotZ);
    } else {
        camTopIdleRotation = ofVec3f(90,0,0);
    }
    camTop.setOrientation(camTopIdleRotation);
//    ofVec3f difVec = (camTopTargetPos - camTop.getPosition())*.2;
//    difVec += camTop.getPosition();
//    difVec += (camTopHomePos - camTop.getPosition())*.2;
//    camTop.setGlobalPosition(difVec);
     camTop.setGlobalPosition(camTopHomePos);
    //ofNode center;
    //  camTop.orbit(mCamOrbitLongitude, 0, mCamTopY);
    
    //  - - - - - - - - - - OVERLAY - - - - - - - - - - - - - - - - -
    if (mOverlayMovie->isLoaded()) mOverlayMovie->update();
    
    //  - - - - - - - - - - PHYSICS - - - - - - - - - - - - - - - - -
    physicsConverse.setDrag(mDrag);
    physicsConverse.setGravity(ofVec3f(0, mGravity, 0));
    physicsConverse.update();
    
    //  - - - - - - - - - - FBO - - - - - - - - - - - - - - - - -
//    rgbaFbo.begin();
//    ofClear(0,68,128,0);  // TODO:
//    drawFBO();
//    rgbaFbo.end();
    
}
//--------------------------------------------------------------
void vizConverse::update( list<ofxTuioCursor*> cursors) {
    //gives better feeback when selecting presets, does not change the actual value though
    mSelectedPreset = int(mSelectedPreset);
    tuioCursors = cursors;
    int count = 0;
    list<ofxTuioCursor*>::iterator tit;
    for (tit=cursors.begin(); tit != cursors.end(); tit++) {
        ofxTuioCursor* cursor = (*tit);
        float x = cursor->getX()*OUTPUT_WIDTH-DECK_WIDTH/2;
        float y = (-cursor->getY()*OUTPUT_HEIGHT+DECK_HEIGHT/2)*(-1);
        addOutsideForce(ofVec3f(x, 0, y), mIntensity, mRange);
        count ++;
    }
}
//--------------------------------------------------------------
void vizConverse::update(ofVec3f target) {
    //gives better feeback when selecting presets, does not change the actual value though
    mSelectedPreset = int(mSelectedPreset);
    
}
//--------------------------------------------------------------
void vizConverse::drawContent(){
    ofSetColor(255);
    if (mbCamTop){
        camTop.begin();
    } else {
        cam.begin();
    }
    
    if(mbTogglePhysics) {
        // DRAW ALL
        for(int i=0; i<physicsConverse.numberOfSprings(); i++) {
            msa::physics::Spring3D *spring = (msa::physics::Spring3D *) physicsConverse.getSpring(i);
            msa::physics::Particle3D *a = spring->getOneEnd();
            msa::physics::Particle3D *b = spring->getTheOtherEnd();
    
            ofPushStyle();
            ofSetLineWidth(mLineThickness);
            ofSetColor(mPhysicsLineColor);
            ofLine(a->getPosition(), b->getPosition());
            ofPopStyle();
       }
        for (int i = 0; i< insideParticles.size(); i++){
            msa::physics::Particle3D* p = insideParticles.at(i);
            ofPushStyle();
            ofSetColor(mPhysicsColor);
            ofSphere(p->getPosition(), mSphereSize);
            ofPopStyle();
        }
        /*
        for (int i = 0; i< topParticles.size(); i++){
            msa::physics::Particle3D* p = topParticles.at(i);
            ofPushStyle();
            ofSetColor(255);
            ofSphere(p->getPosition(), 5);
            ofPopStyle();
        }*/
    }
    if(mbToggleShape){
        // draw inside stripes
        ofVec3f dif;
        ofPushMatrix();
        mLineStepInside = int (mLineStepInside);
        //mLineStepOutside = int (mLineStepOutside);
        int innerLineCount = NUM_STRIPES/mLineStepInside;
        float angleStep = 360.0/float(innerLineCount);
        for (int i = 0; i<innerLineCount; i++){
            msa::physics::Particle3D*p = insideParticles.at(i);
            ofVec3f projVec = ofVec3f(p->getPosition().x, 0, p->getPosition().z);
            
            dif = projVec-center->getPosition();
            float dist = dif.length();
            
            //STRIPES INSIDE - - - - - - - - - - -
            drawStripe(mInnerStripeRadiusMin, dist*mInnerStripeRadiusMax, mInnerStripeThickness, i*angleStep, mStripeInsideColor);
        }
        ofPopMatrix();

        
        
        ofPushMatrix();
        
        int outerLinesCount = NUM_STRIPES/mLineStepOutside;
        float outerLinesAngleStep  = 360.0/float(outerLinesCount);
        for (int i = 0; i<outerLinesCount; i++){
            msa::physics::Particle3D*p = insideParticles.at(i);
            ofVec3f projVec = ofVec3f(p->getPosition().x, 0, p->getPosition().z);
            
            dif = projVec-center->getPosition();
            float dist = dif.length();
            //float angle  = float(i)/outerLinesCount*360-90; //atan2(dif.y, dif.x)*180.0/PI;
            //float angle2 = (float(i)+.5)/outerLinesCount*360-90;

             //STRIPES Outside in between - - - - - - - - - - -
//            if (i<outerLinesCount-1){
//                msa::physics::Particle3D* p2 = insideParticles.at(i+1);
//                ofVec3f projVec2 = ofVec3f(p2->getPosition().x, 0, p2->getPosition().z);
//                ofVec3f dif2 = projVec2-center->getPosition();
//                //float dist2 = dif2.length()*.5 + dist*.5;
//                
//                drawStripe(mOuterStripeRadiusMin, dist*mOuterStripeRadiusMax, mOuterStripeThickness, outerLinesAngleStep*(i+.5), mStripeOutsideColor);
//            } else {
//                msa::physics::Particle3D* p2 = insideParticles.at(0);
//                ofVec3f projVec2 = ofVec3f(p2->getPosition().x, 0, p2->getPosition().z);
//                ofVec3f dif2 = projVec2-center->getPosition();
//                float dist2 = dif2.length()*.5 + dist*.5;
//                
//                drawStripe(mOuterStripeRadiusMin ,dist*mOuterStripeRadiusMax, mOuterStripeThickness, outerLinesAngleStep*(i+.5),mStripeOutsideColor);
//            
//            }
          
            //STRIPES Outside - - - - - - - - - - -
            ofColor c;
            if(0 < (dist-mOffset) ) {
                c = mStripeInsideColor;
            }else {
                c = mStripeOutsideColor;
            }
            drawStripe(mOuterStripeRadiusMin, mOuterStripeRadiusMax*(dist-mOffset), mOuterStripeThickness, outerLinesAngleStep*i, c);
            
            //STRIPES Outside 2 - - - - - - - - - - -
            //drawStripe(mSecondRingRadiusMin, dist*mSecondRingRadiusMax, mSecondRingStripeThickness, outerLinesAngleStep*i+mSecondRingOffsetAngle, ofColor(255,0,0));
           
         
        }
    } else {
        
        ofVec3f dif;
        float dist;
        
        ofPushMatrix();
        ofPushStyle();
      
        //ofRotateX(90);
        for (int i = 0; i<NUM_STRIPES; i+=mLineStepInside){
            msa::physics::Particle3D*p = insideParticles.at(i);
            float dist = p->getPosition().x;
            
            glBegin(GL_QUADS);
            float LTX = 0;
            float LTY = i*DECK_HEIGHT/NUM_STRIPES + mStripeOffset*mOuterStripeThickness - DECK_HEIGHT/2;
            float RTX = dist*mOuterStripeRadiusMax;
            float RTY = LTY;
            float RBX = RTX;
            float RBY = LTY + mOuterStripeThickness;
            float LBX = 0;
            float LBY = RBY;
           
            if (dist > 0){
                ofSetColor(255,255,255);
            } else {
                ofSetColor(0);
               
            }
            
            glVertex3f(LTX, 0, LTY);
            glVertex3f(RTX, 0, RTY);
            glVertex3f(RBX, 0, RBY);
            glVertex3f(LBX, 0, LBY);
            glEnd();
            
        }
        ofPopStyle();
        ofPopMatrix();

        
    }
    //draw TUIO CURSORS
    if (mbShowDots){
        list<ofxTuioCursor*>::iterator tit;
        for (tit=tuioCursors.begin(); tit != tuioCursors.end(); tit++) {
            ofxTuioCursor* cursor = (*tit);
            float x = cursor->getX()*OUTPUT_WIDTH-DECK_WIDTH/2;
            float y = (-cursor->getY()*OUTPUT_HEIGHT+DECK_HEIGHT/2)*(-1);
             ofSphere(x,0,y, mRange);
        }
    }

   
    ofPopMatrix();
    
    if (mbCamTop) {
        camTop.draw();
    } else {
        cam.draw();
    }
    
    if (mbCamTop){
        camTop.end();
    } else {
        cam.end();
    }

}
//--------------------------------------------------------------
void vizConverse::drawStripe (float x, float length, float thickness, float angle, ofColor color ){
    ofPushMatrix();
    ofPushStyle();
    ofRotateY(angle-90);
    ofSetColor(color);
    //ofRect(center->getPosition(), dist, stripeWidth);
    glBegin(GL_QUADS);
    glVertex3f(x, 0, -thickness/2);
    glVertex3f(length+x, 0, -thickness/2);
    glVertex3f(length+x, 0, thickness/2);
    glVertex3f(x, 0, thickness/2);
    glEnd();
    ofPopStyle();
    ofPopMatrix();
}
//--------------------------------------------------------------
void vizConverse::draw(){
    ofEnableAlphaBlending();
    glDisable(GL_DEPTH_TEST);

    if (mbToggleShape){
        ofSetColor(mBgColor);
        ofRect(0,0,DECK_WIDTH ,DECK_HEIGHT);
    } else {
        // TV RENDER
        if(!mbToggleCamRot){
    
            ofSetColor(0);
            ofRect(0,0,640,1080);
            ofSetColor(255);
            ofRect(640,0,640,1080);
        } else {
            ofSetColor(255);
            ofRect(0,0,DECK_WIDTH,DECK_HEIGHT/2);
            ofSetColor(0);
            ofRect(0,DECK_HEIGHT/2,DECK_WIDTH,DECK_HEIGHT/2);
        }
    }
    ofSetColor(255);
//    rgbaFbo.draw(0,0);
    drawContent();

    if (mbToggleOverlayImage) mOverlayImage->draw(0,0);
    if (mbToggleOverlayMovie && mOverlayMovie->isLoaded())  mOverlayMovie -> draw(640-mOverlayMovie->width/2,540-mOverlayMovie->height/2);
   
    ofDisableAlphaBlending();
}
//--------------------------------------------------------------
void vizConverse::addOutsideForce(ofVec3f target, float intensity, float range) {
    ofVec3f dif, targetDif, difNorm, projPos;
    int counter  =0;
    for (int i = 0; i<insideParticles.size(); i++){
        
        msa::physics::Particle3D*p = insideParticles.at(i);
        msa::physics::Particle3D*pTop = topParticles.at(i);
        //msa::physics::Spring3D *s = outsideSprings.at(i);
        
        projPos = ofVec3f(p->getPosition().x, 0, p->getPosition().z);
        
        if (mbToggleShape){
            //cout << "addOutsideForce: projPos: " << projPos << endl;
            if (pTop->getPosition().distanceSquared(center->getPosition()) < target.distanceSquared(center->getPosition()) ){
                dif = center->getPosition()-projPos;
            } else {
                dif = projPos-center->getPosition();
            }
        }else {
            dif = ofVec3f(p->getPosition().x-mouseX, 0, 0);
        }
        
        difNorm = dif.getNormalized();
        //cout << "addOutsideForce: projPos: " << projPos << " target: " << target << endl;
        targetDif = target - projPos;
        float dist = targetDif.length();
        cout << "addOutsideForce: dist: " << dist << endl;
        if ( dist < range) {
            //cout << "found close particles! projPos: "<< projPos <<" id: "  << i << " force: " << difNorm*intensity << endl;
            p->addVelocity(difNorm*intensity);
            //p->addVelocity(ofVec3f(3,0,0));
            counter ++;
           
        }
    }
    if (counter == 0)  cout << "vizConverse::addOutsideForcenot in range" << endl;
    
}
//--------------------------------------------------------------
void vizConverse::addRandomForce(float f) {
    if (!mbToggleShape) {
        f*=mIntensity/100;
    };
   
    ofLogNotice() << mVizIdName << ":: addRandomForce: strength: " << f << endl;
	//forceTimer = f;
	for(int i=0; i<insideParticles.size(); i++) {
		msa::physics::Particle3D *p = insideParticles.at(i);
		if(p->isFree()) p->addVelocity(ofVec3f(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
	}
}
//--------------------------------------------------------------
void vizConverse::keyPressed(int key) {
    ofLogNotice() << mVizIdName << ":: keyPressed:" << key << " _id: " << _id <<endl;

    if ( _id == key-49){
        ofLogNotice() << mVizIdName << ":: gui->toggleVisible();" << endl;
        gui->toggleVisible();
        gui2->toggleVisible();
    }
    switch(key) {
         case 32: addRandomForce(10 * 3); break;
        default:
            break;
    }
    
}
//--------------------------------------------------------------
void vizConverse::mouseReleased(int x, int y, int button){
    addOutsideForce(ofVec3f(mouseX,0,mouseY), mIntensity, mRange);
}
//--------------------------------------------------------------
void vizConverse::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogNotice() << "got event from: " << name << endl;
	
	if(name == "mBGColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mBgColor =  sampler->getColor();
    } else if (name == "mStripeOutsideColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mStripeOutsideColor =  sampler->getColor();
    }else if (name == "mStripeInsideColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mStripeInsideColor =  sampler->getColor();
    }
    
	//ofLogNotice() << mVizIdName << ":: got event from: " << name << endl;
    
    char selectedPreset [32];
    char selectedPresetPath [32];
    
    sprintf (selectedPreset, "%s%i", mVizName, int(mSelectedPreset));
    sprintf(selectedPresetPath, "GUI/%s.xml", selectedPreset);
    
    //char selectedPreset2 [32];
    char selectedPresetPath2 [32];
    //sprintf (selectedPreset2, "%s%i", mVizName, int(mSelectedPreset));
    sprintf(selectedPresetPath2, "GUI/%s_2.xml", selectedPreset);
    
    
	if(name == "load"){
        ofLogNotice() << mVizIdName << "guiEvent:: loadSettings:" << selectedPresetPath << endl;
        gui->loadSettings(selectedPresetPath);
        gui2->loadSettings(selectedPresetPath2);
        
    } else if(name == "save"){
        
        ofLogNotice() << mVizIdName << "guiEvent:: Save Settings:" << selectedPresetPath << endl;
        gui->saveSettings(selectedPresetPath);
        gui2->saveSettings(selectedPresetPath2);
        
    } else if (name == "strength"){
        setSpringStrength(&topRingSprings,mSpringStrength);
        
    } else if (name == "rest length"){
        setSpringRestLength(&topRingSprings,mSpringRestLength);
        
    } else if (name == "mRadius"){
        setRadius(mRadius);
    } else if(name == "mBgColor"){
        ofxUIImageSampler *sampler2 = (ofxUIImageSampler *) e.widget;
        mBgColor =  sampler2->getColor();
        
    }  else if(name == "mPhysicsLineColor"){
        ofxUIImageSampler *sampler2 = (ofxUIImageSampler *) e.widget;
        mPhysicsLineColor =  sampler2->getColor();
        
    }  else if(name == "mPhysicsColor"){
        ofxUIImageSampler *sampler2 = (ofxUIImageSampler *) e.widget;
        mPhysicsColor =  sampler2->getColor();
        
    }
}
//--------------------------------------------------------------
void vizConverse::mouseMoved(int x, int y ) {
    //ofLogNotice() << "mouseX" << mouseX << endl;
    mouseX  = x-DECK_WIDTH/2;
    mouseY =  (-y+DECK_HEIGHT/2)*(-1);
    
}
//--------------------------------------------------------------
void vizConverse::updateMidi (ofxMidiMessage m){
    if(m.control == 1){
        //ofLogNotice() << "vizConverse::updateMidi() value: " << m.value << endl;
        float v = ofMap(m.value, 0, 127, 0, 10);
        mInnerStripeRadiusMax = ofLerp(mInnerStripeRadiusMax, v,0.8);
    }
}
//--------------------------------------------------------------
void vizConverse::exit() {
    ofLogNotice() << "vizConverse::exit()" << endl;
    //gui->saveSettings(mXmlPath);
    //gui2->saveSettings(mXmlPath2);
    delete gui;
    delete gui2;
}
//--------------------------------------------------------------
void vizConverse::loadSettings(int id) {
    ofLogNotice() << "vizConverse::loadSettings: " << id<< endl;
    
    char selectedPreset [32];
    char selectedPresetPath [32];
    
    sprintf (selectedPreset, "%s%i", mVizName, id);
    sprintf(selectedPresetPath, "GUI/%s.xml", selectedPreset);
    
   
    char selectedPresetPath2 [32];
   
    sprintf(selectedPresetPath2, "GUI/%s_2.xml", selectedPreset);
    
    gui->loadSettings(selectedPresetPath);
    gui2->loadSettings(selectedPresetPath2);
}
//--------------------------------------------------------------
//void vizConverse::mouseMoved(int x, int y ){
//    mouseX = x-OUTPUT_WIDTH/2.0;
//    mouseY = y-OUTPUT_HEIGHT/2.0;

//    ofVec3f m = ofVec3f(mouseX, mouseY, 0);
//    float maxDist = radius;
//
//    list<Stripe*>::iterator tit;
//    for (tit=stripes.begin(); tit != stripes.end(); tit++) {
//        Stripe *stripe = (*tit);
//
//        float dist = m.distance(stripe->home);
//        if (dist < maxDist) {
//
//            dist = stripe->mIdleWidth *(maxDist-dist)/maxDist;
//            stripe->setWidth(10.0/dist*stripe->mIdleWidth);
//        } else {
//             stripe->setWidth(0);
//        }

//dist = ofClamp(dist, 0, 1);
//dist = 1/dist;

//cout << "vizConverse::mouseMoved: x: "<< x << " y: " << y << " distance: " << dist <<endl;

//stripe->update();
// }
//}


