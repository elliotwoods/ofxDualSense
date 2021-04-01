#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "ofColor.h"
#include <DualSenseWindows/DS5State.h>

namespace ofxDualSense {
	struct OutputState {
		ofColor lightbar{ 0, 0, 0 };
		
		float hardRumble = 0.0f;
		float softRumble = 0.0f;

		DS5W::LedBrightness ledBrightness = DS5W::LedBrightness::MEDIUM;
		DS5W::MicLed micLED = DS5W::MicLed::OFF;
		DS5W::PlayerLeds playerLEDs{ 0, false, DS5W::LedBrightness::MEDIUM };
		bool disableLEDs = false;
	};
}