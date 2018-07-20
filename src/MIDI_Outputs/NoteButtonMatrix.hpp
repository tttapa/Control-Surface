#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonMatrix.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

template <uint8_t nb_rows, uint8_t nb_cols>
class NoteButtonMatrix
    : public MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols> {
  public:
    NoteButtonMatrix(const pin_t (&rowPins)[nb_rows],
                     const pin_t (&colPins)[nb_cols],
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     uint8_t channel)
        : MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols>(
              rowPins, colPins, addresses, channel) {}
};

namespace Bankable {

template <uint8_t nb_rows, uint8_t nb_cols>
class NoteButtonMatrix
    : public MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols> {
  public:
    NoteButtonMatrix(const pin_t (&rowPins)[nb_rows],
                     const pin_t (&colPins)[nb_cols],
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     uint8_t channel)
        : MIDIButtonMatrix<DigitalNoteSender, nb_rows, nb_cols>(
              rowPins, colPins, addresses, channel) {}
};

} // namespace Bankable
