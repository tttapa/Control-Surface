#pragma once

#include <MIDI_Interfaces/MIDI_Interface.hpp>
#include <gmock-wrapper.h>

class EmptyParser : public CS::MIDI_Parser {
  public:
    static EmptyParser &getInstance() {
        static EmptyParser instance;
        return instance;
    }
    CS::SysExMessage getSysExMessage() const override {
        return {nullptr, 0, 0};
    }
};

class MockMIDI_Interface : public CS::Parsing_MIDI_Interface {
  public:
    MockMIDI_Interface()
        : CS::Parsing_MIDI_Interface(EmptyParser::getInstance()) {}
    MOCK_METHOD(CS::MIDIReadEvent, read, (), (override));
    MOCK_METHOD(void, sendImpl, (uint8_t, uint8_t, uint8_t, uint8_t),
                (override));
    MOCK_METHOD(void, sendImpl, (uint8_t, uint8_t, uint8_t), (override));
    MOCK_METHOD(void, sendImpl, (const uint8_t *, size_t, uint8_t), (override));
    MOCK_METHOD(void, sendImpl, (uint8_t, uint8_t), (override));
};
