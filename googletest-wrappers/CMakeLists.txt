add_library(googletest_wrappers INTERFACE)
target_include_directories(googletest_wrappers 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)