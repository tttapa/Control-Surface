#pragma once

#include "MCU_7SegmentDisplay.hpp"

constexpr static uint8_t TimeDisplayLength = 10;
constexpr static uint8_t TimeDisplayAddress = 0x40;

class MCU_TimeDisplay : public MCU_7SegmentDisplay<TimeDisplayLength> {
  public:
    MCU_TimeDisplay(uint8_t channel = 1)
        : MCU_7SegmentDisplay<TimeDisplayLength>(channel, TimeDisplayAddress) {}
    void
    print() { // TODO: add support for 5-digit bar counts, maybe using decimalPt?
        char barStr[5], beatStr[3], frameStr[4];
        getBars(barStr);
        getBeats(beatStr);
        getFrames(frameStr);
        DEBUG("Bar: " << barStr << "\tBeat: " << beatStr
                      << "\tFrame: " << frameStr);
    }
    void getBars(char *buff) {
        if (getCharacterAt(5) == ' ') {
            getText(buff, 0, 3);
        } else {
            getText(buff, 0, 4);
        }
    }
    void getBeats(char *buff) {
        if (getCharacterAt(5) == ' ') {
            getText(buff, 3, 2);
        } else {
            getText(buff, 4, 2);
        }
    }
    void getFrames(char *buff) {
        if (getCharacterAt(5) == ' ') {
            getText(buff, 7, 3);
        } else {
            getText(buff, 8, 2);
        }
    }
};