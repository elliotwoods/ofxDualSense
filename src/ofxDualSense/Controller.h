#pragma once

#include "InputState.h"
#include "OutputState.h"

#include "ofEvents.h"

#include <string>
#include <vector>

#include <DualSenseWindows/IO.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/Helpers.h>

namespace ofxDualSense {
	class Controller {
	public:
		static std::vector<std::shared_ptr<Controller>> listControllers();

		Controller(DS5W::DeviceEnumInfo&, size_t index);
		~Controller();

		void update();
		void reconnect();
		bool isFrameNew() const;

		const InputState & getInputState() const;
		void setOutputState(const OutputState&);

		std::string getDevicePath() const;
		DS5W::DeviceConnection getConnectionType() const;
		bool isConnected() const;
		std::string getStatusString() const;

		void free();
	protected:
		bool frameIsNew = false;
		bool initialised = false;
		size_t index;
		DS5W::DeviceContext deviceContext;
		
		InputState inputState;
		OutputState outputState;
	};

	std::string toString(const DS5W::DeviceConnection&);
}