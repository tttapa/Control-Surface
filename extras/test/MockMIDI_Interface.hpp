#pragma once

#include <MIDI_Interfaces/MIDI_Interface.hpp>
#include <gmock/gmock.h>

class EmptyParser : public cs::MIDI_Parser {
  public:
    static EmptyParser &getInstance() {
        static EmptyParser instance;
        return instance;
    }
    cs::SysExMessage getSysExMessage() const { return {nullptr, 0}; }
};

class MockMIDI_Interface : public cs::MIDI_Interface {
    using Cable = cs::Cable;

  public:
    MOCK_METHOD(cs::MIDIReadEvent, read, ());
    MOCK_METHOD(void, sendChannelMessageImpl, (cs::ChannelMessage), (override));
    MOCK_METHOD(void, sendSysCommonImpl, (cs::SysCommonMessage), (override));
    MOCK_METHOD(void, sendSysExImpl, (cs::SysExMessage), (override));
    MOCK_METHOD(void, sendRealTimeImpl, (cs::RealTimeMessage), (override));
    MOCK_METHOD(void, handleStall, (), (override));
    void update() override {}
    void sendNowImpl() override {}
};
