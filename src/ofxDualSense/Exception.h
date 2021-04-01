#pragma once
#include <DualSenseWindows/DSW_Api.h>
#include <exception>
#include <string>

namespace ofxDualSense {
	void throwIfError(const DS5W_ReturnValue&);

	class Exception : public std::exception {
	public:
		Exception(const std::string&);
		Exception(const DS5W_ReturnValue&);
		const char* what() const override;
	protected:
		std::string message;
	};
}