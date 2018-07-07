#pragma once

#include "Abstract/MIDIButtonMatrix.hpp"

/**
 * @brief   A class for button matrices that send MIDI Controller Change events.
 * 
 * @tparam  nb_rows
 *          The number of rows in the button matrix.
 * @tparam  nb_cols
 *          The number of columns in the button matrix.
 */
template <size_t nb_rows, size_t nb_cols>
class CCButtonMatrix : public MIDIButtonMatrix<nb_rows, nb_cols> {
  public:
    /**
     * @brief   Construct a new CCButtonMatrix object.
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
     * @param   addresses
     *          A 2D array of dimensions (nb_rows × nb_cols) containing the
     *          controller numbers.
     * @note    The list of addresses is not copied. It should be a reference to
     *          a 2-dimensional array. This means that initializing `addresses`
     *          with a brace-enclosed initializer list is not allowed.
     * @param   channel
     *          The MIDI channel. [1, 16]
     * @param   offValue
     *          The Controller value to send when a button is released.
     *          [0,127]
     * @param   onValue
     *          The Controller value to send when a button is pressed.
     *          [0,127]
     */
    CCButtonMatrix(const pin_t (&rowPins)[nb_rows],
                   const pin_t (&colPins)[nb_cols],
                   const uint8_t (&addresses)[nb_rows][nb_cols],
                   uint8_t channel, uint8_t offValue = 0, uint8_t onValue = 127)
        : MIDIButtonMatrix<nb_rows, nb_cols>(rowPins, colPins, addresses, channel),
          offValue(offValue), onValue(onValue) {}

  private:
    void send(uint8_t address, bool state) const final override {}

    const uint8_t offValue;
    const uint8_t onValue;
};
