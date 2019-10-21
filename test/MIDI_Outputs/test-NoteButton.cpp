#include <MIDI_Outputs/Bankable/NoteButton.hpp>
#include <MIDI_Outputs/Bankable/NoteButtons.hpp>
#include <MIDI_Outputs/NoteButton.hpp>
#include <MIDI_Outputs/NoteButtons.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock-wrapper.h>

using namespace ::testing;
using namespace CS;

TEST(NoteButton, pressAndRelease) {
    MockMIDI_Interface midi;

    NoteButton button(2, {0x3C, CHANNEL_7, 0xC});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F, 0xC));
    button.update();

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F, 0xC));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

TEST(NoteButtonBankable, pressAndRelease) {
    MockMIDI_Interface midi;

    OutputBank bank(4);

    Bankable::NoteButton button(bank, 2, {0x3C, CHANNEL_7, 0xC});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F, 0xC));
    button.update();

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F, 0xC));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(NoteButtonBankable, changeSettingAndPressAndRelease) {
    MockMIDI_Interface midi;

    OutputBank bank(4);
    // Change bank setting
    bank.select(1);

    Bankable::NoteButton button(bank, 2, {0x3C, CHANNEL_7, 0xC});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C + 4, 0x7F, 0xC));
    button.update();

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C + 4, 0x7F, 0xC));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(NoteButtonBankable, pressAndChangeSettingAndRelease) {
    MockMIDI_Interface midi;

    OutputBank bank(4);

    Bankable::NoteButton button(bank, 2, {0x3C, CHANNEL_7, 0xC});
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F, 0xC));
    button.update();

    // Change bank setting
    bank.select(1);

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F, 0xC));
    button.update();

    // Pressing again (with new bank setting)
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(5000));
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C + 4, 0x7F, 0xC));
    button.update();

    // Releasing again (with new bank setting)
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(6000));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C + 4, 0x7F, 0xC));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}