#pragma once

#include "action.h"
#include "const_strings.h"

#include <fstream>
#include <iostream>

class otp_init : public action {
public:
	bool match() const override {
		return data->application_start_arguments[1] == "init";
	}

	std::string name() const override {
		return "config";
	}

	std::string short_description() const override {
		return "INIT CONFIG JSON";
	}

	std::string help() const override {
		return "Syntax: otp init";
	}

protected:
	void act() override {
		action_error::assert_true(data->application_start_arguments.size() == 2, "Too many arguments.");
		action_error::assert_true(!data->exists_config_file(), "File config.json already found.");
		auto config_json{ std::ofstream(data->config_file_path()) };
		action_error::assert_true(config_json.good(), "Could not create config.json.");
		config_json << const_strings::EMPTY_CONFIG;
		config_json.close();
		std::cout << "Successfully created a fresh config.json:\n\t" << data->config_file_path() << '\n';
	}

};
