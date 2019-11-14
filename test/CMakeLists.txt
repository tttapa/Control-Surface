# Test executable compilation and linking
file(GLOB_RECURSE TESTS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(tests ${TESTS_SOURCES})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests
                      Arduino_Helpers
                      Control_Surface
                      googletest_wrappers)

# Add tests
gtest_discover_tests(tests)