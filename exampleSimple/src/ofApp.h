#pragma once

#include "ofMain.h"
#include "ofxDualSense.h"

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	vector<shared_ptr<ofxDualSense::Controller>> controllers;
};
