#include <MIDI_Outputs/CCButton.hpp>
#include <MockMIDI_Interface.hpp>
#include <gmock/gmock.h>

using namespace ::testing;

TEST(CCButton, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    CCButton button(2, 0x3C, 7);

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x7F));
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
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x00));
    button.update();
}

TEST(CCButtonBankable, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    BankableNS::CCButton button(2, 0x3C, 7);

    Bank bank(4);
    bank.add(button);

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x7F));
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
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x00));
    button.update();
}

TEST(CCButtonBankable, changeSettingAndPressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    BankableNS::CCButton button(2, 0x3C, 7);

    Bank bank(4);
    // Change bank setting
    bank.setBankSetting(1);
    bank.add(button);

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C + 4, 0x7F));
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
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C + 4, 0x00));
    button.update();
}

TEST(CCButtonBankable, pressAndChangeSettingAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    BankableNS::CCButton button(2, 0x3C, 7);

    Bank bank(4);
    bank.add(button);

    // Still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(1000));
    button.update();

    // Pressing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(2000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x7F));
    button.update();

    // Change bank setting
    bank.setBankSetting(1);

    // Still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(3000));
    button.update();

    // Releasing
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillOnce(Return(4000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x00));
    button.update();
}

// -------------------------------------------------------------------------- //

TEST(CCButtons, pressAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    CCButtons<2> buttons = {{2, 3}, 0x3C, 7, 1, 2};

    // 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    buttons.update();

    // Pressing 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x7F));
    buttons.update();

    // 2 still pressed & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1, 0x7F));
    buttons.update();

    // 2 still pressed & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    buttons.update();

    // Releasing 2 & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x00));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1, 0x00));
    buttons.update();

    // 2 still released & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(7000));
    buttons.update();
}

// -------------------------------------------------------------------------- //

TEST(CCButtonsBankable, pressChangeBankAndRelease) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    BankableNS::CCButtons<2> buttons = {{2, 3}, 0x3C, 7, 1, 2};

    Bank bank(4);
    bank.add(buttons);

    // 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    buttons.update();

    // Pressing 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x7F));
    buttons.update();

    bank.setBankSetting(1);

    // 2 still pressed & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1, 0x7F));
    buttons.update();

    // 2 still pressed & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    buttons.update();

    // Releasing 2 & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x00));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1, 0x00));
    buttons.update();

    // 2 still released & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(7000));
    buttons.update();
}

TEST(CCButtonsBankable, pressChangeBankAndReleaseAndPress) {
    MockMIDI_Interface midi;
    Control_Surface.setMIDI_Interface(midi);

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, INPUT_PULLUP));
    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(3, INPUT_PULLUP));
    BankableNS::CCButtons<2> buttons = {{2, 3}, 0x3C, 7, 1, 2};

    Bank bank(4);
    bank.add(buttons);

    // 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(1000));
    buttons.update();

    // Pressing 2 & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(2000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x7F));
    buttons.update();

    bank.setBankSetting(1);

    // 2 still pressed & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(3000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1, 0x7F));
    buttons.update();

    // 2 still pressed & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(4000));
    buttons.update();

    // Releasing 2 & 3 still pressed
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(5000));
    EXPECT_CALL(midi, sendImpl(CC, 6, 0x3C, 0x00));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(6000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1, 0x00));
    buttons.update();

    // 2 still released & 3 still released
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(7000));
    buttons.update();

    // 2 still released & pressing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(LOW));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(8000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1 + 4, 0x7F));
    buttons.update();

    // 2 still released & releasing 3
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(2))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalRead(3))
        .WillOnce(Return(HIGH));
    EXPECT_CALL(ArduinoMock::getInstance(), millis())
        .Times(2)
        .WillRepeatedly(Return(9000));
    EXPECT_CALL(midi, sendImpl(CC, 6 + 2, 0x3C + 1 + 4, 0x00));
    buttons.update();
}