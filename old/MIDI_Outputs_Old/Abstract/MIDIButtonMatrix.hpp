#pragma once

#include "../../Hardware/ButtonMatrix.h"
#include "../../Settings/SettingsWrapper.h"
#include "DigitalMIDIOut.h"

/**
 * @brief   A class for button matrices that send MIDI events.
 *
 * @tparam  nb_rows
 *          The number of rows in the button matrix.
 * @tparam  nb_cols
 *          The number of columns in the button matrix.
 */
template <size_t nb_rows, size_t nb_cols>
class MIDIButtonMatrix : public DigitalMIDIOut,
                         public ButtonMatrix<nb_rows, nb_cols> {
  public:
    /**
     * @brief   Construct a new MIDIButtonMatrix object.
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
     *          addresses.
     * @note    The list of addresses is not copied. It should be a reference to
     *          a 2-dimensional array. This means that initializing `addresses`
     *          with a brace-enclosed initializer list is not allowed.
     * @param   channel
     *          The MIDI channel. [1, 16]
     */
    MIDIButtonMatrix(const pin_t (&rowPins)[nb_rows],
                     const pin_t (&colPins)[nb_cols],
                     const uint8_t (&addresses)[nb_rows][nb_cols],
                     uint8_t channel)
        : ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins),
          addresses(addresses), channel(channel) {}

  private:
    /**
     * @brief   Send the button state of the given address over MIDI.
     *
     * @param   address
     *          The address of the button of which the state changed.
     * @param   state
     *          The new state of the button.
     */
    virtual void send(uint8_t address, bool state) const = 0;

    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        send(addresses[row][col], state);
    }

    void refresh() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

    bool isActive() const final override {
        return ButtonMatrix<nb_rows, nb_cols>::isActive();
    }

    const uint8_t (&addresses)[nb_rows][nb_cols];
    const uint8_t channel;
};
