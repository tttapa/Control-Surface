#pragma once

#include <MIDI_Interfaces/USBMIDI_Interface.h>

#ifdef DISPLAY_GFX
#include <Display/Display.hpp>
#include <Display/DisplayElement.hpp>
#endif

class Control_Surface_ {
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
     */
    MIDI_Interface &MIDI();

  private:
    /**
     * @brief   Control_Surface_ is a singleton, so the constructor is private.
     */
    Control_Surface_() = default;

    /**
     * @brief   Update all Selector%s (BankSelector%s, Transposer%s,
     *          ProgramSelector%s).
     */
    void updateSelectors();

    void updateMidiInput();

    void updateInputs();
};

void updateDisplays();

extern Control_Surface_ &Control_Surface;