# Test executable compilation and linking
file(GLOB_RECURSE TESTS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(tests ${TESTS_SOURCES})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE Control_Surface)

# Add tests
gtest_add_tests(TARGET tests TEST_LIST test_list)
set_tests_properties(${test_list} PROPERTIES TIMEOUT 20)