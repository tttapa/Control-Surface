cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# <copy>
# Download and unpack googletest at configure time
configure_file(cmake/CMakeLists.txt.in googletest/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest/googletest-build
                 EXCLUDE_FROM_ALL)
# </copy>

set (CMAKE_C_COMPILER "gcc-8")
set (CMAKE_CXX_COMPILER "g++-8")

# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # LTO

include(GoogleTest)
enable_testing()

add_definitions("-g3 -O1 -Wall -Wextra -fdiagnostics-show-option -Wsuggest-override")

#include Google Test
include_directories(${CMAKE_BINARY_DIR}/googletest/googletest-src/googletest/include)

#include Google Mock
include_directories(${CMAKE_BINARY_DIR}/googletest/googletest-src/googlemock/include)

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
add_test(NAME Tests COMMAND tests)

# Add documentation target
add_custom_target(documentation rm -r html && ./Scripts/examples.py && doxygen
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../doc/Doxygen)

# Add export documentation target
add_custom_target(export_documentation ./Export.sh
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../doc/Doxygen/Scripts)

# Add XBM target

add_custom_target(xbm python3 XBM-export.py
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../src/Display/Bitmaps/Scripts/)

# Custom test targets

add_custom_target(check tests
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
                  DEPENDS tests)

add_custom_target(arduino ./build-arduino.sh
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/../scripts)