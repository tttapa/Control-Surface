/* ✔ */

#pragma once

#include <MIDI_Interfaces/MIDI_Interface.hpp>

#include <Display/DisplayElement.hpp>
#include <Display/DisplayInterface.hpp>
#include <Helpers/MillisTimer.hpp>

/** 
 * @brief   This class ensures initialization, updating, and interaction between
 *          all other classes, it's the glue that holds everything together.
 */
class Control_Surface_ : public MIDI_Callbacks {
  public:
    // Copying is not allowed
    Control_Surface_(Control_Surface_ const &) = delete;
    void operator=(Control_Surface_ const &) = delete;

    /**
     * @brief   Return the static Control_Surface_ instance.
     *          (Control_Surface_ is a singleton.)
     */
    static Control_Surface_ &getInstance();

    /**
     * @brief   Initialize the Control_Surface.
     */
    void begin();

    /**
     * @brief   Update all MIDI elements, send MIDI events and read MIDI input.
     */
    void loop();

    /**
     * @brief   Get the MIDI interface of the Control Surface.
     *
     * @return  A reference to the Control Surface's MIDI interface.
     * 
     * @todo    This violate's the Law of Demeter.
     */
    MIDI_Interface &MIDI();

    /** 
     * @brief   Update all MIDI interfaces to receive new MIDI events.
     */
    void updateMidiInput();

    /**
     * @brief   Update all MIDIInputElement#s.
     */
    void updateInputs();

    /** 
     * @brief   Clear, draw and display all displays.
     */
    void updateDisplays();

  private:
    /**
     * @brief   Control_Surface_ is a singleton, so the constructor is private.
     */
    Control_Surface_() = default;

    /** 
     * @brief   The callback to be called when a MIDI channel message is
     *          received.
     */
    void onChannelMessage(MIDI_Interface &midi) override;

    /** 
     * @brief   The callback to be called when a MIDI System Exclusive message
     *          is received.
     */
    void onSysExMessage(MIDI_Interface &midi) override;

    /// A timer to know when to update the analog inputs.
    Timer<micros> potentiometerTimer = {FILTERED_INPUT_UPDATE_INTERVAL};
    /// A timer to know when to refresh the displays.
    Timer<micros> displayTimer = {1000000UL / MAX_FPS};
};

/// A predefined instance of the Control Surface to use in the Arduino sketches.
extern Control_Surface_ &Control_Surface;