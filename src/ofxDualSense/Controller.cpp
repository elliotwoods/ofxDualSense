#include "Controller.h"
#include "Exception.h"
#include "ofLog.h"


namespace ofxDualSense {
	//----------
	std::vector<std::shared_ptr<Controller>>
		Controller::listControllers() {
		std::vector<std::shared_ptr<Controller>> result;

		DS5W::DeviceEnumInfo devices[16];
		uint32_t controllerCount = 0;

		throwIfError(
			DS5W::enumDevices(devices
				, 16
				, &controllerCount)
		);

		for (uint32_t i = 0; i < controllerCount; i++) {
			try {
				result.push_back(std::make_shared<Controller>(devices[i], i));
			}
			catch (const Exception & e) {
				ofLogError("ofxDualSense") << "Failed to open #" << i << " : " << e.what();
			}
		}

		return result;
	}

	//----------
	Controller::Controller(DS5W::DeviceEnumInfo& device, size_t index)
		: index(index)
	{
		throwIfError(
			DS5W::initDeviceContext(&device, &this->deviceContext)
		);

		this->initialised = true;
	}

	//----------
	Controller::~Controller()
	{
		this->free();
	}

	//----------
	void
		Controller::update()
	{
		try {
			// INPUT
			{
				DS5W::DS5InputState inputState;
				memset(&inputState, 0, sizeof(inputState));

				throwIfError(
					DS5W::getDeviceInputState(&this->deviceContext, &inputState)
				);

				// Analog sticks
				{
					this->inputState.analogStickLeft.x = (float)inputState.leftStick.x / 127.0f;
					this->inputState.analogStickLeft.y = (float)inputState.leftStick.y / 127.0f;
					this->inputState.analogStickRight.x = (float)inputState.rightStick.x / 127.0f;
					this->inputState.analogStickRight.y = (float)inputState.rightStick.y / 127.0f;
				}

				// D-Pad
				{
					this->inputState.DPad.x = (inputState.buttonsAndDpad & DS5W_ISTATE_DPAD_LEFT ? -1.0f : 0.0f)
						+ (inputState.buttonsAndDpad & DS5W_ISTATE_DPAD_RIGHT ? 1.0f : 0.0f);
					this->inputState.DPad.y = (inputState.buttonsAndDpad & DS5W_ISTATE_DPAD_DOWN ? -1.0f : 0.0f)
						+ (inputState.buttonsAndDpad & DS5W_ISTATE_DPAD_UP ? 1.0f : 0.0f);
					this->inputState.DPad.x = (inputState.buttonsAndDpad & DS5W_ISTATE_DPAD_LEFT ? -1.0f : 0.0f)
						+ (inputState.buttonsAndDpad & DS5W_ISTATE_DPAD_RIGHT ? 1.0f : 0.0f);
				}

				// Triggers
				{
					this->inputState.leftTrigger = (float)inputState.leftTrigger / 255.0f;
					this->inputState.rightTrigger = (float)inputState.rightTrigger / 255.0f;
				}

				// Touch pad touches
				{
					this->inputState.touchPadTouches.clear();

					if (inputState.touchPoint1.x != 0) {
						this->inputState.touchPadTouches.push_back({
							(float)inputState.touchPoint1.x / 1920 * 2.0f - 1.0f // 2000 this is according to the lib docs
							, (float)inputState.touchPoint1.y / 1080 * 2.0f - 1.0f
							});
					}

					if (inputState.touchPoint2.x != 0) {
						this->inputState.touchPadTouches.push_back({
							(float)inputState.touchPoint2.x / 1920 * 2.0f - 1.0f // 2000 this is according to the lib docs
							, (float)inputState.touchPoint2.y / 1080 * 2.0f - 1.0f
							});
					}
				}

				// Button
				{
					this->inputState.buttons.leftBumper = inputState.buttonsA & DS5W_ISTATE_BTN_A_LEFT_BUMPER;
					this->inputState.buttons.rightBumper = inputState.buttonsA & DS5W_ISTATE_BTN_A_RIGHT_BUMPER;
					this->inputState.buttons.leftTrigger = inputState.buttonsA & DS5W_ISTATE_BTN_A_LEFT_TRIGGER;
					this->inputState.buttons.rightTrigger = inputState.buttonsA & DS5W_ISTATE_BTN_A_RIGHT_TRIGGER;
					this->inputState.buttons.leftStick = inputState.buttonsA & DS5W_ISTATE_BTN_A_LEFT_STICK;
					this->inputState.buttons.rightStick = inputState.buttonsA & DS5W_ISTATE_BTN_A_RIGHT_STICK;

					this->inputState.buttons.select = inputState.buttonsA & DS5W_ISTATE_BTN_A_SELECT;
					this->inputState.buttons.menu = inputState.buttonsA & DS5W_ISTATE_BTN_A_MENU;
					this->inputState.buttons.playstationLogo = inputState.buttonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO;
					this->inputState.buttons.touchPad = inputState.buttonsB & DS5W_ISTATE_BTN_B_PAD_BUTTON;
					this->inputState.buttons.mic = inputState.buttonsB & DS5W_ISTATE_BTN_B_MIC_BUTTON;

					this->inputState.buttons.square = inputState.buttonsAndDpad & DS5W_ISTATE_BTX_SQUARE;
					this->inputState.buttons.cross = inputState.buttonsAndDpad & DS5W_ISTATE_BTX_CROSS;
					this->inputState.buttons.triangle = inputState.buttonsAndDpad & DS5W_ISTATE_BTX_TRIANGLE;
					this->inputState.buttons.circle = inputState.buttonsAndDpad & DS5W_ISTATE_BTX_CIRCLE;
				}

				// IMU
				{
					// For now, we just pass these values through without normalising

					this->inputState.accelerometer.x = (float)inputState.accelerometer.x / (float) std::numeric_limits<uint16_t>::max();
					this->inputState.accelerometer.y = (float)inputState.accelerometer.y / (float)std::numeric_limits<uint16_t>::max();
					this->inputState.accelerometer.z = (float)inputState.accelerometer.z / (float)std::numeric_limits<uint16_t>::max();

					this->inputState.gyroscope.x = (float)inputState.gyroscope.x / (float)std::numeric_limits<uint16_t>::max();
					this->inputState.gyroscope.y = (float)inputState.gyroscope.y / (float)std::numeric_limits<uint16_t>::max();
					this->inputState.gyroscope.z = (float)inputState.gyroscope.z / (float)std::numeric_limits<uint16_t>::max();
				}

				// Battery
				{
					this->inputState.battery.charging = inputState.battery.chargin;
					this->inputState.battery.fullyCharged = inputState.battery.fullyCharged;
					this->inputState.battery.level = (float)inputState.battery.level / 255.0f;
				}

				this->inputState.headPhoneConnected = inputState.headPhoneConnected;
				this->inputState.leftTriggerFeedback = (float)inputState.leftTriggerFeedback / 255.0f;
				this->inputState.rightTriggerFeedback = (float)inputState.rightTriggerFeedback / 255.0f;
			}

			// OUTPUT
			{
				DS5W::DS5OutputState outputState;
				memset(&outputState, 0, sizeof(outputState));

				outputState.leftRumble = (uint8_t)(this->outputState.hardRumble * 255.0f);
				outputState.rightRumble = (uint8_t)(this->outputState.softRumble * 255.0f);
				outputState.microphoneLed = this->outputState.micLED;
				outputState.disableLeds = this->outputState.disableLEDs;
				outputState.playerLeds = this->outputState.playerLEDs;
				{
					outputState.lightbar.r = this->outputState.lightbar.r;
					outputState.lightbar.g = this->outputState.lightbar.g;
					outputState.lightbar.b = this->outputState.lightbar.b;
				}

				throwIfError(
					DS5W::setDeviceOutputState(&this->deviceContext, &outputState)
				);
			}
		}
		catch (const Exception& e) {
			ofLogError("ofxDualSense") << "Failed to update #" << this->index << " : " << e.what();
		}
	}

