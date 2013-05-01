//
//  vizPhysics.h
//  Sketch06
//
//  Created by Moc O'Matics on 26/04/2013.
//
//

#ifndef __Sketch06__vizPhysics__
#define __Sketch06__vizPhysics__

#include <iostream>
#include "ofMain.h"
#include "Dot.h"
#include "ofxUI.h"
#include "ofxTuio.h"
#include "MSAPhysics3D.h"

class vizPhysics {
public:
    void setup();
    void draw();
    void drawCopy();
    void exit();
    void keyPressed(int key);
    void update();
    void update(ofVec3f target);
    void update(list<ofxTuioCursor*> cursors);
    void mouseReleased(int x, int y, int button);
    void mouseMoved(int x, int y );
    void mouseDragged (int x, int y, int button );
    void setTextureRef (ofTexture* tex);
    // - - - - - - BASE GUI
    int _id;
    void drawMesh();
    void drawScene();
    //ofRectangle viewMain;
//    ofFbo rgbaFbo;
//    ofFbo textureFbo;
    // - - - - - - Physics
    void addParticleGrid();
    ofVec3f getNormal (ofVec3f a, ofVec3f b, ofVec3f c);
    ofTexture* texRef;
    
    ofShader shader;
	ofMaterial material;
    
    //LIGHTING
    ofLight pointLight;
	ofLight spotLight;
    
    ofLight spotLightMouse;
	
    float  mSpotLightPosX, mSpotLightPosY, mSpotLightPosZ;
    float  mSpotLightRotX, mSpotLightRotY, mSpotLightRotZ;
    float  mPointLightPosX, mPointLightPosY, mPointLightPosZ;
    
    ofColor color;
  
    int mouseX;
    int mouseY;
    float gridResX;
    float gridResY;
    
    // - - - - - - CAMERA
    ofEasyCam cam; // add mouse controls for camera movement
    ofCamera camTop;
    bool mbCamTop;//, mbToggleFboTexture;
  
    float mBoxSize, mSphereSize, mMaterialShininess, mSphereResolution, mCamTopY, mOffsetX, mPulseIntensity;
   
    bool mbBlendModeAdd, mbTogglePulse,
    mbToggleLighting, mbTogglePointLight, mbToggleSpotLight, mbToggleSpotLightMouse,
    mbToggleMaterial, mbToggleTexture1, mbToggleTexture2, mbPerVertexUVs, mbToggleShader, mbToggleBoxes, mbToggleSpheres, mbToggleMesh, mbToggleGLLines, mbToggleGLTriangles;
    
    ofImage	image, image2, mOverLay;
    ofColor mSpotLightDiffuseColor, mSpotLightSpecularColor,
            mPointLightDiffuseColor, mPointLightSpecularColor,
            mVertexColor, mMaterialDiffuseColor, mMaterialSpecularColor, mMaterialEmissiveColor, mMaterialAmbientColor,
            mBoxColor, mSphereColor, mPulseColor;
    
    //- - - - - - - OFX UI - - - - - - - -
    void guiEvent(ofxUIEventArgs &e);
    ofxUICanvas *gui;
    ofxUICanvas *gui2;
    ofxUICanvas *gui3;

    
    ofImage m_imgGradient;
    char mVizIdNameCount[32];
    int smVizCount;
    int mPresetCount;
    float mSelectedPreset;
    
    char mVizName[32];
    char mVizIdName[32];
    char mXmlPath[32];
    char mXmlPath2[32];
    char mXmlPath3[32];
    char mVizIdWeightName[32];
    

    // - - - - - - - - PHYSICS
    msa::physics::World3D		physics;
    msa::physics::Particle3D    mouseNode;
    msa::physics::Particle3D    attractor;
    vector<msa::physics::Particle3D*> particles;
    
    void addRandomForce(float f);
    void toggleMouseAttract();
    void addForce(ofVec3f v, ofVec3f dir, float f);
    void loadSettings (int id);
    
    float rotSpeed		= 0;
    float mouseMass		= 1;
    bool mouseAttract	= false;
    int	 forceTimer		= false;
    float mGravity, mDrag;
    float mForceRadius, mForceStrength;
   
};

#endif /* defined(__Sketch06__vizPhysics__) */
