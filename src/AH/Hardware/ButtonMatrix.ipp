#include "ButtonMatrix.hpp"
#include <AH/Containers/CRTP.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <string.h>

BEGIN_AH_NAMESPACE

template <class Derived, uint8_t NumRows, uint8_t NumCols>
ButtonMatrix<Derived, NumRows, NumCols>::ButtonMatrix(
    const PinList<NumRows> &rowPins, const PinList<NumCols> &colPins)
    : rowPins(rowPins), colPins(colPins) {
    memset(prevStates, 0xFF, sizeof(prevStates));
}

template <class Derived, uint8_t NumRows, uint8_t NumCols>
void ButtonMatrix<Derived, NumRows, NumCols>::update() {
    unsigned long now = millis();
    // only update 25 ms after previous change (crude software debounce).
    // Edit this in Settings/Settings.hpp
    if (now - prevRefresh < debounceTime)
        return;

    for (size_t row = 0; row < NumRows; row++) { // scan through all rows
        pinMode(rowPins[row], OUTPUT);           // make the current row Lo-Z 0V
#if !defined(__AVR__) && defined(ARDUINO)
        delayMicroseconds(SELECT_LINE_DELAY);
#endif
        for (size_t col = 0; col < NumCols; col++) { // scan through all columns
            bool state = digitalRead(colPins[col]);  // read the state
            if (state != getPrevState(col, row)) {
                // if the state changed since last time
                // execute the handler
                CRTP(Derived).onButtonChanged(row, col, state);
                setPrevState(col, row, state); // remember the state
                prevRefresh = now;
            }
        }
        pinMode(rowPins[row], INPUT); // make the current row Hi-Z again
    }
}

template <class Derived, uint8_t NumRows, uint8_t NumCols>
void ButtonMatrix<Derived, NumRows, NumCols>::begin() {
    // make all columns input pins and enable
    // the internal pull-up resistors
    for (const pin_t &colPin : colPins)
        pinMode(colPin, INPUT_PULLUP);
    // make all rows Hi-Z
    for (const pin_t &rowPin : rowPins)
        pinMode(rowPin, INPUT);
}

template <class Derived, uint8_t NumRows, uint8_t NumCols>
inline uint8_t
ButtonMatrix<Derived, NumRows, NumCols>::positionToBits(uint8_t col,
                                                        uint8_t row) {
    // map from a 2D array of bits to a flat array of bits
    return col * NumRows + row;
}

template <class Derived, uint8_t NumRows, uint8_t NumCols>
inline uint8_t
ButtonMatrix<Derived, NumRows, NumCols>::bitsToIndex(uint8_t bits) {
    return bits >> 3; // bits / 8
}

template <class Derived, uint8_t NumRows, uint8_t NumCols>
inline uint8_t
ButtonMatrix<Derived, NumRows, NumCols>::bitsToBitmask(uint8_t bits) {
    return 1 << (bits & 7); // bits % 8
}

template <class Derived, uint8_t NumRows, uint8_t NumCols>
bool ButtonMatrix<Derived, NumRows, NumCols>::getPrevState(uint8_t col,
                                                           uint8_t row) {
    uint8_t bits = positionToBits(col, row);
    return !!(prevStates[bitsToIndex(bits)] & bitsToBitmask(bits));
}

template <class Derived, uint8_t NumRows, uint8_t NumCols>
void ButtonMatrix<Derived, NumRows, NumCols>::setPrevState(uint8_t col,
                                                           uint8_t row,
                                                           bool state) {
    uint8_t bits = positionToBits(col, row);
    if (state)
        prevStates[bitsToIndex(bits)] |= bitsToBitmask(bits);
    else
        prevStates[bitsToIndex(bits)] &= ~bitsToBitmask(bits);
}

END_AH_NAMESPACE