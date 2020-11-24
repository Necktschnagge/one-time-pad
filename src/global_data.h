#pragma once

#include "logger.h"
#include "bad_argument.h"
#include "const_strings.h"

#include <filesystem>
#include <vector>
#include <string>

/**
* @brief Contains all data and structure instances appearing global for OTP
*/
class global_data {

	std::filesystem::path _config_file_path; // absolute path to config file, weakly_cannonical, i.e. containing no .. or .

	std::vector<std::string> _action_command_sequence; // commands regarding the action to be executed (no executable path, no -C option anymore)

	std::filesystem::path executable_path; // path to the executable which is running

	std::filesystem::path working_directory; // current working directory

	inline std::filesystem::path pad_root_directory() { return  _config_file_path.parent_path(); } // absolute path to the root directory of the pad, which is the directory where the config.json is located

public:

	const std::vector<std::string> application_start_arguments; // all arguments as passed via the commandline

	inline const std::filesystem::path& config_file_path() const { return _config_file_path; }

	inline bool exists_config_file() const { return std::filesystem::exists(config_file_path()); }

	const std::vector<std::string>& action_command_sequence{ _action_command_sequence };

private:

	std::vector<std::string> decode_command_line_input(int argc, char** argv) {
		auto result{ std::vector<std::string>(argc) };
		for (decltype(argc) i = 0; i < argc; ++i)
			result[i] = argv[i];
		return result;
	}

public:

	global_data(int argc, char** argv) : application_start_arguments(decode_command_line_input(argc, argv)) {
		standard_logger().debug("Initializing global data...");
		try {
			executable_path = application_start_arguments[0];
			working_directory = std::filesystem::current_path();
		}
		catch (...) {
			throw internal_error("Error when initializing working directories.");
		}
		standard_logger().debug(std::string("path to executable: ") + executable_path.generic_string());
		standard_logger().debug(std::string("working directory: ") + working_directory.generic_string());

		unsigned int skip_arg_counter{ 1 };

		if ((application_start_arguments.size() > 1) && (application_start_arguments[1] == "-C")) {
			// use a config file located at a custom directory with a custom name.
			std::filesystem::path given_path = application_start_arguments[2];
			_config_file_path = given_path.has_root_path() ?
				given_path :
				working_directory / given_path;
			try {
				_config_file_path = std::filesystem::weakly_canonical(_config_file_path);
			}
			catch (const std::filesystem::filesystem_error& e) {
				standard_logger().error(std::string("Error on processing config file path: ") + e.what());
				throw bad_argument("Given config file path might be ill-formed.");
			}
			skip_arg_counter += 2;
			standard_logger().info(std::string("Set a custom config file path: ") + _config_file_path.generic_string());
		}
		else {
			_config_file_path = working_directory / const_strings::CONFIG_FILE_STANDARD_NAME;
			standard_logger().debug(std::string("Implicitly use standard config file path: ") + _config_file_path.generic_string());
		}

		internal_error::assert_true(application_start_arguments.cend() - application_start_arguments.cbegin() <= skip_arg_counter,
			"Skipping arguments on global data initialisation failed.");
		for (auto it = application_start_arguments.cbegin() + skip_arg_counter; it != application_start_arguments.cend(); ++it) {
			_action_command_sequence.push_back(*it);
		}
	}

};
