#pragma once

#include "action.h"

#include <forward_list>
#include <iostream>

class otp_help : public action {

	bool suppressed_match_check{ false };

	const std::forward_list<std::unique_ptr<action>>& all_actions;

protected:
	void act() override {
		if (data->action_command_sequence.size() != 1) {
			standard_logger().warn("Unexpected arguments.");
		}
		standard_logger().info("Showing info...");
		std::cout << "Usage:\n\totp [-C path/to/config.json ] [action_name] [action params]\n\n";
		std::cout << "\t-C path/to/config.json: Specify a custom path where the configuration file is located. NOTE: Relative paths to pad files defined in config will be treated as relative to the directory where the config file is located. Paths given as arguments that should be written into config will be converted into equivalent paths relativ to the directory where the config is located.\n";
		std::cout << "\n";
		std::cout << "The following actions are available:\n";
		for (const auto& ptr : all_actions) {
			std::cout << "\n";
			std::cout << "\tName: " << ptr->name()
				<< "\tShort description: " << ptr->short_description()
				<< "\tSyntax and behaviour:\n" << ptr->help();
		}
		standard_logger().info("Showing info ...DONE.");
	}

public:

	otp_help(const std::forward_list<std::unique_ptr<action>>& all_actions) : all_actions(all_actions) {}

	bool match() const override {
		return suppressed_match_check || (data->action_command_sequence[0] == "help");
	}

	inline void suppress_match_check() { suppressed_match_check = true; }

	std::string name() const override {
		return "help";
	}

	std::string short_description() const override {
		return "SHOW HELP";
	}

	std::string help() const override {
		return ""
	}

};
