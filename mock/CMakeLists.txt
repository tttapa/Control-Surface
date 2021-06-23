file(GLOB_RECURSE
     ARDUINO_MOCK_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(ArduinoMock ${ARDUINO_MOCK_SOURCES})
target_include_directories(ArduinoMock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Core-Libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Adafruit_GFX
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Adafruit_SSD1306
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Encoder)
target_link_libraries(ArduinoMock PUBLIC GTest::gtest GTest::gmock)
