cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# <copy>
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest/CMakeLists.txt)
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

include(GoogleTest)
enable_testing()

#include Google Mock
include_directories(${CMAKE_BINARY_DIR}/googletest/googletest-src/googlemock/include)

# Arduino Core files
include_directories("mock")
file(GLOB_RECURSE ARDUINO_SOURCES "mock/*.cpp")
add_library(ArduinoMock STATIC ${ARDUINO_SOURCES})

# add_definitions(-DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)

# Control Surface library files
include_directories("src")
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Control_Surface STATIC ${SOURCES})

# Test executable compilation and linking
file(GLOB_RECURSE TESTS "test/*.cpp")
add_executable(tests ${TESTS})
target_link_libraries(tests 
                      gtest_main
                      gmock 
                      Control_Surface
                      ArduinoMock)

# Add tests
gtest_discover_tests(tests)
add_test(NAME Tests COMMAND tests)

# Add documentation target
add_custom_target(documentation doxygen 
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/doc/Doxygen")