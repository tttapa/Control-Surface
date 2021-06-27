add_subdirectory(AH)
option(FAST_COMPILE "Compile only the necessary files without compiling the \
                     header-only components in isolation to check the include \
                     hierarchy" OFF)
if (FAST_COMPILE)
    set(CONTROL_SURFACE_SOURCES
        Def/MIDIAddress.cpp
        MIDI_Inputs/MCU/LCD.cpp
        MIDI_Constants/MCUNameFromNoteNumber.cpp
        Display/DisplayInterface.cpp
        Display/DisplayElement.cpp
        Display/MCU/VPotDisplay.cpp
        Control_Surface/Control_Surface_Class.cpp
        MIDI_Senders/RelativeCCSender.cpp
        Banks/BankAddresses.cpp
        MIDI_Parsers/MIDI_MessageTypes.cpp
        MIDI_Parsers/USBMIDI_Parser.cpp
        MIDI_Parsers/SerialMIDI_Parser.cpp
        MIDI_Parsers/SysExBuffer.cpp
        MIDI_Interfaces/MIDI_Pipes.cpp
        MIDI_Interfaces/MIDI_Interface.cpp
        MIDI_Interfaces/SerialMIDI_Interface.cpp
        MIDI_Interfaces/DebugMIDI_Interface.cpp
        MIDI_Interfaces/BluetoothMIDI_Interface.cpp
        MIDI_Interfaces/BLEMIDI/BLEMIDIPacketBuilder.cpp
        MIDI_Interfaces/BLEMIDI/MIDIMessageQueue.cpp)
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
            -DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)
else ()
        target_compile_definitions(Control_Surface 
        PUBLIC 
            -DNO_DEBUG_PRINTS 
            -DTEST_COMPILE_ALL_HEADERS_SEPARATELY
            -DANALOG_FILTER_SHIFT_FACTOR_OVERRIDE=2)
endif ()

target_link_libraries(Control_Surface PUBLIC Arduino_Helpers)
