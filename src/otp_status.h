#pragma once

#include "action.h"

class otp_status : public action {
	bool match() const override {
		return data->application_start_arguments[1] == "status";
	}

	std::string name() const override {
		return "status";
	}

	std::string short_description() const override {
		return "SHOW CONFIG STATUS";
	}

	std::string help() const override {
		return "Syntax: otp status";
	}

protected:
	void act() override {
		action_error::assert_true(data->application_start_arguments.size() == 2, "Too many arguments.");
		if (!data->exists_config_file()) {
			std::cout << "The current directory has not been set up for One-Time-Pad: File config.json is missing.\n You may create one by typing:\n   otp init\n";
			return;
		}
		auto config_json{ std::ifstream(data->config_file_path()) };
		action_error::assert_true(config_json.good(), "Could not read config.json although there exists such a file.");

		config_json.close();
	}
};
