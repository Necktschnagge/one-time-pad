


#include "internal_error.h"
#include "global_data.h"
#include "logger.h"

#include "nlohmann/json.hpp"


#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <exception>
#include <fstream>
#include <memory>
#include <forward_list>
#include <numeric>
#include <list>
#include <string_view>



/*
{
 pads :
	[
		{
			name: "password_pad",
			content: [
				{
					id : 0,
					path : "C:/test.padkey"
					size : 1024
				},
				{
					id : 1,
					path : "C:/test2.padkey"
					size : 100
				}
				],
			pointer: {
				file: 0,
				byte: 0
				}
		},
		{

		}
	]
config: {
	auto_delete_used_keyfiles : false
	auto_increase_keypad_pointer : true
}
}


*/


/**
 otp init -> create fresh config.json
 otp status -> check status of config.json
 otp [encrypt|decrypt] source_file_path destination_file_path pad_name
 otp register pad_name keyfile_path -> add new keyfile to a

 -C ./path/to/another_file_name.json


otp [-D | -F | -DF ] [action_name] [action params]



*/



class action_error : public std::exception {
	std::string message;
public:
	static void assert_true(bool condition, const action_error& exception) {
		if (!condition) throw exception;
	}

	action_error(const std::string& message) : message(message) {}
	action_error(const char* message) : action_error(std::string(message)) {}
	action_error(const action_error&) = default;



	const char* what() const noexcept override {
		return message.c_str();
	}
};





class action {
public:

	inline static const global_data* data{ nullptr };
private:
	std::size_t id;
	inline static std::size_t new_free_id{ 0 };
protected:

	void assert_match() {
		if (!match()) throw action_error("This action does not match the input");
	}
	virtual void act() = 0;

public:
	action() : id(new_free_id++) {}

	virtual bool match() const = 0;

	inline void run() { assert_match(); act(); }

	virtual std::string description() const = 0;

	inline bool operator==(const action& another) const { return another.id == id; }
};


class otp_init : public action {
public:
	bool match() const override {
		return data->application_start_arguments[1] == "init";
	}

	std::string description() const override {
		return "INIT CONFIG JSON";
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

class otp_status : public action {
	bool match() const override {
		return data->application_start_arguments[1] == "status";
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


int cli(int argc, char** argv) {
	try {
		standard_logger().info("This is Fussel One-Time-Pad 1.0");
		// init global data:
		global_data data{ argc, argv };
		// give calss action access to global data:
		action::data = &data;
		// list all implemented actions:
		std::forward_list<std::unique_ptr<action>> all_actions;
		all_actions.emplace_front(new otp_init());

		// check for actions matching the input:
		std::forward_list<action*> matching_actions;
		std::for_each(all_actions.cbegin(), all_actions.cend(), [&](const auto& p_action) { if (p_action->match()) matching_actions.push_front(p_action.get()); });

		// Check for missing or ambiguous actions:
		action_error::assert_true(!matching_actions.empty(), "Invalid input. There is no otp action matching your input.");
		internal_error::assert_true(std::next(matching_actions.cbegin()) == matching_actions.cend(),
			std::string("More than one action matched for input sequence which is:\n  ")
			+ std::accumulate(data.application_start_arguments.cbegin(), data.application_start_arguments.cend(), std::string(), [](const auto& l, const auto& r) { return l + " " + r; })
			+ "\nThe matching actions are {"
			+ std::accumulate(matching_actions.cbegin(), matching_actions.cend(), std::string(), [](const auto& str, const auto& ptr) {
				return str + (str.empty() ? "" : ", ") + ptr->description();
				})
			+ "}."
					);

		// When execution is here: there is exactly one matching action. Execute it:
		matching_actions.front()->run();

		// Done:
		std::cout << "Successfully finished.\n";
	}
	catch (const action_error& e) {
		std::cerr <<
			"\n" << 
			"An error occurred:\n\n" << 
			e.what() <<
			"\n\nAborted.";
		return 1;
	}
	catch (const bad_argument& e) {
		std::cerr << e.what();
	}
	catch (const internal_error& e) {
		std::cerr << e.what();
		return 2;
	}
	catch (...) {
		std::cerr << "Unknown exception caught. Please report to the developer.";
		//### rethrow so that one can see the output of exception
		return 5;
	}
	return 0;
}

int main(int argc, char** argv)
{
	init_logger();

	return cli(argc, argv);


	// run debugger to find exception cause
	// visual studio if debug mode does not hold at the debug points, go to project -> properties -> c/c++ -> general -> debug information format
	// change it to program dtabase for edit and continue
}
