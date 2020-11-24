#pragma once

#include <string>
#include <exception>

class internal_error : public std::exception {
	std::string message;

	inline static const std::string INTERNAL_ERROR{ "Internal Error:  " };
	inline static const std::string PLEASE_REPORT{ "  Please report this to the developers: Open an issue at https://github.com/Necktschnagge/one-time-pad/issues and copy-paste the error message. If possible describe the scenario which led to this error." };
public:
	static void assert_true(bool condition, const internal_error& exception) {
		if (!condition) throw exception;
	}

	internal_error(const std::string& message) : message(INTERNAL_ERROR + message + PLEASE_REPORT) {}
	internal_error(const char* message) : internal_error(std::string(message)) {}
	internal_error(const internal_error&) = default;



	const char* what() const noexcept override {
		return message.c_str();
	}
};
