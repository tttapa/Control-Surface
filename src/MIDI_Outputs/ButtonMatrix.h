#ifndef BUTTONMATRIX_H_
#define BUTTONMATRIX_H_

#include "./MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h" // for pin_t

/**
 * @brief A MIDI_Control_Element that reads the states of a button matrix and sends it over MIDI as a series of Note events.
 * 
 * @tparam  nb_rows 
 *          The number of rows in the button matrix.
 * @tparam  nb_cols 
 *          The number of columns in the button matrix.
 */
template <size_t nb_rows, size_t nb_cols>
class ButtonMatrix : public MIDI_Control_Element
{
  public:
    /**
     * @brief Construct a new ButtonMatrix object.
     * 
     * @param rowPins
     *        A list of pin numbers connected to the rows of the button matrix. These pins will be driven LOW (Lo-Z).
     * @param colPins
     *        A list of pin numbers connected to the columns of the button matrix. These pins will be used as inputs, and the internal pull-up resistor will be enabled.
     * @param addresses
     *        A 2D array of dimensions (nb_rows × nb_cols) containing the address for each button.
     * @param channel 
     *        The MIDI channel [1, 16].
     * @param velocity 
     *        The velocity of the MIDI Note events [1, 127].
     * 
     * @note  The lists of pins are copied.  
     *        The list of addresses is not copied. It should be a reference to a 2-dimensional array.  
     *        This means that initializing \c addresses with a brace-enclosed initializer list is not allowed.
     */
    ButtonMatrix(const pin_t (&rowPins)[nb_rows], const pin_t (&colPins)[nb_cols], const uint8_t (&addresses)[nb_rows][nb_cols], uint8_t channel, uint8_t velocity = 0x7F);
    ~ButtonMatrix();

  protected:
    void update() override;

  private:
    void init();

    inline uint8_t positionToBits(uint8_t col, uint8_t row);
    inline uint8_t bitsToIndex(uint8_t bits);
    inline uint8_t bitsToBitmask(uint8_t bits);
    bool getPrevState(uint8_t col, uint8_t row);
    void setPrevState(uint8_t col, uint8_t row, bool state);
    bool allReleased();

    void setChannelOffset(uint8_t offset);
    void setAddressOffset(uint8_t offset);

    unsigned long prevRefresh = 0;
    uint8_t *prevStates = nullptr;

    pin_t rowPins[nb_rows], colPins[nb_cols];

    const uint8_t (&addresses)[nb_rows][nb_cols];
    const uint8_t channel, velocity;

    uint8_t newAddressOffset = addressOffset;
    uint8_t newChannelOffset = channelOffset;
};

#include "./ButtonMatrix.cpp" // Because it's a template class

#endif // BUTTONMATRIX_H_