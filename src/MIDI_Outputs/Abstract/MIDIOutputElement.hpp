#pragma once

#include <Helpers/Updatable.hpp>
#include <stdint.h>

/** // TODO
 * @brief   A base class for all MIDI output elements.
 *
 * All MIDI output elements are added to a linked list,
 * so they can be updated all at once by simply looping over
 * the entire list.
 */
class MIDIOutputElement : public Updatable {
  protected:
    MIDIOutputElement() = default;
};