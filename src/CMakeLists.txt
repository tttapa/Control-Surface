file(GLOB_RECURSE
     CONTROL_SURFACE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(Control_Surface ${CONTROL_SURFACE_SOURCES})
target_include_directories(Control_Surface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(Control_Surface 
    PUBLIC 
        -DNO_DEBUG_PRINTS 
        -DTEST_COMPILE_ALL_HEADERS_SEPARATELY
        -DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2
)
target_link_libraries(Control_Surface PUBLIC ArduinoMock)
