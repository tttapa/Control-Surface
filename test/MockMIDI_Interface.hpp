#pragma once

#include <MIDI_Interfaces/MIDI_Interface.hpp>
#include <gmock/gmock.h>

class EmptyParser : public CS::MIDI_Parser {
  public:
    static EmptyParser &getInstance() {
        static EmptyParser instance;
        return instance;
    }
    CS::SysExMessage getSysExMessage() const { return {nullptr, 0}; }
};

class MockMIDI_Interface : public CS::MIDI_Interface {
    using Cable = CS::Cable;

  public:
    MOCK_METHOD(CS::MIDIReadEvent, read, ());
    MOCK_METHOD(void, sendChannelMessageImpl, (CS::ChannelMessage), (override));
    MOCK_METHOD(void, sendSysCommonImpl, (CS::SysCommonMessage), (override));
    MOCK_METHOD(void, sendSysExImpl, (CS::SysExMessage), (override));
    MOCK_METHOD(void, sendRealTimeImpl, (CS::RealTimeMessage), (override));
    MOCK_METHOD(void, handleStall, (), (override));
    void update() override {}
    void sendNowImpl() override {}
};
