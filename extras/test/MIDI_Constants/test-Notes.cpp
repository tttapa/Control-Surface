#include <MIDI_Constants/Notes.hpp>
#include <gtest/gtest.h>

TEST(Notes, noteNumbers) {
    // See https://en.wikipedia.org/wiki/Scientific_pitch_notation
    // and page 48 of MIDI 1.0 Detailed Specification 4.2
    EXPECT_EQ(cs::MIDI_Notes::C[-1], 0x00);
    EXPECT_EQ(cs::MIDI_Notes::C[0], 0x0C);
    EXPECT_EQ(cs::MIDI_Notes::C[4], 0x3C);
    EXPECT_EQ(cs::MIDI_Notes::A[4], 0x45); // 440 Hz
}