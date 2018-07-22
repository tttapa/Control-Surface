#pragma once

#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h> // for pin_t
#include <Settings/SettingsWrapper.h>
#include <Helpers/Array.hpp>

template <size_t N>
using PinList = Array<pin_t, N>;

/**
 * @brief   A MIDI_Control_Element that reads the states of a button matrix and
 *          sends it over MIDI.
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
     */
    ButtonMatrix(const PinList<nb_rows> &rowPins,
                 const PinList<nb_cols> &colPins);
    /**
     * @brief   Destructor.
     */
    ~ButtonMatrix();

    void refresh();

  private:
    void init();

    virtual void onButtonChanged(uint8_t row, uint8_t col, bool state) = 0;

    static inline uint8_t positionToBits(uint8_t col, uint8_t row);
    static inline uint8_t bitsToIndex(uint8_t bits);
    static inline uint8_t bitsToBitmask(uint8_t bits);
    bool getPrevState(uint8_t col, uint8_t row);
    void setPrevState(uint8_t col, uint8_t row, bool state);
    // bool allReleased();

    unsigned long prevRefresh = 0;
    uint8_t *prevStates = nullptr;

    const PinList<nb_rows> rowPins;
    const PinList<nb_cols> colPins;
};

#include "ButtonMatrix.ipp" // Template implementations