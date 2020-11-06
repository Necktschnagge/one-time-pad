#pragma once

#include "action.h"

class otp_help : public action {

	bool suppressed_match_check{ false };

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

public:

	bool match() const override {
		return suppressed_match_check || (data->application_start_arguments[1] == "help");
	}

	inline void suppress_match_check() { suppressed_match_check = true; }

	std::string description() const override {
		return "SHOW CONFIG STATUS";
	}

};
