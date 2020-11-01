#pragma once

#include <string_view>

struct const_strings {

	static constexpr std::string_view CONFIG_FILE_STANDARD_NAME{ "config.json" };

	static constexpr std::string_view EMPTY_CONFIG{ R"xxx({
   pads: []
   config: {
      auto_delete_used_keyfiles: false
   }
}
)xxx" };


};
