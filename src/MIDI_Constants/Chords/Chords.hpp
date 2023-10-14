#pragma once

#include "Intervals.hpp"
#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

class IChord {
  public:
    virtual ~IChord() = default;
    virtual const int8_t *begin() const = 0;
    virtual const int8_t *end() const = 0;
};

template <uint8_t N>
class Chord : public IChord {
  public:
    Chord(const Array<int8_t, N> &offsets) : offsets(offsets) {}
    const int8_t *begin() const override { return offsets.begin(); }
    const int8_t *end() const override { return offsets.end(); }

    template <uint8_t M>
    Chord<N + M> operator+(const Chord<M> &rhs) const {
        return {cat(this->getOffsets(), rhs.getOffsets())};
    }

    Chord<N + 1> operator+(int8_t rhs) const {
        return *this + Chord<1> {{rhs}};
    }

    const Array<int8_t, N> getOffsets() const { return offsets; }

  private:
    Array<int8_t, N> offsets;
};

/// @addtogroup MIDIConstants
/// @{

/// Predefined Chord constants.
namespace Chords {

using namespace Intervals;

const Chord<2> Major = {{M3, P5}};
const Chord<2> MajorFirstInv = {{M3, P5 - P8}};       // First inversion
const Chord<2> MajorSecondInv = {{M3 - P8, P5 - P8}}; // Second inversion

const Chord<2> Minor = {{m3, P5}};
const Chord<2> MinorFirstInv = {{m3, P5 - P8}};
const Chord<2> MinorSecondInv = {{m3 - P8, P5 - P8}};

const Chord<2> Diminished = {{m3, d5}};
const Chord<2> Augmented = {{m3, m6}};

const Chord<3> DominantSeventh = {{M3, P5, m7}};
const Chord<3> MajorSeventh = {{M3, P5, M7}};

} // namespace Chords

/// Predefined Chord constants with bass notes.
namespace Bass {

using namespace Intervals;

const Chord<1> Single = {{-P8}};
const Chord<2> Double = {{-P8, -2 * P8}};
const Chord<3> Triple = {{-P8, -2 * P8, -3 * P8}};

} // namespace Bass

/// @}

END_CS_NAMESPACE