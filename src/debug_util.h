#pragma once

#include <iostream>

namespace debug {
	[[deprecated]] void read() {
		char c;
		std::cin >> c;
	}

}