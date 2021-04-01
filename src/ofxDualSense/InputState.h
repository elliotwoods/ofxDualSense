#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <DualSenseWindows/DS5State.h>
#include "ofRectangle.h"

namespace ofxDualSense {
	struct InputState{
		/// <summary>
		///  A utilty function for drawing the data
		/// </summary>
		/// <param name="bounds">Area of screen to draw into</param>
		void draw(const ofRectangle& bounds) const;

		glm::vec2 analogStickLeft;
		glm::vec2 analogStickRight;
		glm::vec2 DPad;

		float leftTrigger;
		float rightTrigger;

		std::vector<glm::vec2> touchPadTouches;

		struct {
			bool square;
			bool cross;
			bool circle;
			bool triangle;

			bool leftBumper;
			bool rightBumper;
			bool leftTrigger;
			bool rightTrigger;
			bool leftStick;
			bool rightStick;

			bool select;
			bool menu;
			bool playstationLogo;
			bool touchPad;
			bool mic;
		} buttons;

		glm::vec3 accelerometer;
		glm::vec3 gyroscope;

		struct {
			bool charging;
			bool fullyCharged;
			float level;
		} battery;

		bool headPhoneConnected;
		float leftTriggerFeedback;
		float rightTriggerFeedback;
	};
}