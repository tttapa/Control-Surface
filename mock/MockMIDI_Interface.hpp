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

#if __GNUC__ >= 5
// Disable GCC 5's -Wsuggest-override warnings in mock methods
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#endif

class MockMIDI_Interface : public CS::Parsing_MIDI_Interface {
  public:
    MockMIDI_Interface()
        : CS::Parsing_MIDI_Interface(EmptyParser::getInstance()) {}
    MOCK_METHOD0(read, CS::MIDI_read_t(void));
    MOCK_METHOD5(sendImpl, void(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD4(sendImpl, void(uint8_t, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD3(sendImpl, void(const uint8_t *, size_t, uint8_t));
};

#if __GNUC__ >= 5
#pragma GCC diagnostic pop
#endif