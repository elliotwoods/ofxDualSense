#include "pch_ofApp.h"

#include "ofApp.h"
#include "ofxDualSense.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetWindowTitle("ofxDualSense example");

	this->controllers = ofxDualSense::Controller::listControllers();
	for (auto& controller : controllers) {
		std::cout << controller->getStatusString() << std::endl;
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	for (auto& controller : this->controllers) {
		controller->update();

		auto inputState = controller->getInputState();
		ofxDualSense::OutputState outputState;
		outputState.hardRumble = inputState.leftTrigger;
		outputState.softRumble = inputState.rightTrigger;

		outputState.playerLEDs.bitmask = (ofGetFrameNum() / 4) % 32;

		// Actually this is always equal 2 at the moment - hopefully we can detect touches up/down later
		if (inputState.touchPadTouches.size() >= 2)
		{
			outputState.lightbar.r = ofMap(inputState.touchPadTouches[0].x, -1, 1, 0, 255);
			outputState.lightbar.g = ofMap(inputState.touchPadTouches[0].y, -1, 1, 0, 255);
			outputState.lightbar.b = ofMap(inputState.touchPadTouches[1].x, -1, 1, 0, 255);
			outputState.ledBrightness = inputState.touchPadTouches[1].y > 0
				? DS5W::LedBrightness::HIGH
				: DS5W::LedBrightness::LOW;
		}
		

		controller->setOutputState(outputState);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	size_t i = 0;
	for (const auto& controller : this->controllers) {
		const auto & inputState = controller->getInputState();
		inputState.draw(ofRectangle(0, i * 300, ofGetWidth(), 300));
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	// Free all controllers before exiting
	this->controllers.clear();
}