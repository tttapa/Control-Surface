#include <MIDI_Outputs/Bankable/NoteButtonLatching.hpp>
#include <MIDI_Outputs/NoteButtonLatching.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;

TEST(NoteButtonLatching, pressAndRelease) {
    MockMIDI_Interface midi;

    NoteButtonLatching button(2, 0x3C, 7);
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
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F));
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
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

// -------------------------------------------------------------------------- //

TEST(NoteButtonLatchingBankable, pressAndRelease) {
    MockMIDI_Interface midi;

    Bank bank(4);

    Bankable::NoteButtonLatching button(bank, 2, 0x3C, 7);
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
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F));
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
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(NoteButtonLatchingBankable, changeSettingAndPressAndRelease) {
    MockMIDI_Interface midi;

    Bank bank(4);

    Bankable::NoteButtonLatching button(bank, 2, 0x3C, 7);
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    button.begin();

    // Change bank setting
    bank.select(1);

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C + 4, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C + 4, 0x7F));
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
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C + 4, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C + 4, 0x7F));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(NoteButtonLatchingBankable, pressAndChangeSettingAndRelease) {
    MockMIDI_Interface midi;

    Bank bank(4);

    Bankable::NoteButtonLatching button(bank, 2, 0x3C, 7);
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
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C, 0x7F));
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
    EXPECT_CALL(midi, sendImpl(NOTE_ON, 6, 0x3C + 4, 0x7F));
    EXPECT_CALL(midi, sendImpl(NOTE_OFF, 6, 0x3C + 4, 0x7F));
    button.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}