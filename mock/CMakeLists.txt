include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

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
