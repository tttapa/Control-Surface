/* ✔ */

#pragma once

#include <Settings/NamespaceSettings.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/// @addtogroup    MIDIConstants
/// @{

/// MIDI note names.
/// Uses the [Scientific Pitch Notation system](https://en.wikipedia.org/wiki/Scientific_pitch_notation),
/// where <b>A<sub>4</sub></b> is 440 Hz, and <b>C<sub>-1</sub></b> is 8.1758 Hz.
///
/// |Octave| C |C♯/D♭| D |D♯/E♭| E | F  |F♯/G♭| G |G♯/A♭| A |A♯/B♭| B  |
/// |:-----|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
/// |-1    |   0|   1|   2|   3|   4|   5|   6|   7|   8|   9|  10|  11|
/// |0     |  12|  13|  14|  15|  16|  17|  18|  19|  20|  21|  22|  23|
/// |1     |  24|  25|  26|  27|  28|  29|  30|  31|  32|  33|  34|  35|
/// |2     |  36|  37|  38|  39|  40|  41|  42|  43|  44|  45|  46|  47|
/// |3     |  48|  49|  50|  51|  52|  53|  54|  55|  56|  57|  58|  59|
/// |4     |  60|  61|  62|  63|  64|  65|  66|  67|  68|  69|  70|  71|
/// |5     |  72|  73|  74|  75|  76|  77|  78|  79|  80|  81|  82|  83|
/// |6     |  84|  85|  86|  87|  88|  89|  90|  91|  92|  93|  94|  95|
/// |7     |  96|  97|  98|  99| 100| 101| 102| 103| 104| 105| 106| 107|
/// |8     | 108| 109| 110| 111| 112| 113| 114| 115| 116| 117| 118| 119|
namespace MIDI_Notes {

class Note {
  private:
    int8_t base;
    constexpr Note(int8_t base) : base(base) {}

  public:
    constexpr int8_t operator()(int8_t octave = -1) const {
        return base + 12 * (octave + 1);
    }
    constexpr int8_t operator[](int8_t octave) const { return (*this)(octave); }

    constexpr static Note(C)() { return Note {0}; }
    constexpr static Note(Db)() { return Note {1}; }
    constexpr static Note(D)() { return Note {2}; }
    constexpr static Note(Eb)() { return Note {3}; }
    constexpr static Note(E)() { return Note {4}; }
    constexpr static Note(F)() { return Note {5}; }
    constexpr static Note(Gb)() { return Note {6}; }
    constexpr static Note(G)() { return Note {7}; }
    constexpr static Note(Ab)() { return Note {8}; }
    constexpr static Note(A)() { return Note {9}; }
    constexpr static Note(Bb)() { return Note {10}; }
    constexpr static Note(B)() { return Note {11}; }
};

constexpr Note C = (Note::C)();   ///< C (Do)
constexpr Note Db = (Note::Db)(); ///< C♯, D♭ (Do sharp, Re flat)
constexpr Note D = (Note::D)();   ///< D (Re)
constexpr Note Eb = (Note::Eb)(); ///< D♯, E♭ (Re sharp, Mi flat)
constexpr Note E = (Note::E)();   ///< E (Mi)
constexpr Note F = (Note::F)();   ///< F (Fa)
constexpr Note Gb = (Note::Gb)(); ///< F♯, G♭ (Fa sharp, Sol flat)
constexpr Note G = (Note::G)();   ///< G (Sol)
constexpr Note Ab = (Note::Ab)(); ///< G♯, A♭ (Sol sharp, La flat)
constexpr Note A = (Note::A)();   ///< A (La)
constexpr Note Bb = (Note::Bb)(); ///< A♯, B♭ (La sharp, Si flat)
constexpr Note B = (Note::B)();   ///< B (Si)

CS_DEPREC("Instead of `MIDI_Notes::F_(4)`, use `MIDI_Notes::F[4]`")
constexpr Note F_ = (Note::F)(); ///< F (Fa)

/// Get the MIDI note in the given octave.
CS_DEPREC("Instead of `note(C, 4)`, use `MIDI_Notes::C(4)`")
constexpr int8_t note(Note note, int8_t numOctave) { return note(numOctave); }

} // namespace MIDI_Notes

/// @}

END_CS_NAMESPACE