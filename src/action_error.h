#pragma once

#include <string>
#include <exception>

class action_error : public std::exception {
	std::string message;
public:
	static void assert_true(bool condition, const action_error& exception) {
		if (!condition) throw exception;
	}

	action_error(const std::string& message) : message(std::string("action error: ") + message) {}
	action_error(const char* message) : action_error(std::string(message)) {}
	action_error(const action_error&) = default;

	const char* what() const noexcept override {
		return message.c_str();
	}
};
