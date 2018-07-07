#pragma once

#include "../Hardware/ButtonMatrix.h"
#include "../Settings/SettingsWrapper.h"
#include "Abstract/DigitalMIDIOut.h"

/**
 * @brief   A class for MIDI_Control_Element%s that send MIDI notes on many
 *          different note numbers.
 * 
 * @tparam  nb_rows
 *          The number of rows in the note matrix.
 * @tparam  nb_cols
 *          The number of columns in the note matrix.
 */
template <size_t nb_rows, size_t nb_cols>
class MatrixNoteOut : public DigitalMIDIOut,
                      public ButtonMatrix<nb_rows, nb_cols> {
  public:
    /**
     * @brief   Construct a new MatrixNoteOut object.
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
    MatrixNoteOut(const pin_t (&rowPins)[nb_rows],
                  const pin_t (&colPins)[nb_cols],
                  const uint8_t (&addresses)[nb_rows][nb_cols], uint8_t channel,
                  uint8_t velocity = 0x7F); // Constructor

    void setVelocity(uint8_t velocity);

  protected:
    void send(uint8_t row, uint8_t col, bool state);
    void refresh() override;

  private:
    bool isActive() const override;

    uint8_t velocity;
};