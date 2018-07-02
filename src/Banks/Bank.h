#ifndef BANK_h_
#define BANK_h_

#include "../MIDI_Element.h"

/**
 * @brief A class that groups MIDI_Element%s and allows the user
 * to change the MIDI addresses (Controller number or Note number)
 * or the MIDI channels of these elements.
 *
 * Consider two controls: \c volume_A and \c volume_B:
 * \c volume_A has controller 0x07 on MIDI channel 1, and
 * \c volume_B has controller 0x07 on MIDI channel 2.
 * Let's add them to a bank with 2 tracks per bank.
 * @code{.cpp}
 * AnalogCC volume_A(A0, MIDI_CC::Channel_Volume, 1);
 * AnalogCC volume_B(A1, MIDI_CC::Channel_Volume, 2);
 *
 * Bank bank(2);
 *
 * bank.add(volume_A, Bank::CHANGE_CHANNEL);
 * bank.add(volume_B, Bank::CHANGE_CHANNEL);
 * @endcode
 * By default, the bank setting is 0, so it has no impact:
 * \c volume_A still sends MIDI messages on channel 1, and
 * \c volume_B still sends MIDI messages on channel 2.
 * However, when the bank setting is changed, this is
 * no longer the case:
 * @code{.cpp}
 * bank.setBankSetting(1);
 * @endcode
 * Now that the bank setting is set to one, the number of
 * tracks per bank is added to the channel of each element
 * in the bank:
 * \c volume_A now sends MIDI messages on channel 3, and
 * \c volume_B now sends MIDI messages on channel 4.
 *
 * When the bank setting is set to 2, \c volume_A will use
 * channel 5, and \c volume_B channel 6, etc.
 *
 * When using the bank type Bank::CHANGE_ADDRESS, the same
 * thing happens, but the address changes and the channel
 * stays the same.
 *
 * To change the bank setting, a BankSelector can be used.
 *
 * Apart from changing the address or channel of an element,
 * banks can also be used to apply a single map function
 * to many Analog (AnalogCC or AnalogPB) elements at the same time.
 *
 * @f$ c = c_0 + p \cdot s @f$ &emsp; where @f$c@f$ is the effective channel,
 * @f$c_0@f$ is the base channel of the MIDI_Element, @f$p@f$ is the number of
 * tracks per bank, and @f$s@f$ is the current bank setting.
 */
class Bank {
public:
  /**
   * @brief Construct a new Bank object
   *
   * @param tracksPerBank The number of tracks (i.e. addresses or channels) that
   * are skipped when the bank setting changes.
   */
  Bank(uint8_t tracksPerBank = 1);
  ~Bank();
  enum bankType {
    /**
     * @brief Change the offset of the channel number of the MIDI_Element.
     */
    CHANGE_CHANNEL,
    /**
     * @brief Change the offset of the address (i.e. Controller number or Note
     * number) of the MIDI_Element.
     */
    CHANGE_ADDRESS
  };
  /**
   * @brief Add a MIDI_Element to the bank.
   *
   * @param element A pointer to the MIDI_Element to be added.
   * @param type Either change the MIDI channel or the address.
   */
  void add(MIDI_Element *element, bankType type = CHANGE_ADDRESS);
  /**
   * @brief Add a MIDI_Element to the bank.
   *
   * @param element The MIDI_Element to be added.
   * @param type Either change the MIDI channel or the address.
   */
  void add(MIDI_Element &element, bankType type = CHANGE_ADDRESS);
  /**
   * @brief Add an array of MIDI_Element%s to the bank.
   *
   * @tparam T The type of the MIDI_Element%s.
   * @tparam N The number of MIDI_Element%s in the array.
   *
   * @param element An array of MIDI_Element%s to be added.
   * @param type Either change the MIDI channel or the address of the elements.
   */
  template <class T, size_t N>
  void add(T (&arr)[N], bankType type = CHANGE_ADDRESS) {
    for (size_t i = 0; i < N; i++)
      add(arr[i], type);
  }
  /**
   * @brief Set the Bank Setting
   *
   * @param bankSetting The new Bank Setting.
   */
  void setBankSetting(uint8_t bankSetting);
  /**
   * @brief Apply a mapping function to all Analog elements (AnalogBase::map) in
   * the bank.
   *
   * @param fn A pointer to the mapping function to be applied.
   */
  void map(int (*fn)(int));

private:
  const uint8_t tracksPerBank;
  struct MIDI_Element_list_node {
    MIDI_Element *element;
    MIDI_Element_list_node *next;
    bankType type;
  };
  MIDI_Element_list_node *firstMIDI_Element = nullptr;
  MIDI_Element_list_node *lastMIDI_Element = nullptr;
};

#endif // BANK_h_