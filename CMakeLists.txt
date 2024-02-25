cmake_minimum_required(VERSION 3.18)
project(Control-Surface CXX)
set(CMAKE_CXX_STANDARD 17)
enable_testing()

# Add coverage target
option(AH_WITH_COVERAGE
    "Generate coverage information." Off)
if (AH_WITH_COVERAGE)
    add_custom_target(coverage
        ${CMAKE_CURRENT_LIST_DIR}/scripts/coverage.sh
        ${CMAKE_CXX_COMPILER_ID}
        ${CMAKE_CXX_COMPILER_VERSION}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fno-inline")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fno-inline")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    add_dependencies(coverage Arduino-Helpers::tests)
endif()

# Add documentation target
find_package(Doxygen)
find_package(Python3 COMPONENTS Interpreter)
if (DOXYGEN_FOUND AND Python3_Interpreter_FOUND)
    # Add XBM target
    add_custom_target(xbm-doxygen
        Python3::Interpreter ${CMAKE_CURRENT_LIST_DIR}/src/Display/Bitmaps/Scripts/XBM-export.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/docs/Doxygen)
    # Add example documentation target
    add_custom_target(examples-doxygen 
        Python3::Interpreter ${CMAKE_CURRENT_LIST_DIR}/doxygen/scripts/examples.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/doxygen)
    # Add documentation target
    add_custom_target(documentation
        Doxygen::doxygen
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/doxygen
        DEPENDS xbm-doxygen examples-doxygen)
endif()

# Compiler warnings
option(AH_WARNINGS_AS_ERRORS "Enable -Werror" On)
include(cmake/Warnings.cmake)

# Build the source files and tests
add_subdirectory(mock)
add_subdirectory(src)
add_subdirectory(test)
