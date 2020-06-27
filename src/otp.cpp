
#include "nlohmann/json.hpp"

#include <experimental/filesystem>
#include <iostream>

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


bool exists_config_file(int argc, char** argv) {
	std::experimental::filesystem::path p;
	return false;
}

int main(int argc, char** argv)
{
	

	std::cout << "Finished.\n";
}
