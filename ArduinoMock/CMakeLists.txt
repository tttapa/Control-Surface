file(GLOB_RECURSE
     ARDUINO_MOCK_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(ArduinoMock ${ARDUINO_MOCK_SOURCES})
target_include_directories(ArduinoMock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ArduinoMock PUBLIC googletest_wrappers)