#pragma once

#include "SevenSegmentDisplay.hpp"

BEGIN_CS_NAMESPACE

namespace MCU {

constexpr static uint8_t TimeDisplayLength = 10;
constexpr static uint8_t TimeDisplayAddress = 0x40;

/// Class that receives and stores the text of the Mackie Control Universal
/// 7-segment time display.
///
/// @ingroup MIDIInputElements
class TimeDisplay : public SevenSegmentDisplay<TimeDisplayLength> {
  public:
    TimeDisplay(Channel channel = Channel_1)
        : SevenSegmentDisplay<TimeDisplayLength>(
              {TimeDisplayAddress, channel}) {}
    // TODO: add support for 5-digit bar counts
    void print() const {
        char barStr[6], beatStr[3], frameStr[4];
        getBars(barStr);
        getBeats(beatStr);
        getFrames(frameStr);
        DEBUG(F("Bar: ") << barStr << F("\tBeat: ") << beatStr << F("\tFrame: ")
                         << frameStr);
    }
    void getBars(char *buff) const {
        if (getCharacterAt(5) == ' ') {
            getText(buff, 0, 3);
        } else if (getCharacterAt(6) == ' ') {
            getText(buff, 0, 4);
        } else {
            getText(buff, 0, 5);
        }
    }
    void getBeats(char *buff) const {
        if (getCharacterAt(5) == ' ') {
            getText(buff, 3, 2);
        } else if (getCharacterAt(6) == ' ') {
            getText(buff, 4, 2);
        } else {
            getText(buff, 5, 2);
        }
    }
    void getFrames(char *buff) const {
        if (getCharacterAt(5) == ' ') {
            getText(buff, 7, 3);
        } else if (getCharacterAt(6) == ' ') {
            getText(buff, 8, 2);
        } else {
            getText(buff, 9, 1);
        }
    }
};

} // namespace MCU

END_CS_NAMESPACE