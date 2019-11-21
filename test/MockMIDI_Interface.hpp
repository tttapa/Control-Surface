#pragma once

#include <MIDI_Interfaces/MIDI_Interface.hpp>
#include <gmock-wrapper.h>

class EmptyParser : public CS::MIDI_Parser {
  public:
    static EmptyParser &getInstance() {
        static EmptyParser instance;
        return instance;
    }
    CS::SysExMessage getSysEx() const override { return {nullptr, 0, 0}; }
};

W_SUGGEST_OVERRIDE_OFF

class MockMIDI_Interface : public CS::Parsing_MIDI_Interface {
  public:
    MockMIDI_Interface()
        : CS::Parsing_MIDI_Interface(EmptyParser::getInstance()) {}
    MOCK_METHOD0(read, CS::MIDI_read_t(void));
    MOCK_METHOD5(sendImpl, void(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD4(sendImpl, void(uint8_t, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD3(sendImpl, void(const uint8_t *, size_t, uint8_t));
    MOCK_METHOD2(sendImpl, void(uint8_t, uint8_t));
};

W_SUGGEST_OVERRIDE_ON