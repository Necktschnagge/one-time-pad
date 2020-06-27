cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME One-Time-Pad)

project($(PROJECT_NAME) VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

enable_testing()
add_subdirectory(extern/googletest)

add_executable($(PROJECT_NAME)
	src/otp.cpp
	)

include_directories(SYSTEM extern/json/include)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT $(PROJECT_NAME))#Set Visualo Studio start-up project, so that one can directly run the debugger.
