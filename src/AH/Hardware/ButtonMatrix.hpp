/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/Hardware-Types.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   A class that reads the states of a button matrix.
 *
 * @tparam  nb_rows
 *          The number of rows in the button matrix.
 * @tparam  nb_cols
 *          The number of columns in the button matrix.
 * 
 * @ingroup AH_HardwareUtils
 */
template <uint8_t nb_rows, uint8_t nb_cols>
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
    ButtonMatrix(const PinList<nb_rows> &rowPins,
                 const PinList<nb_cols> &colPins);
    /**
     * @brief   Destructor.
     */
    virtual ~ButtonMatrix() = default;

    /**
     * @brief   Initialize (enable internal pull-up resistors on column pins).
     */
    void begin();

    /**
     * @brief   Scan the matrix, read all button states, and call the
     *          onButtonChanged callback.
     */
    void update();

  private:
    /**
     * @brief   The callback function that is called whenever a button changes
     *          state.
     * 
     * @param   row
     *          The row of the button that changed state.
     * @param   col
     *          The column of the button that changed state.
     * @param   state
     *          The new state of the button.
     */
    virtual void onButtonChanged(uint8_t row, uint8_t col, bool state) = 0;

    static inline uint8_t positionToBits(uint8_t col, uint8_t row);
    static inline uint8_t bitsToIndex(uint8_t bits);
    static inline uint8_t bitsToBitmask(uint8_t bits);
    bool getPrevState(uint8_t col, uint8_t row);
    void setPrevState(uint8_t col, uint8_t row, bool state);

    unsigned long prevRefresh = 0;
    uint8_t prevStates[(nb_cols * nb_rows + 7) / 8];

    const PinList<nb_rows> rowPins;
    const PinList<nb_cols> colPins;
};

END_AH_NAMESPACE

#include "ButtonMatrix.ipp" // Template implementations

AH_DIAGNOSTIC_POP()
