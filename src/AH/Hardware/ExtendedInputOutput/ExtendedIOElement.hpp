/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/Hardware-Types.hpp>
#include <AH/Containers/LinkedList.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   An abstract base class for Extended Input/Output elements.
 *
 * The limited number of IO pins of the Arduino can be extended by
 * adding multiplexers, shift registers, IO expanders, etc.  
 * ExtendedIOElement serves as a base class for all these expanders.
 *
 * The pins of each extended IO element are mapped to a pin number
 * greater than the greatest Arduino pin number.  
 * You can supply this pin number to the IO functions in the ExtIO
 * namespace.  
 * If the pin number corresponds to an actual Arduino pin,
 * the default Arduino IO function (digitalRead, digitalWrite, ...)
 * will be called.  
 * If the pin is not an Arduino pin, it is an extended IO pin number,
 * so the extended IO element that this pin belongs to is looked up,
 * and the IO function for this element is executed with the correct
 * pin number.
 *
 * For example:
 * Imagine an Arduino with 20 pins (e.g. the Arduino UNO).
 * Pins 0 - 19 will correspond to the Arduino pins, and
 * `ExtIO::digitalRead(pin)` will have the exact same effect as
 * the standard `digitalRead(pin)` function (albeit a little slower).  
 * Now, we'll add two 8-channel analog multiplexers, let's call them
 * `mux1` and `mux2`.  
 * The first pin (pin 0) of `mux1` will be extended IO pin number 20,
 * the last pin (pin 7) of `mux1` will be extended IO pin number 27,
 * etc.
 * The first pin of `mux2` will be extended IO pin number 28, you get
 * the idea.
 * If you now call `ExtIO::digitalRead(mux1.#pin (7))` or
 * `ExtIO::digitalRead(27)`, both will be
 * translated to `mux1.digitalRead(7)`.
 *
 * The number of extended IO elements is limited only by the size of
 * `pin_t`. However, looking up the extended IO element for a given
 * extended IO pin number uses linear search, so that might add
 * some noticable overhead for large pin numbers.  
 * 
 * The design here is a compromise: saving a pointer to each extended IO element
 * to find it directly would be much faster than having to search all elements
 * each time. On the other hand, it would require each `pin_t` variable to be
 * at least one byte larger. Since almost all other classes in this library 
 * store pin variables, the memory penalty would be too large, especially on AVR
 * microcontrollers.  
 * Another reason to do it this way, is that this approach is still fast enough
 * to make sure it is not noticable to human users.
 */
class ExtendedIOElement : public DoublyLinkable<ExtendedIOElement> {
  protected:
    /**
     * @brief   Create an ExtendedIOElement with the given number of pins.
     * 
     * @param   length
     *          The number of pins this element has.
     */
    ExtendedIOElement(pin_t length);

  public:
    virtual ~ExtendedIOElement();

    /** 
     * @brief   Set the mode of a given pin.
     * 
     * @note    This function might not be implemented by all subclasses.  
     *          Some extended IO types, such as shift registers, can only be 
     *          used as outputs.  
     *          On others, it might be implemented, but it could impact all pins
     *          of the IO element. For example, enabling the internal pull-up 
     *          resistor on an analog multiplexer affects all pins of the mux.
     * 
     * @param   pin
     *          The (zero-based) pin of this IO element.
     * @param   mode
     *          The mode to set the pin to (e.g. `INPUT`, `OUTPUT` or 
     *          `INPUT_PULLUP`).
     */
    virtual void pinMode(pin_t pin, uint8_t mode) = 0;

    /** 
     * @brief   Set the output of the given pin to the given state.
     * 
     * @param   pin
     *          The (zero-based) pin of this IO element.
     * @param   state
     *          The new state to set the pin to.
     */
    virtual void digitalWrite(pin_t pin, uint8_t state) = 0;

    /** 
     * @brief   Read the state of the given pin.
     * 
     * @param   pin
     *          The (zero-based) pin of this IO element.
     * @return  The state of the given pin.
     */
    virtual int digitalRead(pin_t pin) = 0;

    /**
     * @brief   Write an analog (or PWM) value to the given pin.
     * 
     * @param   pin 
     *          The (zero-based) pin of this IO element.
     * @param   val 
     *          The new analog value to set the pin to.
     */
    virtual void analogWrite(pin_t pin, analog_t val) = 0;

    /**
     * @brief   Read the analog value of the given pin.
     * 
     * @param   pin 
     *          The (zero-based) pin of this IO element.
     * @return  The new analog value of pin.
     */
    virtual analog_t analogRead(pin_t pin) = 0;

    /**
     * @brief   Initialize the extended IO element.
     */
    virtual void begin() = 0;

    /**
     * @brief   Initialize all extended IO elements.
     */
    static void beginAll();

    /**
     * @brief   Update the extended IO element:
     *          write the internal state to the physical outputs,
     *          or read the physical state into the input buffers.
     */
    virtual void update() = 0;

    /**
     * @brief   Get the extended IO pin number of a given physical pin of this
     *          extended IO element.
     * @param   pin
     *          The zero-based physical pin number of this IO element.
     * @return  The global, unique extended IO pin number for the given pin.
     */
    pin_t pin(pin_t pin) const;

    /**
     * @brief   Get the extended IO pin number of a given physical pin of this
     *          extended IO element.  
     *          It is alias for `ExtendedIOElement::pin`.
     * @param   pin
     *          The zero-based physical pin number of this IO element.
     * @return  The global, unique extended IO pin number for the given pin.
     */
    pin_t operator[](pin_t pin) const;

    /**
     * @brief Get the number of pins this IO element has.
     * 
     * @return The number of pins this IO element has. 
     */
    pin_t getLength() const;

    /**
     * @brief   Get the largest global extended IO pin number that belongs to
     *          this extended IO element.
     */
    pin_t getEnd() const;

    /**
     * @brief   Get the smallest global extended IO pin number that belongs to
     *          this extended IO element.
     */
    pin_t getStart() const;

    /**
     * @brief   Get the list of all Extended IO elements.
     */
    static DoublyLinkedList<ExtendedIOElement> &getAll();

  private:
    const pin_t length;
    const pin_t start;
    const pin_t end;
    static pin_t offset;

    static DoublyLinkedList<ExtendedIOElement> elements;
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
