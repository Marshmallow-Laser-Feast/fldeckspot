//
//  vizPhysics.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 26/04/2013.
//
//
#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080
#define MAIN_SCREEN_WIDTH 1680
#define MAIN_SCREEN_HEIGHT 1080
#define DECK_WIDTH 1280
#define DECK_HEIGHT 1080
#include "vizPhysics.h"

#define MIN_BOUNCE				0.2
#define MAX_BOUNCE				0.9

#define MAX_LIGHT_POS 1000
#define SECTOR_COUNT			1		// currently there is a bug at sector borders


void vizPhysics::setup(){
    
    gridResX = 34;
    gridResY = 34; //for dots
    
    
    
    _id = 2;
    smVizCount = 10;
    mPresetCount = 10;
    //mSelectedPreset = 0;
    
    sprintf(mVizName, "vizPhysics");
    ofLogNotice() << mVizName << "::setup()" << endl;
    
    sprintf(mVizIdName, "%d_%s", _id, mVizName);
    sprintf(mXmlPath, "GUI/%s%i.xml", mVizName, int(mSelectedPreset));
    sprintf(mXmlPath2, "GUI/%s%i_2.xml", mVizName, int(mSelectedPreset));
    sprintf(mXmlPath3, "GUI/%s%i_3.xml", mVizName, int(mSelectedPreset));
    
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
    
    gui = new ofxUICanvas(600+dim, 0, length+xInit, 1080);
    
    gui->setFont("GUI/CarroisGothic-Regular.ttf");                     //This loads a new font and sets the GUI font
    gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
    gui->setFontSize(OFX_UI_FONT_LARGE, 8); //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui->setFontSize(OFX_UI_FONT_SMALL, 6); //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
    
    
    gui->addWidgetDown(new ofxUILabel(mVizName, OFX_UI_FONT_MEDIUM));
    gui->addSlider("select preset", 0.0f, mPresetCount, &mSelectedPreset, length-xInit,dim);
    gui->addButton("load", false, dim, dim);
    gui->addButton("save", false, dim, dim);
    
    //  - - - - - - - - - - VIZ UI - - - - - - - - - - - - - - - - -
    m_imgGradient.allocate(162,81, OF_IMAGE_COLOR);
    m_imgGradient.loadImage("img/gradient_162x81.png");
    gui->addToggle("mbCamTop", &mbCamTop, dim, dim);
    gui->addSlider("mCamTopY", -100.0, -2000.0, &mCamTopY, length-xInit,dim);
    //gui->addSlider("mCamTopX", -500.0, 500.0, &mCamTopX, length-xInit,dim);
    
    gui->addToggle("mbToggleShader", &mbToggleShader, dim, dim);
    gui->addToggle("mbPerVertexUVs", &mbPerVertexUVs, dim, dim);
    gui->addToggle("mbToggleLighting", &mbToggleLighting, dim, dim);
    gui->addToggle("mbToggleGLTriangles", &mbToggleGLTriangles, dim, dim);
    gui->addToggle("mbToggleGLLines", &mbToggleGLLines, dim, dim);
    
    gui->addToggle("mbToggleMesh", &mbToggleMesh, dim, dim);
    gui->addToggle("mbToggleBoxes", &mbToggleBoxes, dim, dim);
    gui->addToggle("mbToggleSpheres", &mbToggleSpheres, dim, dim);
    gui->addToggle("mbTogglePulse", &mbTogglePulse, dim, dim);
    gui->addSlider("mPulseIntensity", 0.0, 0.5, &mPulseIntensity, length-xInit,dim);
    
    gui->addToggle("mbTogglePointLight", &mbTogglePointLight, dim, dim);
    gui->addToggle("mbToggleSpotLightMouse", &mbToggleSpotLightMouse, dim, dim);
    gui->addSlider("mBoxSize", 1, 100, &mBoxSize, length-xInit,dim);
    gui->addSlider("mSphereSize", 1, 100, &mSphereSize, length-xInit,dim);
    gui->addSlider("mForceRadius", 1, 300, &mForceRadius, length-xInit,dim);
    gui->addSlider("mForceStrength", -500, 500, &mForceStrength, length-xInit,dim);
    
    gui->addSlider("gravity", 0, 10, &mGravity, length-xInit,dim);
    gui->addSlider("drag", 0.01, 1.0, &mDrag, length-xInit,dim);
    
    //gui->addWidgetDown(new ofxUILabel("pointlight position", OFX_UI_FONT_LARGE));
    gui->addSlider("mPointLightPosX", -MAX_LIGHT_POS, MAX_LIGHT_POS, &mPointLightPosX, length-xInit,dim);
    gui->addSlider("mPointLightPosY", -MAX_LIGHT_POS, MAX_LIGHT_POS, &mPointLightPosY, length-xInit,dim);
    gui->addSlider("mPointLightPosZ", -MAX_LIGHT_POS, MAX_LIGHT_POS, &mPointLightPosZ, length-xInit,dim);
    gui->addWidgetDown(new ofxUILabel("mPointLightDiffuseColor", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mPointLightDiffuseColor"));
    gui->addWidgetDown(new ofxUILabel("mPointLightSpecularColor", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mPointLightSpecularColor"));
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    gui2 = new ofxUICanvas(800+dim, 0, length+xInit, 1080);
    
    gui2->setFont("GUI/CarroisGothic-Regular.ttf");                     //This loads a new font and sets the GUI font
    gui2->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
    gui2->setFontSize(OFX_UI_FONT_LARGE, 8); //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui2->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui2->setFontSize(OFX_UI_FONT_SMALL, 6); //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
    
    //gui2->addWidgetDown(new ofxUILabel(mVizName, OFX_UI_FONT_MEDIUM));
    //    gui2->addSlider("select preset", 0.0f, mPresetCount, &mSelectedPreset, length-xInit,dim);
    //    gui2->addButton("load", false, dim, dim);
    //    gui2->addButton("save", false, dim, dim);
    gui2->addSlider("mSphereResolution", 1, 40, &mSphereResolution, length-xInit,dim);
    gui2->addWidgetDown(new ofxUILabel("mSphereColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mSphereColor"));
    gui2->addWidgetDown(new ofxUILabel("mBoxColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mBoxColor"));
    
    gui2->addWidgetDown(new ofxUILabel("Material", OFX_UI_FONT_LARGE));
    gui2->addToggle("mbToggleMaterial", &mbToggleMaterial, dim, dim);
    gui2->addToggle("mbToggleTexture DOTS", &mbToggleTexture1, dim, dim);
    gui2->addToggle("mbToggleTexture2 STRIPES", &mbToggleTexture2, dim, dim);
//    gui2->addToggle("mbToggleFboTexture", &mbToggleFboTexture, dim, dim);
    
    
    
    gui2->addSlider("mMaterialShininess", 0, 128, &mMaterialShininess, length-xInit,dim);
    gui2->addWidgetDown(new ofxUILabel("mMaterialDiffuseColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mMaterialDiffuseColor"));
    gui2->addWidgetDown(new ofxUILabel("mMaterialSpecularColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mMaterialSpecularColor"));
    gui2->addWidgetDown(new ofxUILabel("mMaterialEmissiveColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mMaterialEmissiveColor"));
    gui2->addWidgetDown(new ofxUILabel("mMaterialAmbientColor", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mMaterialAmbientColor"));
    
    
    gui3 = new ofxUICanvas(1000+dim, 0, length+xInit, 1080);
    
    gui3->setFont("GUI/CarroisGothic-Regular.ttf");                     //This loads a new font and sets the GUI font
    gui3->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
    gui3->setFontSize(OFX_UI_FONT_LARGE, 8); //These call are optional, but if you want to resize the LARGE, MEDIUM, and SMALL fonts, here is how to do it.
    gui3->setFontSize(OFX_UI_FONT_MEDIUM, 6);
    gui3->setFontSize(OFX_UI_FONT_SMALL, 6); //SUPER IMPORTANT NOTE: CALL THESE FUNTIONS BEFORE ADDING ANY WIDGETS, THIS AFFECTS THE SPACING OF THE GUI
    
    
    gui3->addWidgetDown(new ofxUILabel("spotlight", OFX_UI_FONT_LARGE));
    //    gui3->addSlider("select preset", 0.0f, mPresetCount, &mSelectedPreset, length-xInit,dim);
    //    gui3->addButton("load", false, dim, dim);
    //    gui3->addButton("save", false, dim, dim);
    
    
    gui3->addToggle("mbToggleSpotLight", &mbToggleSpotLight, dim, dim);
    gui3->addWidgetDown(new ofxUILabel("spotlight position", OFX_UI_FONT_LARGE));
    gui3->addSlider("mSpotLightPosX", -MAX_LIGHT_POS, MAX_LIGHT_POS, &mSpotLightPosX, length-xInit,dim);
    gui3->addSlider("mSpotLightPosY", -MAX_LIGHT_POS, MAX_LIGHT_POS, &mSpotLightPosY, length-xInit,dim);
    gui3->addSlider("mSpotLightPosZ", -MAX_LIGHT_POS, MAX_LIGHT_POS, &mSpotLightPosZ, length-xInit,dim);
    
    gui3->addWidgetDown(new ofxUILabel("spotlight rotation", OFX_UI_FONT_LARGE));
    gui3->addSlider("mSpotLightRotX", 0, 360, &mSpotLightRotX, length-xInit,dim);
    gui3->addSlider("mSpotLightRotY", 0, 360, &mSpotLightRotY, length-xInit,dim);
    gui3->addSlider("mSpotLightRotZ", 0, 360, &mSpotLightRotZ, length-xInit,dim);
    gui3->addWidgetDown(new ofxUILabel("Color", OFX_UI_FONT_LARGE));
    gui3->addWidgetDown(new ofxUILabel("mSpotLightDiffuseColor", OFX_UI_FONT_SMALL));
    gui3->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mSpotLightDiffuseColor"));
    gui3->addWidgetDown(new ofxUILabel("mSpotLightSpecularColor", OFX_UI_FONT_SMALL));
    gui3->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mSpotLightSpecularColor"));
    gui3->addWidgetDown(new ofxUILabel("mVertexColor", OFX_UI_FONT_SMALL));
    gui3->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mVertexColor"));
    gui3->addWidgetDown(new ofxUILabel("mPulseColor", OFX_UI_FONT_SMALL));
    gui3->addWidgetDown(new ofxUIImageSampler(m_imgGradient.getWidth(), m_imgGradient.getHeight(), &m_imgGradient, "mPulseColor"));
    
    ofAddListener(gui->newGUIEvent,this,&vizPhysics::guiEvent);
    ofAddListener(gui2->newGUIEvent,this,&vizPhysics::guiEvent);
    ofAddListener(gui3->newGUIEvent,this,&vizPhysics::guiEvent);
    
    gui->loadSettings(mXmlPath);
    gui2->loadSettings(mXmlPath2);
    gui3->loadSettings(mXmlPath3);
    
    
    gui->toggleVisible();
    gui2->toggleVisible();
    gui3->toggleVisible();
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
	shader.load("vert.glsl", "frag.glsl");
    
    
    image.loadImage("textures/tex10.png");  // dots
    image2.loadImage("textures/tex07.png"); //stripes
    //mOverLay.loadImage("overlay/Nike_Tune_Comp_001 (00003).png");
	
    // size of the cone of emitted light, angle between light axis and side of cone //
    // angle range between 0 - 90 in degrees //
    spotLight.setSpotlightCutOff( 50 );
    spotLightMouse.setSpotlightCutOff( 50 );
    // rate of falloff, illumitation decreases as the angle from the cone axis increases //
    // range 0 - 128, zero is even illumination, 128 is max falloff //
    spotLight.setSpotConcentration( 45 );
    spotLightMouse.setSpotConcentration( 45 );
    // turn the light into spotLight, emit a cone of light //
    spotLight.setSpotlight();
    spotLightMouse.setSpotlight();
    
    
    // - - - - - - PHYSICS - - - - - - - - - -
	//physics.verbose = true;			// dump activity to log
	// set world dimensions, not essential, but speeds up collision
	//physics.setWorldSize(ofVec3f(-OUTPUT_WIDTH/2, -OUTPUT_HEIGHT, -OUTPUT_WIDTH/2), ofVec3f(OUTPUT_WIDTH/2, OUTPUT_HEIGHT, OUTPUT_WIDTH/2));
	physics.setSectorCount(SECTOR_COUNT);
    
	//physics.setDrag(1);		// FIXTHIS
	physics.disableCollision();
	
	// clear all particles and springs etc
	//physics.clear();
    
    addParticleGrid();
    
    camTop.setOrientation(ofVec3f(90,0,0));
    //camTop.enableOrtho();
    cam.enableMouseInput();
    //ofDisableArbTex();
    
//    rgbaFbo.allocate(DECK_WIDTH, DECK_HEIGHT, GL_RGBA, 4);
//    rgbaFbo.begin();
//    ofClear(255,128,128,0);
//    rgbaFbo.end();
//    
//    textureFbo.allocate(DECK_WIDTH, DECK_HEIGHT, GL_RGBA, 4);
//    rgbaFbo.begin();
//    ofClear(255,128,128,0);
//    rgbaFbo.end();
}
//--------------------------------------------------------------
void vizPhysics::addParticleGrid() {
    cout << "addParticleGrid(): "<< endl;
    
    
    float mass		= 1;
	float bounce	= ofRandom(MIN_BOUNCE, MAX_BOUNCE);
	float radius	= 10;
	float posY  = 0;
    
    
    for(int j=0; j<gridResY; j++){
        for (int i=0; i<gridResX; i++){
            float posX	= ofMap(i, 0, gridResX-1, -DECK_WIDTH/2, DECK_WIDTH/2);
            float posY  = ofMap(j, 0, gridResY-1, -DECK_HEIGHT/2, DECK_HEIGHT/2); //DECK_HEIGHT/(gridResY-1) * j - DECK_HEIGHT/2;
            
            
            msa::physics::Particle3D *a = physics.makeParticle(ofVec3f(posX, 0, posY));
            a->setMass(mass)->setBounce(bounce)->setRadius(radius)->disableCollision()->makeFree();
            particles.push_back(a);
            if(j==0 || i==0 || i==gridResX-1 || j==gridResY-1 ) a->makeFixed();
            
        }
    }
    
    
    
    int count = particles.size();
    cout << " PARTICLE COUNT: " << count << endl;
	for (int i = 0; i< count; i++){
        
        msa::physics::Particle3D* a = particles[i];
        if (i+1 < count ){
            msa::physics::Particle3D* b = particles[i+1];
            physics.makeSpring(a, b, .1,  DECK_HEIGHT/(gridResY-1));
        }
        if (i+gridResX+1 < count){
            msa::physics::Particle3D* c = particles[i+gridResX+1];
            physics.makeSpring(a, c, .01,  DECK_WIDTH/(gridResX-1));
        }
    }
    
}
//--------------------------------------------------------------
ofVec3f vizPhysics::getNormal (ofVec3f a, ofVec3f b, ofVec3f c) {
    ofVec3f BA = b-a;
    ofVec3f CA = c-a;
    ofVec3f dir = BA.getCrossed(CA);
    ofVec3f norm = dir.getNormalized();
    norm = -norm;
    return norm;
    
}
//--------------------------------------------------------------
void vizPhysics::toggleMouseAttract() {
	mouseAttract = !mouseAttract;
	if(mouseAttract) {
		// loop through all particles and add attraction to mouse
		// (doesn't matter if we attach attraction from mouse-mouse cos it won't be added internally
		for(int i=0; i<particles.size(); i++) physics.makeAttraction(&mouseNode, particles[i], ofRandom(5.0));
        
	} else {
		// loop through all existing attractsions and delete them
		for(int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->kill();
	}
}
//--------------------------------------------------------------
void vizPhysics::addRandomForce(float f) {
	//forceTimer = f;
	for(int i=0; i<particles.size(); i++) {
		msa::physics::Particle3D *p = particles[i];
		if(p->isFree()) p->addVelocity(ofVec3f(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
	}
}
//--------------------------------------------------------------
void vizPhysics::addForce(ofVec3f pos, ofVec3f dir, float f) {
    
    ofLogNotice() << "vizPhysics::addForce "<<endl;
	//forceTimer = 30.0;
    for(int i=0; i<particles.size(); i++) {
        
		msa::physics::Particle3D* p = particles[i];
        ofVec3f pPos = p->getPosition();
        float dist  = (pos - pPos).length();
        //dir.rotate(ofRandom(.4),ofRandom(.4), ofRandom(.4));
        //ofLogNotice() << "vizPhysics::addForce(): distance:" << dist<< " particle Pos: " << pPos << " pos: " << pos << endl;
        
        if ( dist< mForceRadius ){
            // ofLogNotice() << "vizPhysics::addForce(): distance:" << dist<< " particle Pos: " << pPos << endl;
            if(p->isFree()){
                p->addVelocity(dir);
            }
        }
	}
}
//--------------------------------------------------------------
void vizPhysics::update() {
    physics.setDrag(mDrag);
    physics.setGravity(ofVec3f(0, mGravity, 0));
    physics.update();
}

//--------------------------------------------------------------
void vizPhysics::update( list<ofxTuioCursor*> cursors) {
    
    int count = 0;
    list<ofxTuioCursor*>::iterator tit;
    for (tit=cursors.begin(); tit != cursors.end(); tit++) {
        ofxTuioCursor* cursor = (*tit);
        float x = cursor->getX()*OUTPUT_WIDTH-DECK_WIDTH/2;
        float y = (-cursor->getY()*OUTPUT_HEIGHT+DECK_HEIGHT/2)*(-1);
        
        addForce(ofVec3f(x,0,y), ofVec3f(0,mForceStrength, 0),1.0);
        //
        //        //cursor->setTarget(target);
        //        //cursor->update();
        //        count ++;
    }
}
//--------------------------------------------------------------
void vizPhysics::update(ofVec3f target) {
}
//--------------------------------------------------------------
void vizPhysics::draw(){
    //ofScale(0,-1,0);
//    rgbaFbo.draw(0,0);
    camTop.setGlobalPosition(0,mCamTopY, 0);
    // Point lights emit light in all directions //
    // set the diffuse color, color reflected from the light source //
    pointLight.setDiffuseColor( mPointLightDiffuseColor);
    
    // specular color, the highlight/shininess color //
	pointLight.setSpecularColor( mPointLightSpecularColor);
    //pointLight.setAmbientColor(ofColor(128, 128, 128));
    
	pointLight.setPointLight();
    
    spotLight.setDiffuseColor( mSpotLightDiffuseColor);
	spotLight.setSpecularColor( mSpotLightSpecularColor);
    spotLightMouse.setDiffuseColor( mSpotLightDiffuseColor);
    spotLightMouse.setSpecularColor( mSpotLightSpecularColor);
    
    
    material.setEmissiveColor(mMaterialEmissiveColor);
    material.setDiffuseColor(mMaterialDiffuseColor);
    material.setShininess( mMaterialShininess );
    material.setAmbientColor(mMaterialAmbientColor);
    // the light highlight of the material //
	material.setSpecularColor(mMaterialSpecularColor);
    
    //shader.setUniformTexture("tex0", image, 0);
	// = ofGetWidth();
	//height = ofGetHeight();
    
    //    for (int i = 0; i<particles.size(); i++){
    //        msa::physics::Particle3D *p = particles[i];
    //        mesh.setVertex(i,p->getPosition());
    //    }
    
    
    //ofEnableAlphaBlending();
//    rgbaFbo.begin();
//    ofClear(0,68,128,0);  // TODO?
    drawCopy();
//    rgbaFbo.end();
    /*
     textureFbo.begin();
     ofClear(0,68,128,0);
     ofSetColor (255,0,0);
     ofRect(0,0,400,400);
     textureFbo.end();*/
    
    
}
//--------------------------------------------------------------
void vizPhysics::drawCopy(){
    if (mbCamTop){
        camTop.begin();
    } else {
        cam.begin();
    }
    //ofBackground(255,0,0);
    //ofSetColor(255);
    //glShadeModel(GL_FLAT);
    //glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    //glShadeModel(GL_SMOOTH);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glAlphaFunc(GL_GREATER, 0.5);
    //ofEnableAlphaBlending();
    
    
    
    if(mbToggleShader){
        shader.begin();
        shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
        shader.setUniformTexture("tex0", image.getTextureReference(), 0);
        
    }
    
    if(mbToggleLighting) {
        ofEnableLighting();
        if (mbToggleMaterial) material.begin();
        if (mbTogglePointLight) pointLight.enable();
        if (mbToggleSpotLight) spotLight.enable();
        if (mbToggleSpotLightMouse) spotLightMouse.enable();
    }
    pointLight.setPosition( ofVec3f( mPointLightPosX, mPointLightPosY, mPointLightPosZ) );
    spotLight.setOrientation( ofVec3f( mSpotLightRotX, mSpotLightRotY, mSpotLightRotZ) );
	spotLight.setPosition( mSpotLightPosX  , mSpotLightPosY, mSpotLightPosZ);
    spotLightMouse.setPosition(ofVec3f( mouseX, mSpotLightPosY, mouseY));
    spotLightMouse.setOrientation( ofVec3f( mSpotLightRotX, mSpotLightRotY, mSpotLightRotZ) );
    
    ofEnableNormalizedTexCoords();
    if(mbToggleTexture1) {
        image.getTextureReference().bind();
        ofSetColor(255, 255, 255, 255);
    }
    if(mbToggleTexture2) {
        image2.getTextureReference().bind();
        ofSetColor(255, 255, 255, 255);
    }
    
//    if(mbToggleFboTexture) {
//        textureFbo.getTextureReference().bind();
//        ofSetColor(255, 255, 255, 255);
//    }
    
   
	//mesh.draw();
    drawMesh();
    //ofSphere( 0,0,0, 100.0f);
    //ofSetColor(0);
//    for(int i=0; i<particles.size(); i++) {
//        msa::physics::Particle3D *p = particles[i];
//        
//    }
    if(mbToggleTexture1) image.getTextureReference().unbind();
    if(mbToggleTexture2) image2.getTextureReference().unbind();
//    if(mbToggleFboTexture) textureFbo.getTextureReference().unbind();
    ofDisableNormalizedTexCoords();
    //ofSetColor(1,0,0);
    //mesh.drawWireframe();
    //ofSphere(ofVec3f(),100.f);
    // glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
    
    if(mbToggleLighting) {
        if (mbTogglePointLight) pointLight.disable();
        if (mbToggleSpotLight) spotLight.disable();
        if (mbToggleSpotLightMouse) spotLightMouse.disable();
        
        ofDisableLighting();
        if (mbTogglePointLight) {
            ofSetColor( pointLight.getDiffuseColor() );
            pointLight.draw();
        }
        if (mbToggleSpotLight){
            ofSetColor( spotLight.getDiffuseColor() );
            spotLight.draw();
        }
        if (mbToggleSpotLightMouse){
            ofSetColor(spotLightMouse.getDiffuseColor());
            spotLightMouse.draw();
        }
    }
    
    //drawScene();
    if (mbCamTop) {
        
        camTop.draw();
    } else {
        
        cam.draw();
    }
    
    if (mbToggleMaterial) material.end();
    if (mbToggleShader) shader.end();
    
    //glDisable(GL_CULL_FACE);
    if (mbCamTop){
        camTop.end();
    } else {
        cam.end();
    }
    
}
//--------------------------------------------------------------
void vizPhysics::setTextureRef(ofTexture* tex){
    texRef = tex;
}
//--------------------------------------------------------------
void vizPhysics::loadSettings (int id){
    
    
    char selectedPreset [32];
    char selectedPresetPath [32];
    sprintf (selectedPreset, "%s%i", mVizName, id);
    sprintf(selectedPresetPath, "GUI/%s.xml", selectedPreset);
    
    char selectedPresetPath2 [32];
    sprintf(selectedPresetPath2, "GUI/%s_2.xml", selectedPreset);
    
    char selectedPresetPath3 [32];
    sprintf(selectedPresetPath3, "GUI/%s_3.xml", selectedPreset);
    
    ofLogNotice() << mVizIdName << "guiEvent:: loadSettings:" << selectedPresetPath << endl;
     ofLogNotice() << mVizIdName << "guiEvent:: loadSettings:" << selectedPresetPath2 << endl;
     ofLogNotice() << mVizIdName << "guiEvent:: loadSettings:" << selectedPresetPath3 << endl;
    gui->loadSettings(selectedPresetPath);
    gui2->loadSettings(selectedPresetPath2);
    gui3->loadSettings(selectedPresetPath3);
}
//--------------------------------------------------------------
void vizPhysics::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogNotice() << "got event from: " << name << endl;
	
    
    char selectedPreset [32];
    char selectedPresetPath [32];
    sprintf (selectedPreset, "%s%i", mVizName, int(mSelectedPreset));
    sprintf(selectedPresetPath, "GUI/%s.xml", selectedPreset);
    
    char selectedPresetPath2 [32];
    sprintf(selectedPresetPath2, "GUI/%s_2.xml", selectedPreset);
    
    char selectedPresetPath3 [32];
    sprintf(selectedPresetPath3, "GUI/%s_3.xml", selectedPreset);
    
	if(name == "mPointLightDiffuseColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mPointLightDiffuseColor =  sampler->getColor();
    }
    if(name == "mPointLightSpecularColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mPointLightSpecularColor =  sampler->getColor();
    } else if(name == "mSpotLightDiffuseColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mSpotLightDiffuseColor =  sampler->getColor();
    } else if(name == "mSpotLightSpecularColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mSpotLightSpecularColor =  sampler->getColor();
    } else if(name == "mVertexColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mVertexColor =  sampler->getColor();
    } else if(name == "mMaterialDiffuseColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mMaterialDiffuseColor =  sampler->getColor();
    } else if(name == "mMaterialSpecularColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mMaterialSpecularColor =  sampler->getColor();
    } else if(name == "mMaterialEmissiveColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mMaterialEmissiveColor =  sampler->getColor();
    }else if(name == "mPulseColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mPulseColor =  sampler->getColor();
    }
    
    else if(name == "load"){
        ofLogNotice() << mVizIdName << "guiEvent:: loadSettings:" << selectedPresetPath << endl;
        gui->loadSettings(selectedPresetPath);
        gui2->loadSettings(selectedPresetPath2);
        gui3->loadSettings(selectedPresetPath3);
        
    } else if(name == "save"){
        
        ofLogNotice() << mVizIdName << "guiEvent:: Save Settings:" << selectedPresetPath << endl;
        gui->saveSettings(selectedPresetPath);
        gui2->saveSettings(selectedPresetPath2);
        gui3->saveSettings(selectedPresetPath3);
    } else if (name=="mSphereColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mSphereColor =  sampler->getColor();
        
    } else if (name=="mBoxColor"){
        ofxUIImageSampler *sampler = (ofxUIImageSampler *) e.widget;
        mBoxColor =  sampler->getColor();
        
    } else if(name==  "mSphereResolution"){
        
        ofSetSphereResolution(int(mSphereResolution));
    }
}
//--------------------------------------------------------------
void vizPhysics::keyPressed(int key){
    
    ofLogNotice() << mVizIdName << ":: keyPressed:" << key << " _id: " << _id <<endl;
    if ( _id == key-49){
        ofLogNotice() << mVizIdName << ":: gui->toggleVisible();" << endl;
        gui->toggleVisible();
        gui2->toggleVisible();
        gui3->toggleVisible();
    }
    
    switch (key) {
        case OF_KEY_UP:
            // setSpotlightCutOff is clamped between 0 - 90 degrees //
            spotLight.setSpotlightCutOff(spotLight.getSpotlightCutOff()+1);
            break;
        case OF_KEY_DOWN:
            // setSpotlightCutOff is clamped between 0 - 90 degrees //
            spotLight.setSpotlightCutOff(spotLight.getSpotlightCutOff()-1);
            break;
        case OF_KEY_RIGHT:
            // setSpotConcentration is clamped between 0 - 128 //
            spotLight.setSpotConcentration(spotLight.getSpotConcentration()+1);
            break;
        case OF_KEY_LEFT:
            // setSpotConcentration is clamped between 0 - 128 //
            spotLight.setSpotConcentration(spotLight.getSpotConcentration()-1);
            break;
        case 'a': toggleMouseAttract(); break;
            //case 'f': addRandomForce(FORCE_AMOUNT); break;
		case 32: addRandomForce(10 * 3); break;
		default:
			break;
	}
}
//--------------------------------------------------------------
void vizPhysics::mouseReleased(int x, int y, int button){
    addForce(ofVec3f(mouseX,0,mouseY),ofVec3f(0, mForceStrength, 0), 2.0f);
}
//--------------------------------------------------------------
void vizPhysics::mouseDragged(int x, int y, int button){
    addForce(ofVec3f(mouseX,0,mouseY),ofVec3f(0, mForceStrength, 0), 2.0f);
    
}
//--------------------------------------------------------------
void vizPhysics::mouseMoved(int x, int y ){
    
    //ofVec3f mouseRay = ofVec3f(x, y, -1);
    //mouseRay =camTop.screenToWorld(mouseRay,ofGetCurrentViewport());
    
    mouseX  = x-DECK_WIDTH/2;
    mouseY =  (-y+DECK_HEIGHT/2)*(-1);
    
    
    
}
//--------------------------------------------------------------
void vizPhysics::exit() {
    gui->saveSettings(mXmlPath);
    gui2->saveSettings(mXmlPath2);
    gui3->saveSettings(mXmlPath3);
    ofLogNotice() << "vizPhysics::exit() XML Settings saved." << endl;
    delete gui,gui2, gui3;
}
//--------------------------------------------------------------
void vizPhysics::drawMesh () {
    ofSetColor(0,0,0);
    for (int y = 0; y < gridResY; y++){
		for (int x = 0; x < gridResX; x++){
            int i = y * gridResX + x;
            
            //left top
            msa::physics::Particle3D *lt = particles[i];
            ofVec3f posLT = lt->getPosition();
            float factorLT = lt->getVelocity().length()*.2*mPulseIntensity;
            ofColor colLT =  mVertexColor;
            
            if (mbTogglePulse) mVertexColor - mPulseColor*factorLT;
            //if (x==15 && y == 15) cout << "factor LT: " << factorLT << endl;
            
            if(y < gridResY - 1 && x < gridResX - 1) {
                //right top
                msa::physics::Particle3D *rt = particles[i+gridResY];
                //right bottom
                msa::physics::Particle3D *rb = particles[i+1 + gridResY];
                //left bottom
                msa::physics::Particle3D *lb = particles[i+1];
                
                ofVec3f posRT = rt->getPosition();
                ofVec3f posRB = rb->getPosition();
                ofVec3f posLB = lb->getPosition();
             
                float factorRT =  rt->getVelocity().length()*.2*mPulseIntensity;
                ofColor colRT =  mVertexColor;
                if (mbTogglePulse) mVertexColor - mPulseColor*factorRT;
                
                float factorRB  =  rb->getVelocity().length()*.2*mPulseIntensity;
                ofColor colRB =  mVertexColor;
                if (mbTogglePulse) colRB = mVertexColor - mPulseColor*factorRB;
                
                float factorLB  =  lb->getVelocity().length()*.2*mPulseIntensity;
                ofColor colLB =  mVertexColor;
                 if (mbTogglePulse) colLB =  mVertexColor - mPulseColor*factorLB;
                
                
                
                if(mbToggleMesh){
                    if (mbToggleGLLines){
                        glBegin(GL_LINES);
                    } else if (mbToggleGLTriangles){
                        glBegin(GL_TRIANGLES);
                    }
                    
                    // - - - - - - - - - - - - - - - - - - - - - - FIRST TRIANGLE
                    if (mbPerVertexUVs) {
                        glTexCoord2f(0, 0);
                    }else {
                        glTexCoord2f(x/gridResX, y/gridResY);
                    }
                    
                    ofVec3f normLT = getNormal(posLT, posRT, posLB);
                    glNormal3f(normLT.x, normLT.y, normLT.z);
                    glVertex3f(posLT.x, posLT.y, posLT.z);
                    glColor3f(colLT.r/255.0,colLT.g/255.0, colLT.b/255.0);
                
                    // - - - - - - - - - - - - - - - -
                    if (mbPerVertexUVs) {
                        glTexCoord2f(1, 0);
                    }else {
                        glTexCoord2f((x+1)/gridResX, y/gridResY);
                        
                    }
                    ofVec3f normRT = getNormal(posRT, posRB, posLT);
                    glNormal3f(normRT.x, normRT.y, normRT.z);
                    glVertex3f(posRT.x, posRT.y, posRT.z);
                    glColor3f(colRT.r/255.0,colRT.g/255.0, colRT.b/255.0);
                    
                    // - - - - - - - - - - - - - - - -
                    if (mbPerVertexUVs) {
                        glTexCoord2f(1, 1);
                    }else {
                        glTexCoord2f((x+1)/gridResX, (y+1)/gridResY);
                    }
                    
                    ofVec3f normRB = getNormal(posRB, posLB, posRT);
                    glNormal3f(normRB.x, normRB.y, normRB.z);
                    glVertex3f(posRB.x, posRB.y, posRB.z);
                    glColor3f(colRB.r/255.0,colRB.g/255.0, colRB.b/255.0);
                    
                    // - - - - - - - - - - - - - - - - - - - - - - SECOND TRIANGLE
                    
                    if (mbPerVertexUVs) {
                        glTexCoord2f(1, 1);  
                    }else {
                        glTexCoord2f((x+1)/gridResX, (y+1)/gridResY);
                    }
                    
                    
                    glNormal3f(normRB.x, normRB.y, normRB.z);
                    glVertex3f(posRB.x, posRB.y, posRB.z);
                    glColor3f(colRB.r/255.0,colRB.g/255.0, colRB.b/255.0);
                    
                    
                    if (mbPerVertexUVs) {
                        glTexCoord2f(0, 1);
                    }else {
                        glTexCoord2f(x/gridResX, (y+1)/gridResY);
                        
                    }
                    ofVec3f normLB = getNormal(posLB, posLT, posRT);
                    glNormal3f(normLB.x, normLB.y, normLB.z);
                    glVertex3f(posLB.x, posLB.y, posLB.z);
                    glColor3f(colLB.r/255.0,colLB.g/255.0, colLB.b/255.0);
                    
                    if (mbPerVertexUVs) {
                        glTexCoord2f(0, 0);
                    }else {
                        glTexCoord2f(x/gridResX, y/gridResY);
                    }
                    
                    glNormal3f(normLT.x, normLT.y, normLT.z);
                    glVertex3f(posLT.x, posLT.y, posLT.z);
                    glColor3f(colLT.r/255.0,colLT.g/255.0, colLT.b/255.0);
                }
                glEnd();
            }
            
            
            if(mbToggleBoxes){
                ofPushStyle();
                ofSetColor(mBoxColor);
                ofBox(posLT, mBoxSize);
                ofPopStyle();
            }
            if(mbToggleSpheres){
                
                ofPushStyle();               
                ofSetColor(mSphereColor);
                ofSphere(posLT, mSphereSize);
                ofPopStyle();
            }
        }
    }
}




