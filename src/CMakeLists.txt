add_subdirectory(AH)

file(GLOB_RECURSE
    CONTROL_SURFACE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(FILTER CONTROL_SURFACE_SOURCES EXCLUDE REGEX "AH/.*")

add_library(Control_Surface ${CONTROL_SURFACE_SOURCES})
target_include_directories(Control_Surface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(Control_Surface PUBLIC 
    NO_DEBUG_PRINTS
    ANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)

target_link_libraries(Control_Surface PUBLIC Arduino_Helpers)
