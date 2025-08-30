#include "InputState.h"
#include "ofMain.h"

namespace ofxDualSense {
	//----------
	glm::vec2
		applyDeadZone(const glm::vec2& analogStickValue, float deadZone)
	{
		glm::vec2 result;

		if (abs(analogStickValue.x) < deadZone) {
			result.x = 0.0f;
		}
		else if (analogStickValue.x > 0) {
			result.x = ofMap(analogStickValue.x, deadZone, 1.0f, 0.0f, 1.0f);
		}
		else {
			result.x = ofMap(analogStickValue.x, -deadZone, -1.0f, 0.0f, -1.0f);
		}

		if (abs(analogStickValue.y) < deadZone) {
			result.y = 0.0f;
		}
		else if (analogStickValue.y > 0) {
			result.y = ofMap(analogStickValue.y, deadZone, 1.0f, 0.0f, 1.0f);
		}
		else {
			result.y = ofMap(analogStickValue.y, -deadZone, -1.0f, 0.0f, -1.0f);
		}
		
		return result;
	}

	//----------
	void
		InputState::draw(const ofRectangle & bounds) const
	{
		ofPushMatrix();
		{
			ofTranslate(bounds.getTopLeft());

			auto drawAnalogue2D = [this](const ofRectangle& bounds, glm::vec2 value, bool drawCenter, const std::string & label) {
				ofPushMatrix();
				ofPushStyle();
				{
					ofNoFill();
					ofDrawRectangle(bounds);
					ofFill();

					if (drawCenter) {
						ofPushStyle();
						{
							ofSetColor(100);
							ofDrawLine(bounds.getLeft(), bounds.y + bounds.height / 2
								, bounds.getRight(), bounds.y + bounds.height / 2);
							ofDrawLine(bounds.x + bounds.width / 2, bounds.getTop()
								, bounds.x + bounds.width / 2, bounds.getBottom());
						}
						ofPopStyle();
					}

					auto x = ofMap(value.x, -1, 1, bounds.getLeft(), bounds.getRight());
					auto y = ofMap(value.y, -1, 1, bounds.getBottom(), bounds.getTop());
					ofDrawCircle(x
						, y
						, 10);
					ofDrawBitmapString(label + ": " + ofToString(value, 2), bounds.getBottomLeft() + glm::vec2(5, -5));
				}
				ofPopStyle();
				ofPopMatrix();
			};

			auto drawAnalogue1D = [this](const ofRectangle& bounds, float value, bool isSigned, const std::string& label) {
				ofPushMatrix();
				ofPushStyle();
				{
					// fill
					ofPushStyle();
					{
						ofSetColor(100);
						ofFill();
						ofDrawRectangle({
							isSigned ? bounds.x + bounds.width / 2 : bounds.x
							, bounds.y
							, bounds.width * value
							, bounds.height
							});
					}
					ofPopStyle();

					// outline
					ofNoFill();
					ofDrawRectangle(bounds);
					ofFill();

					ofDrawBitmapString(label + ": " + ofToString(value, 2), bounds.getBottomLeft() + glm::vec2(5, -5));
				}
				ofPopStyle();
				ofPopMatrix();
			};

			auto drawButton = [this](const ofRectangle& bounds, bool value, const std::string& label) {
				ofPushStyle();
				{
					// fill
					if (value) {
						ofPushStyle();
						{
							ofSetColor(100);
							ofFill();
							ofDrawRectangle(bounds);
						}
						ofPopStyle();
					}

					// outline
					ofNoFill();
					ofDrawRectangle(bounds);

					// label
					ofDrawBitmapString(label, bounds.getBottomLeft() + glm::vec2(5, -5));
				}
				ofPopStyle();
			};

			drawAnalogue2D(ofRectangle(bounds.width * 0 / 4, 0, bounds.width / 4, bounds.height / 2)
				, this->DPad
				, true
				, "DPad");
			drawAnalogue2D(ofRectangle(bounds.width * 1 / 4, 0, bounds.width / 4, bounds.height / 2)
				, this->analogStickLeft
				, true
				, "analogStickLeft");
			drawAnalogue2D(ofRectangle(bounds.width * 2 / 4, 0, bounds.width / 4, bounds.height / 2)
				, this->analogStickRight
				, true
				, "analogStickRight");

			int iTouch = 0;
			for (const auto& touchIt : this->touchPadTouches) {
				drawAnalogue2D(ofRectangle(bounds.width * 6 / 8, bounds.height / 4 * iTouch, bounds.width / 4, bounds.height / 4)
					, touchIt.second
					, false
					, "touchPadTouches[" + ofToString((int) touchIt.first) + "]");
				iTouch++;
			}

			drawAnalogue1D(ofRectangle(0, bounds.height / 2, bounds.width / 3, bounds.height / 8)
				, this->leftTrigger
				, false
				, "leftTrigger");
			drawAnalogue1D(ofRectangle(0, bounds.height / 2 + bounds.height / 8, bounds.width / 3, bounds.height / 16)
				, this->leftTrigger
				, false
				, "leftTriggerFeedback");

			drawAnalogue1D(ofRectangle(bounds.width / 3, bounds.height / 2, bounds.width / 3, bounds.height / 8)
				, this->rightTrigger
				, false
				, "rightTrigger");
			drawAnalogue1D(ofRectangle(bounds.width / 3, bounds.height / 2 + bounds.height / 8, bounds.width / 3, bounds.height / 16)
				, this->rightTriggerFeedback
				, false
				, "rightTriggerFeedback");

			drawAnalogue1D(ofRectangle(bounds.width * 2 / 3, bounds.height / 2, bounds.width / 3, bounds.height / 16)
				, this->accelerometer.x
				, true
				, "accelerometer.x");
			drawAnalogue1D(ofRectangle(bounds.width * 2 / 3, bounds.height / 2 + bounds.height * 1 / 16, bounds.width / 3, bounds.height / 16)
				, this->accelerometer.y
				, true
				, "accelerometer.y");
			drawAnalogue1D(ofRectangle(bounds.width * 2 / 3, bounds.height / 2 + bounds.height * 2 / 16, bounds.width / 3, bounds.height / 16)
				, this->accelerometer.z
				, true
				, "accelerometer.z");
			drawAnalogue1D(ofRectangle(bounds.width * 2 / 3, bounds.height / 2 + bounds.height * 3 / 16, bounds.width / 3, bounds.height / 16)
				, this->gyroscope.x
				, true
				, "gyroscope.x");
			drawAnalogue1D(ofRectangle(bounds.width * 2 / 3, bounds.height / 2 + bounds.height * 4 / 16, bounds.width / 3, bounds.height / 16)
				, this->gyroscope.y
				, true
				, "gyroscope.y");
			drawAnalogue1D(ofRectangle(bounds.width * 2 / 3, bounds.height / 2 + bounds.height * 5 / 16, bounds.width / 3, bounds.height / 16)
				, this->gyroscope.z
				, true
				, "gyroscope.z");
			

			drawButton(ofRectangle(bounds.width / 16 * 8, bounds.height / 16 * 13, bounds.width / 16, bounds.height / 16)
				, this->buttons.square
				, "square");
			drawButton(ofRectangle(bounds.width / 16 * 9, bounds.height / 16 * 12, bounds.width / 16, bounds.height / 16)
				, this->buttons.triangle
				, "triangle");
			drawButton(ofRectangle(bounds.width / 16 * 10, bounds.height / 16 * 13, bounds.width / 16, bounds.height / 16)
				, this->buttons.circle
				, "circle");
			drawButton(ofRectangle(bounds.width / 16 * 9, bounds.height / 16 * 14, bounds.width / 16, bounds.height / 16)
				, this->buttons.cross
				, "cross");

			drawButton(ofRectangle(bounds.width * 0 / 4, bounds.height / 16 * 11, bounds.width / 4, bounds.height / 16)
				, this->buttons.leftBumper
				, "leftBumper");
			drawButton(ofRectangle(bounds.width * 1 / 4, bounds.height / 16 * 11, bounds.width / 4, bounds.height / 16)
				, this->buttons.rightBumper
				, "rightBumper");

			drawButton(ofRectangle(bounds.width * 0 / 8, bounds.height / 16 * 13, bounds.width / 8, bounds.height / 16)
				, this->buttons.select
				, "select");
			drawButton(ofRectangle(bounds.width * 0 / 8, bounds.height / 16 * 14, bounds.width / 8, bounds.height / 16)
				, this->buttons.menu
				, "menu");
			drawButton(ofRectangle(bounds.width * 0 / 8, bounds.height / 16 * 15, bounds.width / 8, bounds.height / 16)
				, this->buttons.playstationLogo
				, "playstationLogo");
			drawButton(ofRectangle(bounds.width * 1 / 8, bounds.height / 16 * 13, bounds.width / 8, bounds.height / 16)
				, this->buttons.touchPad
				, "touchPad");
			drawButton(ofRectangle(bounds.width * 1 / 8, bounds.height / 16 * 14, bounds.width / 8, bounds.height / 16)
				, this->buttons.mic
				, "mic");

			drawAnalogue1D(ofRectangle(bounds.width * 1 / 3, bounds.height / 16 * 15, bounds.width / 3, bounds.height / 16)
				, this->battery.level
				, false
				, "battery.level");
			{
				string batteryStatusString = "";
				if (battery.charging) {
					batteryStatusString += "[charging] ";
				}
				if (battery.fullyCharged) {
					batteryStatusString += "[fully charged]";
				}
				if (!batteryStatusString.empty()) {
					ofDrawBitmapString(batteryStatusString, bounds.width * 1 / 3, bounds.height / 16 * 15 + 10);
				}
			}
		}
		ofPopMatrix();
	}
}