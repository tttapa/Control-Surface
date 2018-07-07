#pragma once

#include "Abstract/MIDIButtonMatrix.hpp"

/**
 * @brief   A class for button matrices that send MIDI Note events.
 * 
 * @tparam  nb_rows
 *          The number of rows in the button matrix.
 * @tparam  nb_cols
 *          The number of columns in the button matrix.
 */
template <size_t nb_rows, size_t nb_cols>
class NoteButtonMatrix : public MIDIButtonMatrix<nb_rows, nb_cols> {
  public:
    /**
     * @brief   Construct a new NoteButtonMatrix object.
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
     *          note numbers.
     * @note    The list of addresses is not copied. It should be a reference to
     *          a 2-dimensional array. This means that initializing `addresses`
     *          with a brace-enclosed initializer list is not allowed.
     * @param   channel
     *          The MIDI channel. [1, 16]
     * @param   velocity
     *          The MIDI Note velocity. [1, 127]
     */
    NoteButtonMatrix(const pin_t (&rowPins)[nb_rows],
                     const pin_t (&colPins)[nb_cols],
                     const uint8_t (&addresses)[nb_rows][nb_cols],
                     uint8_t channel, uint8_t velocity = 0x7F)
        : MIDIButtonMatrix<nb_rows, nb_cols>(rowPins, colPins, addresses, channel),
          velocity(velocity) {}

  private:
    void send(uint8_t address, bool state) const final override {}

    uint8_t velocity;
};
