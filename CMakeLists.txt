cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 11)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

include_directories(${CMAKE_BINARY_DIR}/googletest-src/googlemock/include)

enable_testing()

# Arduino Core files
include_directories("mock")
file(GLOB_RECURSE ARDUINO_SOURCES "mock/*.cpp")
add_library(ArduinoMock STATIC ${ARDUINO_SOURCES})

add_definitions(-DGOOGLETEST)

# Control Surface library files
include_directories("src")
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Control_Surface STATIC ${SOURCES})

# Now simply link against gtest or gtest_main as needed.
file(GLOB_RECURSE TESTS "test/*.cpp")
add_executable(tests ${TESTS})
target_link_libraries(tests gtest_main Control_Surface ArduinoMock)
add_test(NAME tests_tests COMMAND tests)