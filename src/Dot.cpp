//
//  Dot.cpp
//  Sketch06
//
//  Created by Moc O'Matics on 25/04/2013.
//
//

#include "Dot.h"
//--------------------------------------------------------------
void Dot::setup(int id){
    ID = id;
    //cout << "Dot::setup() ID: " << ID << endl;
    target = home;
    current = home;
    
    mbEasing = false;
    mEaseProgress = 1.0;
    

}
//--------------------------------------------------------------
void Dot::setVideo(string path){
    ofLogNotice() << "Dot::setVideo() ID: " << ID << " path: " << path << endl;
    mbVideo = true;
    mVideoPlayer =  new ofVideoPlayer();
    mVideoPlayer->setPixelFormat(OF_PIXELS_RGBA);
//    mVideoPlayer->getTextureReference().clear();
    mVideoPlayer->loadMovie(path);
//    mVideoPlayer->setSpeed(1);
//    mVideoPlayer->play();
    //mVideoPlayer->firstFrame();
    mVideoPlayer->setFrame(0);
    mVideoPlayer->update();
    
    width = mVideoPlayer->width;
    height = mVideoPlayer->height;
    ofLogNotice() << "Dot::setVideo() ID: " << ID << " width: " << width << endl;
    mVideoPlayer->setLoopState(OF_LOOP_NONE);
}
//--------------------------------------------------------------
void Dot::update(){
    float rot;
    if(mbVideo) {
        mVideoPlayer->setSpeed(mMovieSpeed);
        /*
        if(mVideoPlayer->isLoaded() && !mbLoaded){
            mVideoPlayer->firstFrame();
            mbLoaded = true;
        }*/
        if(mVideoPlayer -> isPlaying()) mVideoPlayer->update();
    }
}

//--------------------------------------------------------------
void Dot::draw(){
    //ofLogNotice() << "draw Dot " << ID << endl;
    if (mbVideo) {
        ofSetColor(ofColor(255,255,255));
    } else {
     ofSetColor(color);
    }
    //ofSphere(current, 20.0f);
    glPushMatrix();
    ofTranslate(target);
    //ofRotateZ(rotation);
    if (mbVideo) {
       // float w =  width * mEaseProgress;
        //float h = height*mEaseProgress;
        //if (mVideoPlayer->isPlaying())  mVideoPlayer->draw(-w/2.0,-h/2.0,w,h );
        
        mVideoPlayer->draw(home.x - width/2, home.y-height/2);
    } else {
        ofEllipse(ofPoint(), width * mEaseProgress, height*mEaseProgress);
    }
    //ofRect(mNeedleOffset, -height/2.0, width, height);
    glPopMatrix();
    
}

//--------------------------------------------------------------
void Dot::setIdle(bool idle){
    //    cout << "Dot::setIdle() ID: " << idle << endl;
    mbIdle = idle;
}
//--------------------------------------------------------------
void Dot::setVideo(bool bVideo){
    //    cout << "Dot::setVideo() ID: " << bVideo << endl;
    mbVideo = bVideo;
}
//--------------------------------------------------------------
void Dot::setSpeed(float speed){
    //    cout << "Dot::setSpeed() ID: " << idle << endl;
    mMovieSpeed = speed;
}
//--------------------------------------------------------------
void Dot::setIdleRotation(float rot){
    //    cout << "Dot::setTarget() ID: " << p << endl;
    mIdleRotation = rot;
}
//--------------------------------------------------------------
void Dot::setTarget(ofVec3f p){
    //cout << "Dot::setTarget(): " << p << endl;
    target = p;
}
//--------------------------------------------------------------
void Dot::onComplete(float* arg) {
	cout << "Dot :: onComplete () ID: " << ID << endl;
    mbEasing = false;
    mEaseProgress = 0.0;
    // I am done, remove me ...
}
//--------------------------------------------------------------
void Dot::playVideo() {
    ofLogNotice() << "Dot::playVideo()" << endl;
    if (mbVideo || mVideoPlayer->isPlaying())
        mVideoPlayer-> play();
    
    /*
    if (!mbEasing) {
        mbEasing = true;
        // add the complete listener again so that it will fire again, creating a loop //
        Tweenzor::add( &mEaseProgress, mEaseProgress, 1.0f, 0.0f, 2.5f, EASE_IN_OUT_ELASTIC );
        Tweenzor::addCompleteListener( Tweenzor::getTween(&mEaseProgress), this, &Dot::onComplete);
    }
    */

}
void Dot::pauseVideo() {
    ofLogNotice() << "Dot::pauseVideo()" << endl;
    if (mbVideo) mVideoPlayer->stop();
    /*
     if (!mbEasing) {
     mbEasing = true;
     // add the complete listener again so that it will fire again, creating a loop //
     Tweenzor::add( &mEaseProgress, mEaseProgress, 1.0f, 0.0f, 2.5f, EASE_IN_OUT_ELASTIC );
     Tweenzor::addCompleteListener( Tweenzor::getTween(&mEaseProgress), this, &Dot::onComplete);
     }
     */
    
}
void Dot::setHome(ofVec3f h){
    //photofLogNotice() << "Dot::setHome() ID: " << h << endl;
    home = h;
}
void Dot::setWidth(float w){
   //cout << "Dot::setWidth() ID: " << w << endl;
    width = w;
}
void Dot::setHeight(float h){
   //cout << "Dot::setHeight() ID: " << h << endl;
    height = h;
}
void Dot::setColor(ofColor c){
    //    cout << "Dot::setColor() ID: " << h << endl;
    color = c;
}
void Dot::exit(){
    delete mVideoPlayer;
}