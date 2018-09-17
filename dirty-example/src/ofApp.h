#pragma once

#include "ofMain.h"
#include "ofxHalftoneShader.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

    ofFbo fbo , outFbo;
    ofxHalftoneShader ht;
    ofVideoGrabber cam;
};
