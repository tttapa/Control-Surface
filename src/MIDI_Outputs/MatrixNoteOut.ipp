#include "MatrixNoteOut.h"

using namespace ExtIO;

template <size_t nb_rows, size_t nb_cols>
MatrixNoteOut<nb_rows, nb_cols>::MatrixNoteOut(const pin_t (&rowPins)[nb_rows],
                                             const pin_t (&colPins)[nb_cols]) {
    // Note: this copies the data of rowPins and colPins, regardless of whether
    // it's static or temporary. This means that using global const arrays to
    // initialize ButtonMatrix is slightly less memory efficient than using
    // brace-enclosed initializer lists. There are ways around this, but it's 
    // not really pretty:
    // https://stackoverflow.com/questions/46382034/template-class-with-variable-array-size-initialize-with-array-reference-or-brac
    copy(this->rowPins, rowPins);
    copy(this->colPins, colPins);
    init();
}

template <size_t nb_rows, size_t nb_cols>
ButtonMatrix<nb_rows, nb_cols>::~ButtonMatrix() {
    delete[] prevStates;
}

template <size_t nb_rows, size_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::update() {
    unsigned long now = millis();
    // only update every 25 ms (crude software
    // debounce). Edit this in ../Settings/Settings.h
    if (now - prevRefresh < BUTTON_DEBOUNCE_TIME)
        return;
    prevRefresh = now;

    for (size_t row = 0; row < nb_rows; row++) // scan through all rows
    {
        pinMode(rowPins[row], OUTPUT); // make the current row Lo-Z 0V
        for (size_t col = 0; col < nb_cols; col++) // scan through all columns
        {
            bool state = digitalRead(colPins[col]); // read the state
            if (state != getPrevState(col, row)) {
                // if the state changed since last time
                // execute the handler
                onButtonChanged(row, col, state);
                setPrevState(col, row, state); // remember the state
                active = !allReleased();
            }
        }
        pinMode(rowPins[row], INPUT); // make the current row Hi-Z again
    }
}

template <size_t nb_rows, size_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::init() {
    // Create an array of bytes where each bit represents
    // the state of one of the buttons
    prevStates = new uint8_t[(nb_cols * nb_rows + 7) / 8];
    memset(prevStates, 0xFF, (nb_cols * nb_rows + 7) / 8);
    active = false;
    // make all columns input pins and enable
    // the internal pull-up resistors
    for (const pin_t &colPin : colPins)
        pinMode(colPin, INPUT_PULLUP);
    // make all rows Hi-Z
    for (const pin_t &rowPin : rowPins)
        pinMode(rowPin, INPUT);
}

template <size_t nb_rows, size_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::positionToBits(uint8_t col,
                                                              uint8_t row) {
    // map from a 2D array of bits to a flat array of bits
    return col * nb_rows + row;
}

template <size_t nb_rows, size_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::bitsToIndex(uint8_t bits) {
    return bits >> 3; // bits / 8
}

template <size_t nb_rows, size_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::bitsToBitmask(uint8_t bits) {
    return 1 << (bits & 7); // bits % 8
}

template <size_t nb_rows, size_t nb_cols>
bool ButtonMatrix<nb_rows, nb_cols>::getPrevState(uint8_t col, uint8_t row) {
    uint8_t bits = positionToBits(col, row);
    return !!(prevStates[bitsToIndex(bits)] & bitsToBitmask(bits));
}

template <size_t nb_rows, size_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::setPrevState(uint8_t col, uint8_t row,
                                                  bool state) {
    uint8_t bits = positionToBits(col, row);
    if (state)
        prevStates[bitsToIndex(bits)] |= bitsToBitmask(bits);
    else
        prevStates[bitsToIndex(bits)] &= ~bitsToBitmask(bits);
}

template <size_t nb_rows, size_t nb_cols>
bool ButtonMatrix<nb_rows, nb_cols>::allReleased() {
    size_t nb_bytes = (nb_cols * nb_rows + 7) / 8;
    for (size_t i = 0; i < nb_bytes; i++) {
        if (prevStates[i] != 0xFF)
            return false;
    }
    return true;
}

template <size_t nb_rows, size_t nb_cols>
bool ButtonMatrix<nb_rows, nb_cols>::isActive() {
    return this->active;
}