cmake_minimum_required(VERSION 3.10)
project(One-Time-Pad VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

add_executable(One-Time-Pad
	otp.cpp
	)

include_directories(SYSTEM extern/json/include)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT One-Time-Pad)#Set Visualo Studio start-up project, so that one can directly run the debugger.
