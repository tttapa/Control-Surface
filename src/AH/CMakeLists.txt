if (FAST_COMPILE)
    set(Arduino_Helpers_SOURCES
        PrintStream/PrintStream.cpp
        Debug/Debug.cpp
        Hardware/IncrementDecrementButtons.cpp
        Hardware/Button.cpp
        Hardware/IncrementButton.cpp
        Hardware/ExtendedInputOutput/ShiftRegisterOutRGB.cpp
        Hardware/ExtendedInputOutput/ExtendedIOElement.cpp
        Hardware/ExtendedInputOutput/ExtendedInputOutput.cpp
        Error/Exit.cpp
        Math/Vector.cpp
        Math/Quaternion.cpp
    )
else ()
    file(GLOB_RECURSE
        Arduino_Helpers_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
endif ()

add_library(Arduino_Helpers ${Arduino_Helpers_SOURCES})
target_include_directories(Arduino_Helpers 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

if (FAST_COMPILE)
    target_compile_definitions(Arduino_Helpers 
        PUBLIC 
            -DNO_DEBUG_PRINTS 
            -DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)
else ()
        target_compile_definitions(Arduino_Helpers 
        PUBLIC 
            #-DNO_DEBUG_PRINTS 
            -DTEST_COMPILE_ALL_HEADERS_SEPARATELY
            -DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)
endif ()

target_link_libraries(Arduino_Helpers PUBLIC ArduinoMock)