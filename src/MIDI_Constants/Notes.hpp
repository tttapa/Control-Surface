/* ✔ */

#pragma once

#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// @addtogroup    MIDIConstants
/// @{

/**
 * @brief   MIDI note names.
 */
namespace MIDI_Notes {

constexpr int8_t C = 0;
constexpr int8_t Db = 1;
constexpr int8_t D = 2;
constexpr int8_t Eb = 3;
constexpr int8_t E = 4;
constexpr int8_t F = 5;
constexpr int8_t Gb = 6;
constexpr int8_t G = 7;
constexpr int8_t Ab = 8;
constexpr int8_t A = 9;
constexpr int8_t Bb = 10;
constexpr int8_t B = 11;

/// Get the first MIDI note in the given octave.
constexpr int8_t octave(int8_t numOctave) { return 12 * (numOctave + 1); }

/// Get the MIDI note in the given octave.
constexpr int8_t note(int8_t note, int8_t numOctave) {
    return note + octave(numOctave);
}

} // namespace MIDI_Notes

/// @}

END_CS_NAMESPACE