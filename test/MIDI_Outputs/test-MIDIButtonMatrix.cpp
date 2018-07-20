#include <MIDI_Outputs/NoteButtonMatrix.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;

TEST(NoteButtonMatrix, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    constexpr uint8_t channel = 1;
    AddressMatrix<3, 2> addresses = {{
        {1, 3},
        {5, 7},
        {9, 11}
    }};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(5, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(6, INPUT_PULLUP));

    NoteButtonMatrix<3, 2> matrix = {{2, 3, 4}, {5, 6}, addresses, channel};

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(4, INPUT));

    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(5))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(6))
        .Times(3)
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));

    EXPECT_CALL(midi, sendImpl(NOTE_ON, 0, 5, 0x7F));

    matrix.update();

    Mock::VerifyAndClear(&ArduinoMock::getInstance());
}