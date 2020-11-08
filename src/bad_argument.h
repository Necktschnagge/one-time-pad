#pragma once

#include <string>
#include <exception>

class bad_argument : public std::exception {
	std::string message;
public:
	static void assert_true(bool condition, const bad_argument& exception) {
		if (!condition) throw exception;
	}

	bad_argument(const std::string& message) : message(std::string("bad argument: ") + message) {}
	bad_argument(const char* message) : bad_argument(std::string(message)) {}
	bad_argument(const bad_argument&) = default;


	const char* what() const noexcept override {
		return message.c_str();
	}
};
