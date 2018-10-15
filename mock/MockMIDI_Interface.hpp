#pragma once

#include <MIDI_Interfaces/MIDI_Interface.hpp>
#include <gmock/gmock.h>

class EmptyParser : public MIDI_Parser {
  public:
    static EmptyParser &getInstance() {
        static EmptyParser instance;
        return instance;
    }
};

class MockMIDI_Interface : public MIDI_Interface {
  public:
    MockMIDI_Interface() : MIDI_Interface(EmptyParser::getInstance()) {}
    MOCK_METHOD0(read, MIDI_read_t(void));
    MOCK_METHOD4(sendImpl, void(uint8_t, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD3(sendImpl, void(uint8_t, uint8_t, uint8_t));
};