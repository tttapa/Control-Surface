#ifndef BUTTONMATRIX_H_
#define BUTTONMATRIX_H_

#include "Arduino.h"
#include "./MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h" // for pin_t

template <size_t nb_rows, size_t nb_cols>
class ButtonMatrix : public MIDI_Control_Element
{
  public:
    ButtonMatrix(const pin_t (&rowPins)[nb_rows], const pin_t (&colPins)[nb_cols], const uint8_t (&addresses)[nb_rows][nb_cols], uint8_t channel, uint8_t velocity = 127);
    ~ButtonMatrix();

  private:
    void refresh();

    void init();

    inline uint8_t positionToBits(uint8_t col, uint8_t row);
    inline uint8_t bitsToIndex(uint8_t bits);
    inline uint8_t bitsToBitmask(uint8_t bits);
    bool getPrevState(uint8_t col, uint8_t row);
    void setPrevState(uint8_t col, uint8_t row, bool state);

    unsigned long prevRefresh = 0;
    uint8_t *prevStates = nullptr;

    pin_t rowPins[nb_rows], colPins[nb_cols];

    const uint8_t (&addresses)[nb_rows][nb_cols];
    const uint8_t channel, velocity;
};

#ifdef __AVR__
#include "./ButtonMatrix.cpp" // Because it's a template class
#endif

#endif // BUTTONMATRIX_H_