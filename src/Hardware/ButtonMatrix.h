#pragma once

#include "../Hardware/ExtendedInputOutput/ExtendedInputOutput.h" // for pin_t

/**
 * @brief   A MIDI_Control_Element that reads the states of a button matrix and
 *          sends it over MIDI as a series of Note events.
 *
 * @tparam  nb_rows
 *          The number of rows in the button matrix.
 * @tparam  nb_cols
 *          The number of columns in the button matrix.
 */
template <uint8_t nb_rows, uint8_t nb_cols>
class ButtonMatrix {
  public:
    /**
     * @brief   Construct a new ButtonMatrix object.
     *
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix. These pins will be driven LOW (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix. These pins will be used as inputs, and the internal
     *          pull-up resistor will be enabled.
     * @note    The lists of pins are copied. This means that they can be 
     *          initialized with a brace-enclosed initializer list.
     */
    ButtonMatrix(const pin_t (&rowPins)[nb_rows],
                 const pin_t (&colPins)[nb_cols]);
    /**
     * @brief   Destructor.
     */
    ~ButtonMatrix();

  protected:
    bool isActive() const;

  private:
    void init();
    void refresh();

    virtual void onButtonChanged(uint8_t row, uint8_t col, bool state) = 0;

    inline uint8_t positionToBits(uint8_t col, uint8_t row);
    inline uint8_t bitsToIndex(uint8_t bits);
    inline uint8_t bitsToBitmask(uint8_t bits);
    bool getPrevState(uint8_t col, uint8_t row);
    void setPrevState(uint8_t col, uint8_t row, bool state);
    bool allReleased();

    bool active;

    unsigned long prevRefresh = 0;
    uint8_t *prevStates = nullptr;

    pin_t rowPins[nb_rows];
    pin_t colPins[nb_cols];
};

#include "ButtonMatrix.ipp" // Template implementations