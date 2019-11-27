#include "ButtonMatrix.hpp"
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <string.h>

BEGIN_AH_NAMESPACE

using namespace ExtIO;

template <uint8_t nb_rows, uint8_t nb_cols>
ButtonMatrix<nb_rows, nb_cols>::ButtonMatrix(const PinList<nb_rows> &rowPins,
                                             const PinList<nb_cols> &colPins)
    : rowPins(rowPins), colPins(colPins) {
    memset(prevStates, 0xFF, sizeof(prevStates));
}

template <uint8_t nb_rows, uint8_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::update() {
    unsigned long now = millis();
    // only update 25 ms after previous change (crude software debounce).
    // Edit this in Settings/Settings.hpp
    if (now - prevRefresh < BUTTON_DEBOUNCE_TIME)
        return;

    for (size_t row = 0; row < nb_rows; row++) { // scan through all rows
        pinMode(rowPins[row], OUTPUT);           // make the current row Lo-Z 0V
        for (size_t col = 0; col < nb_cols; col++) { // scan through all columns
            bool state = digitalRead(colPins[col]);  // read the state
            if (state != getPrevState(col, row)) {
                // if the state changed since last time
                // execute the handler
                onButtonChanged(row, col, state);
                setPrevState(col, row, state); // remember the state
                prevRefresh = now;
            }
        }
        pinMode(rowPins[row], INPUT); // make the current row Hi-Z again
    }
}

template <uint8_t nb_rows, uint8_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::begin() {
    // make all columns input pins and enable
    // the internal pull-up resistors
    for (const pin_t &colPin : colPins)
        pinMode(colPin, INPUT_PULLUP);
    // make all rows Hi-Z
    for (const pin_t &rowPin : rowPins)
        pinMode(rowPin, INPUT);
}

template <uint8_t nb_rows, uint8_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::positionToBits(uint8_t col,
                                                              uint8_t row) {
    // map from a 2D array of bits to a flat array of bits
    return col * nb_rows + row;
}

template <uint8_t nb_rows, uint8_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::bitsToIndex(uint8_t bits) {
    return bits >> 3; // bits / 8
}

template <uint8_t nb_rows, uint8_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::bitsToBitmask(uint8_t bits) {
    return 1 << (bits & 7); // bits % 8
}

template <uint8_t nb_rows, uint8_t nb_cols>
bool ButtonMatrix<nb_rows, nb_cols>::getPrevState(uint8_t col, uint8_t row) {
    uint8_t bits = positionToBits(col, row);
    return !!(prevStates[bitsToIndex(bits)] & bitsToBitmask(bits));
}

template <uint8_t nb_rows, uint8_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::setPrevState(uint8_t col, uint8_t row,
                                                  bool state) {
    uint8_t bits = positionToBits(col, row);
    if (state)
        prevStates[bitsToIndex(bits)] |= bitsToBitmask(bits);
    else
        prevStates[bitsToIndex(bits)] &= ~bitsToBitmask(bits);
}

END_AH_NAMESPACE