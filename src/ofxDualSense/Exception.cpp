#include "Exception.h"

namespace ofxDualSense {
	//----------
	void
		throwIfError(const DS5W_ReturnValue& returnValue)
	{
		if (returnValue != DS5W_ReturnValue::OK) {
			throw(Exception(returnValue));
		}
	}

	//----------
	Exception::Exception(const std::string& message)
		: message(message)
	{

	}

	//----------
	Exception::Exception(const DS5W_ReturnValue& returnValue)
	{
		switch (returnValue) {
		case DS5W_ReturnValue::E_INSUFFICIENT_BUFFER:
		case DS5W_ReturnValue::E_EXTERNAL_WINAPI:
		case DS5W_ReturnValue::E_STACK_OVERFLOW:
		case DS5W_ReturnValue::E_INVALID_ARGS:
		case DS5W_ReturnValue::E_CURRENTLY_NOT_SUPPORTED:
		case DS5W_ReturnValue::E_DEVICE_REMOVED:
		case DS5W_ReturnValue::E_BT_COM:
		case DS5W_ReturnValue::E_UNKNOWN:
		default:
			this->message = "Unknown error";
			break;
		}
	}

	//----------
	const char*
		Exception::what() const
	{
		return this->message.c_str();
	}

}