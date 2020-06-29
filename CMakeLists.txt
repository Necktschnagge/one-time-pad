cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME One-Time-Pad)
project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

add_subdirectory(src)

add_subdirectory(extern/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(SYSTEM extern/json/include)

#add_executable(${PROJECT_NAME}
#	src/otp.cpp
#	)


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})#Set Visualo Studio start-up project, so that one can directly run the debugger.

################################
# Unit Tests
################################
# Add test cpp file
#add_executable( runUnitTests test/example_test.cpp )
# Link test executable against gtest & gtest_main
#target_link_libraries(runUnitTests gtest gtest_main)
#add_test( runUnitTests runUnitTests )