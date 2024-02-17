option(AH_FAST_COMPILE "Compile only necessary files" On)

file(GLOB_RECURSE
    Arduino_Helpers_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(Arduino_Helpers ${Arduino_Helpers_SOURCES})
target_include_directories(Arduino_Helpers 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_compile_definitions(Arduino_Helpers PUBLIC 
    NO_DEBUG_PRINTS 
    ANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)

target_link_libraries(Arduino_Helpers
    PUBLIC ArduinoMock
    PRIVATE Arduino-Helpers::warnings)
