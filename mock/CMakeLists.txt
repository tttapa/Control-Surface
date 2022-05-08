find_package(GTest REQUIRED CONFIG)

add_library(ArduinoMock
    "Core/Arduino.cpp"
    "Core/ArduinoMock.cpp"
    "Core/HardwareSerial0.cpp"
    "Core/Print.cpp"
)
target_include_directories(ArduinoMock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Core-Libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Adafruit_GFX
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Adafruit_SSD1306
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Encoder)
target_link_libraries(ArduinoMock
    PUBLIC GTest::gtest GTest::gmock
    PRIVATE Arduino-Helpers::warnings)
