add_library(googletest_wrappers INTERFACE)
target_include_directories(googletest_wrappers 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(googletest_wrappers INTERFACE gtest gmock)