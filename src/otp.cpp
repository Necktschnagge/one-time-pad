


#include "internal_error.h"
#include "action_error.h"
#include "global_data.h"
#include "logger.h"

#include "action.h"
#include "otp_init.h"
#include "otp_status.h"

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


otp [-C path/to/config.json ] [action_name] [action params]



*/






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
		all_actions.emplace_front(new otp_status());
		all_actions.emplace_front(new otp_help());

		// check for actions matching the input:
		std::forward_list<action*> matching_actions;
		std::for_each(all_actions.cbegin(), all_actions.cend(), [&](const auto& p_action) { if (p_action->match()) matching_actions.push_front(p_action.get()); });

		// Check for missing or ambiguous actions:
		try {
			action_error::assert_true(!matching_actions.empty(), "Invalid input. There is no otp action matching your input.");
		}
		catch (const action_error& e) {
### view help here create a new help object and insert it here?
		}
		internal_error::assert_true(std::next(matching_actions.cbegin()) == matching_actions.cend(),
			std::string("More than one action matched for input sequence which is:\n  ")
			+ std::accumulate(data.application_start_arguments.cbegin(), data.application_start_arguments.cend(), std::string(), [](const auto& l, const auto& r) { return l + " " + r; })
			+ "\nThe matching actions are {"
			+ std::accumulate(matching_actions.cbegin(), matching_actions.cend(), std::string(), [](const auto& str, const auto& ptr) {
				return str + (str.empty() ? "" : ", ") + ptr->short_description();
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
