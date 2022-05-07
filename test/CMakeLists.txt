include(GoogleTest)

# Test executable compilation and linking
add_executable(tests
    "test_example.cpp"
    "test-main.cpp"
    "AH/PrintStream/test-PrintStream.cpp"
    "AH/Timing/test-Timer.cpp"
    "AH/Hardware/test-FilteredAnalog.cpp"
    "AH/Hardware/ExtendedInputOutput/test-AnalogMultiplex.cpp"
    "AH/Hardware/ExtendedInputOutput/test-ExtendedInputOutput.cpp"
    "AH/Hardware/test-IncrementDecrementButtons.cpp"
    "AH/Hardware/test-IncrementButton.cpp"
    "AH/Hardware/test-Button.cpp"
    "AH/Containers/test-Updatable.cpp"
    "AH/Containers/test-DoublyLinkedList.cpp"
    "AH/Containers/test-Array.cpp"
    "AH/Containers/tests-BitArray.cpp"
    "AH/Math/test-Degrees.cpp"
    "AH/Math/test-Quaternion.cpp"
    "AH/Math/test-IncreaseBitDepth.cpp"
    "AH/Math/test-Vector.cpp"
    "AH/Filters/test-Hysteresis.cpp"
    "AH/Filters/test-EMA.cpp"
)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests
    PRIVATE Arduino_Helpers
    PRIVATE Arduino-Helpers::warnings)

# Add tests
gtest_discover_tests(tests DISCOVERY_TIMEOUT 60 TIMEOUT 20)
add_executable(Arduino-Helpers::tests ALIAS tests)