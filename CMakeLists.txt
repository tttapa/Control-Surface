cmake_minimum_required(VERSION 3.10)

project(Control_Surface)

################################################################################
# Add Google Test
################################################################################

add_subdirectory(googletest)
include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR}/include)

include(GoogleTest)
enable_testing()

################################################################################
# Set the C++ standard to C++14 and use GCC 9 if available
################################################################################

set(CMAKE_CXX_STANDARD 14)
find_program(GCC_9_CMAKE_C_COMPILER "gcc-9")
find_program(GCC_9_CMAKE_CXX_COMPILER "g++-9")

if(NOT GCC_9_CMAKE_C_COMPILER OR NOT GCC_9_CMAKE_CXX_COMPILER)
message(WARNING "GCC 9 not found")
else()
message(STATUS "GCC 9 found")
set(CMAKE_C_COMPILER ${GCC_9_CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${GCC_9_CMAKE_CXX_COMPILER})
endif()

################################################################################
# Change the default output directories
################################################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

################################################################################
# Set the compiler options
################################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-g3 -O0 -Wall -Wextra -Werror \
-fdiagnostics-show-option -Wsuggest-override \
-Wdouble-promotion \
-Wswitch-default -Wswitch-enum -Wimplicit-fallthrough=3 \
-Wstrict-overflow=5 \
-Wuninitialized \
-pedantic -pedantic-errors \
-Wmissing-include-dirs")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
-g3 -O0 -Wall -Wextra -Werror")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # LTO

################################################################################
# Enable coverage information
################################################################################

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
# set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

################################################################################
# Build the source files 
################################################################################

# Arduino Core files
include_directories(ArduinoMock)
file(GLOB_RECURSE ARDUINO_SOURCES ArduinoMock/*.cpp)
add_library(ArduinoMock STATIC ${ARDUINO_SOURCES})

# Mock
include_directories(mock)

# Test
include_directories(test)

# add_definitions(-DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)
add_definitions(-DNO_DEBUG_PRINTS -DTEST_COMPILE_ALL_HEADERS_SEPARATELY)

# Control Surface library files
include_directories(src)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(Control_Surface STATIC ${SOURCES})

# Test executable compilation and linking
file(GLOB_RECURSE TESTS test/*.cpp)
add_executable(tests ${TESTS})
target_link_libraries(tests
                      gmock
                      Control_Surface
                      ArduinoMock)

# Add tests
gtest_discover_tests(tests)

################################################################################
# Custom targets for documentation 
################################################################################

# Add documentation target
add_custom_target(documentation rm -r html && ./Scripts/examples.py && doxygen
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../doc/Doxygen)

# Add export documentation target
add_custom_target(export_documentation ./Export.sh
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../doc/Doxygen/Scripts)

# Add coverage target
add_custom_target(coverage ./coverage.sh
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../scripts)

# Add XBM target
add_custom_target(xbm python3 XBM-export.py
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../src/Display/Bitmaps/Scripts/)

# Custom test targets
add_custom_target(check tests
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
                  DEPENDS tests)

add_custom_target(arduino ./build-arduino.sh
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../scripts)