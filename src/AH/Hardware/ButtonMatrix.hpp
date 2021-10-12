/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/Hardware-Types.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   A class that reads the states of a button matrix.
 *
 * @tparam  NumRows
 *          The number of rows in the button matrix.
 * @tparam  NumCols
 *          The number of columns in the button matrix.
 * 
 * @ingroup AH_HardwareUtils
 */
template <class Derived, uint8_t NumRows, uint8_t NumCols>
class ButtonMatrix {
  public:
    /**
     * @brief   Construct a new ButtonMatrix object.
     *
     * @param   rowPins
     *          A list of pin numbers connected to the rows of the button
     *          matrix.  
     *          **⚠** These pins will be driven LOW as outputs (Lo-Z).
     * @param   colPins
     *          A list of pin numbers connected to the columns of the button
     *          matrix.  
     *          These pins will be used as inputs (Hi-Z), and the internal 
     *          pull-up resistor will be enabled.
     */
    ButtonMatrix(const PinList<NumRows> &rowPins,
                 const PinList<NumCols> &colPins);

    /**
     * @brief   Initialize (enable internal pull-up resistors on column pins).
     */
    void begin();

    /**
     * @brief   Scan the matrix, read all button states, and call the
     *          onButtonChanged callback.
     */
    void update();

    /** 
     * Get the state of the button in the given column and row.
     * 
     * @note    No bounds checking is performed.
     */
    bool getPrevState(uint8_t col, uint8_t row);

    /// Configure the debounce time interval. Only one button can change in each
    /// debounce interval. Time in milliseconds.
    void setDebounceTime(unsigned long debounceTime) {
        this->debounceTime = debounceTime;
    }
    /// Get the debounce time.
    unsigned long getDebounceTime() const { return debounceTime; }

  protected:
    /**
     * @brief   The callback function that is called whenever a button changes
     *          state. Implement this in the derived class.
     * 
     * @param   row
     *          The row of the button that changed state.
     * @param   col
     *          The column of the button that changed state.
     * @param   state
     *          The new state of the button.
     */
    void onButtonChanged(uint8_t row, uint8_t col, bool state) = delete;

  private:
    static inline uint8_t positionToBits(uint8_t col, uint8_t row);
    static inline uint8_t bitsToIndex(uint8_t bits);
    static inline uint8_t bitsToBitmask(uint8_t bits);
    void setPrevState(uint8_t col, uint8_t row, bool state);

    unsigned long debounceTime = BUTTON_DEBOUNCE_TIME;
    unsigned long prevRefresh = 0;
    uint8_t prevStates[(NumCols * NumRows + 7) / 8];

    const PinList<NumRows> rowPins;
    const PinList<NumCols> colPins;
};

END_AH_NAMESPACE

#include "ButtonMatrix.ipp" // Template implementations

AH_DIAGNOSTIC_POP()
