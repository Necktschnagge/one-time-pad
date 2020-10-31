cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME One-Time-Pad)
project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

add_subdirectory(extern/googletest)

include_directories(SYSTEM extern/spdlog/include)
include_directories(SYSTEM extern/json/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})#Set Visualo Studio start-up project, so that one can directly run the debugger.

#add_custom_target(
#  Test ALL
#)
#
#add_custom_command(
##  OUTPUT stamped_file.lic
#  TARGET Test
#  DEPENDS Test-One-Time-Pad
#  COMMAND /build/test/Debug/TEST-One-Time-Pad.exe
##  DEPENDS any/dependency.file
#  COMMENT "Run tests"
##  VERBATIM
#)

################################
# Unit Tests
################################
# Add test cpp file
#add_executable(runUnitTests test/example_test.cpp test/test_main.cpp)
# Link test executable against gtest & gtest_main
#target_link_libraries(runUnitTests gtest gtest_main)
#add_test( runUnitTests runUnitTests )