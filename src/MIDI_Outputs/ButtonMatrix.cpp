#ifndef BUTTONMATRIX_CPP_
#define BUTTONMATRIX_CPP_

#include "./ButtonMatrix.h"
#include "../MIDI_Controller/MIDI_Controller_Class.h"

// public:
template <size_t nb_rows, size_t nb_cols>
ButtonMatrix<nb_rows, nb_cols>::ButtonMatrix(const pin_t (&rowPins)[nb_rows], const pin_t (&colPins)[nb_cols], const uint8_t (&addresses)[nb_rows][nb_cols], uint8_t channel, uint8_t velocity = 127)
    : addresses(addresses), channel(channel), velocity(velocity)
{
    memcpy(this->rowPins, rowPins, sizeof(rowPins[0]) * nb_rows);
    memcpy(this->colPins, colPins, sizeof(colPins[0]) * nb_cols);
    init();
}

template <size_t nb_rows, size_t nb_cols>
ButtonMatrix<nb_rows, nb_cols>::~ButtonMatrix()
{
    delete[] prevStates;
}

// private:
template <size_t nb_rows, size_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::refresh()
{
    unsigned long now = millis();
    if (now - prevRefresh < 20)
        return;
    prevRefresh = now;

    for (int row = 0; row < nb_rows; row++)
    {
        pinMode(rowPins[row], OUTPUT);
        for (int col = 0; col < nb_cols; col++)
        {
            bool state = digitalRead(colPins[col]);
            if (state != getPrevState(col, row))
            {
                uint8_t note = addresses[col][row];
                MIDI_Controller.MIDI()->send(state ? NOTE_OFF : NOTE_ON, channel + channelOffset * channelsPerBank, note + addressOffset * channelsPerBank, velocity);
                setPrevState(col, row, state);
            }
        }
        pinMode(rowPins[row], INPUT);
    }
}

template <size_t nb_rows, size_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::init()
{
    prevStates = new uint8_t[(nb_cols * nb_rows + 7) / 8];
    memset(prevStates, 0xFF, (nb_cols * nb_rows + 7) / 8);
    for (int i = 0; i < nb_cols; i++)
    {
        pinMode(colPins[i], INPUT_PULLUP);
    }
    for (int row = 0; row < nb_rows; row++)
    {
        pinMode(rowPins[row], INPUT);
        digitalWrite(rowPins[row], 0);
    }
}

template <size_t nb_rows, size_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::positionToBits(uint8_t col, uint8_t row)
{
    return col * 8 + row;
}

template <size_t nb_rows, size_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::bitsToIndex(uint8_t bits)
{
    return bits >> 3; // bits / 8
}

template <size_t nb_rows, size_t nb_cols>
inline uint8_t ButtonMatrix<nb_rows, nb_cols>::bitsToBitmask(uint8_t bits)
{
    return 1 << (bits & 7); // bits % 8
}

template <size_t nb_rows, size_t nb_cols>
bool ButtonMatrix<nb_rows, nb_cols>::getPrevState(uint8_t col, uint8_t row)
{
    uint8_t bits = positionToBits(col, row);
    return !!(prevStates[bitsToIndex(bits)] & bitsToBitmask(bits));
}

template <size_t nb_rows, size_t nb_cols>
void ButtonMatrix<nb_rows, nb_cols>::setPrevState(uint8_t col, uint8_t row, bool state)
{
    uint8_t bits = positionToBits(col, row);
    if (state)
        prevStates[bitsToIndex(bits)] |= bitsToBitmask(bits);
    else
        prevStates[bitsToIndex(bits)] &= ~bitsToBitmask(bits);
}

#endif // BUTTONMATRIX_CPP_