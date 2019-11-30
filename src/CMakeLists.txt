add_subdirectory(AH)

if (FAST_COMPILE)
    set(CONTROL_SURFACE_SOURCES
        Def/MIDICNChannelAddress.cpp
        MIDI_Inputs/MIDIInputElementCC.cpp
        MIDI_Inputs/MIDIInputElementNote.cpp
        MIDI_Inputs/MIDIInputElementChannelPressure.cpp
        MIDI_Inputs/MIDIInputElementSysEx.cpp
        MIDI_Inputs/MIDIInputElementPC.cpp
        MIDI_Inputs/MCU/LCD.cpp
        MIDI_Constants/MCUNameFromNoteNumber.cpp
        Display/DisplayInterface.cpp
        Display/DisplayElement.cpp
        Display/MCU/VPotDisplay.cpp
        Control_Surface/Control_Surface_Class.cpp
        MIDI_Senders/RelativeCCSender.cpp
        Banks/BankAddresses.cpp
        MIDI_Parsers/USBMIDI_Parser.cpp
        MIDI_Parsers/MIDI_Parser.cpp
        MIDI_Parsers/SerialMIDI_Parser.cpp
        MIDI_Parsers/SysExBuffer.cpp
        MIDI_Interfaces/MIDI_Interface.cpp
        MIDI_Interfaces/DebugMIDI_Interface.cpp)
else ()
    file(GLOB_RECURSE
        CONTROL_SURFACE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    list(FILTER CONTROL_SURFACE_SOURCES EXCLUDE REGEX "AH/.*")
endif ()

add_library(Control_Surface ${CONTROL_SURFACE_SOURCES})
target_include_directories(Control_Surface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (FAST_COMPILE)
target_compile_definitions(Control_Surface 
    PUBLIC 
        -DNO_DEBUG_PRINTS 
        # -DTEST_COMPILE_ALL_HEADERS_SEPARATELY
        -DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)
else ()
        target_compile_definitions(Control_Surface 
        PUBLIC 
            -DNO_DEBUG_PRINTS 
            -DTEST_COMPILE_ALL_HEADERS_SEPARATELY
            -DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)
endif ()

target_link_libraries(Control_Surface PUBLIC ArduinoMock)
target_link_libraries(Control_Surface PUBLIC Arduino_Helpers)
