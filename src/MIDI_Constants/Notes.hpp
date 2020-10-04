/* ✔ */

#pragma once

#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// @addtogroup    MIDIConstants
/// @{

/// MIDI note names.
namespace MIDI_Notes {

constexpr int8_t C = 0;   ///< C (Do)
constexpr int8_t Db = 1;  ///< C♯, D♭ (Do sharp, Re flat)
constexpr int8_t D = 2;   ///< D (Re)
constexpr int8_t Eb = 3;  ///< D♯, E♭ (Re sharp, Mi flat)
constexpr int8_t E = 4;   ///< E (Mi)
constexpr int8_t F = 5;   ///< F (Fa)
constexpr int8_t Gb = 6;  ///< F♯, G♭ (Fa sharp, Sol flat)
constexpr int8_t G = 7;   ///< G (Sol)
constexpr int8_t Ab = 8;  ///< G♯, A♭ (Sol sharp, La flat)
constexpr int8_t A = 9;   ///< A (La)
constexpr int8_t Bb = 10; ///< A♯, B♭ (La sharp, Si flat)
constexpr int8_t B = 11;  ///< B (Si)

/// Get the first MIDI note in the given octave.
constexpr int8_t octave(int8_t numOctave) { return 12 * (numOctave + 1); }

/// Get the MIDI note in the given octave.
constexpr int8_t note(int8_t note, int8_t numOctave) {
    return note + octave(numOctave);
}

} // namespace MIDI_Notes

/// @}

END_CS_NAMESPACE