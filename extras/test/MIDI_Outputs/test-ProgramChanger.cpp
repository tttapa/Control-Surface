#include <Banks/Bank.hpp>
#include <MIDI_Outputs/ProgramChanger.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace cs;

TEST(ProgramChanger, select) {
    MockMIDI_Interface midi;
    Control_Surface.connectDefaultMIDI_Interface();

    ProgramChanger<5> pc {{1, 2, 3, 4, 5}, {Channel_3, Cable_13}};

    EXPECT_CALL(midi, sendChannelMessageImpl(
                          ChannelMessage(0xC2, 0x2, 0x00, Cable_13)));
    pc.select(1);
}