	//----------
	const InputState &
		Controller::getInputState() const
	{
		return this->inputState;
	}

	//----------
	void
		Controller::setOutputState(const OutputState& outputState)
	{
		this->outputState = outputState;
	}

	//----------
	std::string 
		Controller::getDevicePath() const
	{
		std::wstring wDevicePath(this->deviceContext._internal.devicePath);
		std::string devicePath(wDevicePath.begin(), wDevicePath.end());
		return devicePath;
	}

	//----------
	DS5W::DeviceConnection
		Controller::getConnectionType() const
	{
		return this->deviceContext._internal.connection;
	}

	//----------
	bool
		Controller::isConnected() const
	{
		return this->deviceContext._internal.connected;
	}

	//----------
	std::string
		Controller::getStatusString() const
	{
		std::stringstream ss;
		ss << "#" << this->index
			<< " (" << toString(this->getConnectionType()) << " : " << (this->isConnected() ? "CONNECTED" : "DISCONNECTED") << ") : "
			<< this->getDevicePath();
		return ss.str();
	}

	//----------
	void
		Controller::free()
	{
		if (this->initialised) {
			DS5W::freeDeviceContext(&this->deviceContext);
			this->initialised = false;
		}
	}

	//----------
	std::string
		toString(const DS5W::DeviceConnection& connection)
	{
		switch (connection) {
		case DS5W::DeviceConnection::USB:
			return "USB";
		case DS5W::DeviceConnection::BT:
			return "BT";
		default:
			return "Unknown";
		}
	}
}