#pragma once

#include <Banks/Bank.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>
#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

/// Class for transposing the address of @ref NoteButton and other MIDI elements.
template <int8_t MinTransposition, int8_t MaxTransposition>
class Transposer : public Bank<MaxTransposition - MinTransposition + 1> {
  public:
    Transposer(int8_t step = 1)
        : Bank<NumBanks>(step, -MinTransposition, MinTransposition) {
        static_assert(MinTransposition <= 0,
                      "Error: the minimum transposition must be negative");
        static_assert(MaxTransposition >= 0,
                      "Error: the maximum transposition must be positive");
    }

    /// Set the transposition.
    /// @param  tp
    ///         The new transposition in the range
    ///         [ @p MinTransposition, @p MaxTransposition ].
    /// @note   The @ref Bank::select() method expects a zero-based argument,
    ///         which is cumbersome if the minimum transposition is nonzero.
    void setTransposition(int8_t tp) { this->select(tp - MinTransposition); }

    /// Get the transposition.
    int8_t getTransposition() const {
        return this->getSelection() + MinTransposition;
    }

    /// Get the transposition as a number of semitones.
    int8_t getTranspositionSemitones() const { return this->getOffset(); }

    static constexpr setting_t N = MaxTransposition - MinTransposition + 1;
    static constexpr setting_t NumBanks = N;
};

END_CS_NAMESPACE