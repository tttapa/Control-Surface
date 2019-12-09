cmake_minimum_required(VERSION 3.10)

################################################################################
# Enable ccache if available
################################################################################

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

################################################################################
# Set the C++ standard to C++14
################################################################################

set(CMAKE_CXX_STANDARD 14)
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
# Change the default output directories
################################################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

################################################################################
# Set the compiler options
################################################################################

set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} \
    -Wmissing-include-dirs")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -Wall -Wextra -Werror \
    -fdiagnostics-show-option \
    -Wdouble-promotion \
    -Wswitch-default -Wswitch-enum -Wimplicit-fallthrough \
    -Wuninitialized \
    -Wno-double-promotion \
    -Wno-missing-braces \
    -pedantic -pedantic-errors")
    
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -Wall -Wextra -Werror")

set(CMAKE_CXX_FLAGS_DEBUG 
    "-ggdb3 -O0 -fno-inline-functions -fsanitize=address")
set(CMAKE_C_FLAGS_DEBUG
    "-ggdb3 -O0 -fno-inline-functions -fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_RELEASE "-Os")

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -g3 -ggdb")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -g3 -ggdb")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Winconsistent-missing-override")
    
else()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Wsuggest-override")

endif()

# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # LTO

################################################################################
# Enable coverage information
################################################################################

set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG} \
                            -fprofile-arcs -ftest-coverage \
                            -fsanitize=address")
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} \
                              -fprofile-arcs -ftest-coverage \
                              -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

################################################################################
# Build the source files and tests
################################################################################

add_subdirectory(gtest-wrappers)
add_subdirectory(mock)
add_subdirectory(src)
add_subdirectory(test)

################################################################################
# Custom targets for documentation 
################################################################################

add_custom_target(export_documentation ./export.sh
WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/doxygen/scripts)

# Add XBM target
add_custom_target(xbm 
    python3 ${CMAKE_CURRENT_LIST_DIR}/src/Display/Bitmaps/Scripts/XBM-export.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/doxygen/images/xbm)

# Add export documentation target
add_custom_target(documentation 
    rm -rf html && ./scripts/examples.py && doxygen
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/doxygen)

# Add coverage target
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
add_custom_target(coverage 
    ${CMAKE_CURRENT_LIST_DIR}/scripts/coverage.sh ${CMAKE_CXX_COMPILER_ID}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# Custom test targets
add_custom_target(check tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
    DEPENDS tests)

add_custom_target(arduino ./build-arduino-examples.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/scripts)

add_custom_target(arduino_ci ./build-arduino-examples-ci.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/scripts)
