/* ✔ */

#pragma once

#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// @addtogroup    MIDIConstants
/// @{

/// MIDI note names.
namespace MIDI_Notes {

class Note {
  private:
    int8_t base;
    constexpr Note(int8_t base) : base(base) {}

  public:
    constexpr int8_t operator()(int8_t octave = -1) const {
        return base + 12 * (octave + 1);
    }

    constexpr static Note C() { return Note{0}; }
    constexpr static Note Db() { return Note{1}; }
    constexpr static Note D() { return Note{2}; }
    constexpr static Note Eb() { return Note{3}; }
    constexpr static Note E() { return Note{4}; }
    constexpr static Note F_() { return Note{5}; }
    constexpr static Note Gb() { return Note{6}; }
    constexpr static Note G() { return Note{7}; }
    constexpr static Note Ab() { return Note{8}; }
    constexpr static Note A() { return Note{9}; }
    constexpr static Note Bb() { return Note{10}; }
    constexpr static Note B() { return Note{11}; }
};

constexpr Note C = Note::C();   ///< C (Do)
constexpr Note Db = Note::Db(); ///< C♯, D♭ (Do sharp, Re flat)
constexpr Note D = Note::D();   ///< D (Re)
constexpr Note Eb = Note::Eb(); ///< D♯, E♭ (Re sharp, Mi flat)
constexpr Note E = Note::E();   ///< E (Mi)
constexpr Note F_ = Note::F_(); ///< F (Fa)
constexpr Note Gb = Note::Gb(); ///< F♯, G♭ (Fa sharp, Sol flat)
constexpr Note G = Note::G();   ///< G (Sol)
constexpr Note Ab = Note::Ab(); ///< G♯, A♭ (Sol sharp, La flat)
constexpr Note A = Note::A();   ///< A (La)
constexpr Note Bb = Note::Bb(); ///< A♯, B♭ (La sharp, Si flat)
constexpr Note B = Note::B();   ///< B (Si)

/// Get the MIDI note in the given octave.
[[deprecated("Instead of `note(C, 4)`, use `MIDI_Notes::C(4)`")]] //
constexpr int8_t
note(Note note, int8_t numOctave) {
    return note(numOctave);
}

} // namespace MIDI_Notes

/// @}

END_CS_NAMESPACE