#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cout<<endl;
    ofImage img;
    img.load("test.png");

    fbo.allocate(img.getWidth(),img.getHeight());
    outFbo.allocate(img.getWidth(),img.getHeight());
    fbo.begin();
    ofClear(0);
    img.draw(0,0);
    fbo.end();

    cam.setup(fbo.getWidth(), fbo.getHeight());

    ht.setup();
}   

//--------------------------------------------------------------
void ofApp::update(){

    float x = ofMap(mouseX, 0, ofGetWidth(), .5, 100., true);
    float y = ofMap(mouseY, ofGetHeight(),0, 0., 10., true);
    ofSetWindowTitle("fps:"+ofToString(ofGetFrameRate(),2)+"  dotSize:"+to_string(x) + "  ratio:"+to_string(y));
    ht.setDotSize(x);
    ht.setRatio(y);


    static bool mode=false;
    if(ofGetKeyPressed(' ')) mode  = !mode;

    if(mode){
        cam.update();
        if(cam.isFrameNew()) ht.process(cam.getTexture(),outFbo);
    }else{
        ht.process(fbo.getTexture(),outFbo);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.draw(0,0);
    outFbo.draw(fbo.getWidth()+10, 0);

}

