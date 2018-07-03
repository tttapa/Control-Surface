#ifndef TRANSPOSER_H_
#define TRANSPOSER_H_

#include "../Banks/Bank.h"
#include "./Selector.h"

/**
 * @brief A class for transposing notes.
 *
 * One button makes all affected buttons
 * send a note one semitone higher, another
 * decreases the note number by one semitone.
 *
 * @todo    Create virtual increment/decrement methods
 *          that allow you to decide in the supclasses
 *          whether to clamp the min/max setting or to
 *          wrap around.
 */
class Transposer : public Selector {
  public:
    template <class T, size_t N>
    Transposer(Bank &bank, const T (&switchPins)[N],
               int8_t minimumSemitones = -12, int8_t maximumSemitones = 12)
        : Selector((const pin_t (&)[N])switchPins,
                   (size_t)maximumSemitones - minimumSemitones + 1),
          bank(bank), minimumSemitones(minimumSemitones) {
        setSetting(-minimumSemitones);
    }

    int8_t getTransposition() { return settingToSemitones(getSetting()); }

  private:
    Bank &bank;
    const int8_t minimumSemitones;
    int8_t settingToSemitones(uint8_t setting) {
        return setting + minimumSemitones;
    }
    void updateImpl(uint8_t newSetting) {
        bank.setBankSetting(settingToSemitones(newSetting));
    }
};

#endif // TRANSPOSER_H_