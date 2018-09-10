#pragma once

#include <MIDI_Interfaces/MIDI_Interface.h>

#include <Display/DisplayElement.hpp>
#include <Display/DisplayInterface.hpp>
#include <Helpers/MillisTimer.hpp>

/** 
 * @brief   This class ensures initialization, updating, and interaction between
 *          all other classes, it's the glue that holds everything together.
 */
class Control_Surface_ : public MIDI_Callbacks {
  public:
    /**
     * @brief   Return the static Control_Surface_ instance.
     *          (Control_Surface_ is a singleton.)
     */
    static Control_Surface_ &getInstance();
    Control_Surface_(Control_Surface_ const &) = delete;
    void operator=(Control_Surface_ const &) = delete;

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
     * @todo    This violate's the Law of Demeter
     */
    MIDI_Interface &MIDI();

    void updateMidiInput();

    void updateInputs();

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

    Timer<micros> potentiometerTimer = {FILTERED_INPUT_UPDATE_INTERVAL};
    Timer<micros> displayTimer = {1000000UL / MAX_FPS};
};

extern Control_Surface_ &Control_Surface;