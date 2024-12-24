#include <MIDI_Constants/MCUNameFromNoteNumber.hpp>
#include <gtest/gtest.h>

USING_CS_NAMESPACE;

TEST(MCUNameFromNoteNumber, getMCUNameFromNoteNumber) {
    EXPECT_STREQ(
        reinterpret_cast<const char *>(MCU::getMCUNameFromNoteNumber(0x00)),
        "REC_RDY_1");
    EXPECT_STREQ(
        reinterpret_cast<const char *>(MCU::getMCUNameFromNoteNumber(0x76)),
        "RELAY");
    EXPECT_STREQ(
        reinterpret_cast<const char *>(MCU::getMCUNameFromNoteNumber(0x77)),
        "<out-of-bounds>");
}