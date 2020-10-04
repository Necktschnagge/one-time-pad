
//#include "nlohmann/json.hpp"

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
}
}


*/


/**
 otp init -> create fresh config.json
 otp status -> check status of config.json
 otp [encrypt|decrypt] source_file_path destination_file_path pad_name
 otp register pad_name keyfile_path -> add new keyfile to a
*/

inline std::filesystem::path config_file_path(const std::filesystem::path& working_directory) {
	return working_directory / "config.json";
}

inline bool exists_config_file(const std::filesystem::path& working_directory) {
	return std::filesystem::exists(config_file_path(working_directory));
}

struct global_data {
	std::vector<std::string> input;
	std::filesystem::path working_directory;
};


static const std::string EMPTY_CONFIG{ R"xxx({
   pads: []
   config: {
      auto_delete_used_keyfiles: false
      }
}
)xxx" };




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
		return data->input[1] == "init";
	}

	std::string description() const override {
		return "INIT CONFIG JSON";
	}

protected:
	void act() override {
		action_error::assert_true(data->input.size() == 2, "Too many arguments.");
		action_error::assert_true(!exists_config_file(data->working_directory), "File config.json already found.");
		auto config_json{ std::ofstream(config_file_path(data->working_directory)) };
		action_error::assert_true(config_json.good(), "Could not create config.json.");
		config_json << EMPTY_CONFIG;
		config_json.close();
		std::cout << "Successfully created a fresh config.json:\n\t" << data->working_directory << '\n';
	}

};

class otp_status : public action {
	bool match() const override {
		return data->input[1] == "status";
	}

protected:
	void act() override {
		action_error::assert_true(data->input.size() == 2, "Too many arguments.");
		if (!exists_config_file(data->working_directory)) {
			std::cout << "The current directory has not been set up for One-Time-Pad: File config.json is missing.\n You may create one by typing:\n   otp init\n";
			return;
		}
		auto config_json{ std::ifstream(config_file_path(data->working_directory)) };
		action_error::assert_true(config_json.good(), "Could not read config.json although there exists such a file.");

		config_json.close();
	}
};

std::vector<std::string> decode_input(int argc, char** argv) {
	auto result{ std::vector<std::string>(argc) };
	for (decltype(argc) i = 0; i < argc; ++i)
		(result)[i] = argv[i];
	return result;
}


int cli(int argc, char** argv) {
	try {
		std::cout << "This is Fussel One-Time-Pad 1.0";

		// init global data:
		global_data data;
		data.input = decode_input(argc, argv);
		data.working_directory = data.input[0];

		// give calss action access to global data:
		action::data = &data;

		// list all implemented actions:
		std::forward_list<std::unique_ptr<action>> all_actions;
		all_actions.emplace_front(new otp_init());

		// check for actions matching the input:
		std::list<action*> matching_actions;
		std::for_each(all_actions.cbegin(), all_actions.cend(), [&](const auto& p_action) { if (p_action->match()) matching_actions.emplace_back(p_action.get()); });

		// Check for missing or ambiguous actions:
		action_error::assert_true(matching_actions.size() > 0, "Invalid input. There is no otp action matching your input.");
		action_error::assert_true(matching_actions.size() < 2,
			std::string("Internal error. Please report the following to the developers:\nMore than one action matched for input sequence which is:\n  ")
			+ std::accumulate(data.input.cbegin(), data.input.cend(), std::string(), [](const auto& l, const auto& r) { return l + " " + r; })
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
		std::cerr << "\n" << e.what() << "\n\nAborted.";
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown exception caught. Please report to the developer.";
		return 2;
	}
	return 0;
}

int main(int argc, char** argv)
{
	return cli(argc, argv);
}
