
#include "nlohmann/json.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <functional>

static constexpr bool DEBUG_MODE = false;

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

bool exists_config_file(const std::filesystem::path& working_directory) {
	return std::filesystem::exists(working_directory / "config.json");
}

struct global_data {
	std::filesystem::path working_path;
};


static const std::string EMPTY_CONFIG{ R"xxx({
   pads: []
   config: {
      auto_delete_used_keyfiles: false
      }
})xxx" };

auto run_otp_init() -> std::string {
	auto x = true;
	return "";
};

using opt_type = std::optional<std::function<std::string(*)()>>;
opt_type parse_otp_init(const std::vector<std::string>& inputs){
	 /*if (inputs[1] == "init") {
		 return opt_type();
	 }
	 else {
		 return opt_type();
	 }*/
	return std::optional<std::function<std::string(*)()>>();
}

std::vector<std::string> decode_input(int argc, char** argv){
	std::vector<std::string> result(argc);
	for(decltype(argc) i = 0; i < argc; ++i)
		result[i] = argv[i];
	return result;
}

int main(int argc, char** argv)
{
	const auto decoded_input = decode_input(argc, argv);
	
	

	std::cout << "Finished.\n";
}
