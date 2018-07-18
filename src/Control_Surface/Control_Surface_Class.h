#pragma once

#include "../MIDI_Interfaces/USBMIDI_Interface.h"

#ifdef DISPLAY_GFX
#include "../Display/Display.hpp"
#include "../Display/DisplayElement.hpp"
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
    ~Control_Surface_();

    /**
     * @brief Initialize the Control_Surface object.
     */
    void begin();
    /**
     * @brief Update all MIDI elements, send MIDI events and read MIDI input.
     */
    void loop();
    /**
     * @brief   Get the MIDI interface of the Control Surface.
     * 
     * @return  A reference to the Control Surface's MIDI interface.
     */
    MIDI_Interface &MIDI();
    /**
     * @brief   Set the given MIDI interface as the Control Surface's interface.
     * 
     * @param   midi
     *          The MIDI interface to use for input and output of the Control
     *          surface.
     */
    void setMIDI_Interface(MIDI_Interface &midi);

  private:
    /**
     * @brief   Control_Surface_ is a singleton, so the constructor is private.
     */
    Control_Surface_() {}
    MIDI_Interface *new_midi = nullptr;
    MIDI_Interface *midi = nullptr;
    /**
     * @brief   Update all MIDI_Control_Element%s.
     */
    void updateControls();
    /**
     * @brief   Update all Selector%s (BankSelector%s, Transposer%s, 
     *          ProgramSelector%s).
     */
    void updateSelectors();
    void updateMidiInput();
    void updateInputs();

  private:
    void updateDisplays() {
#ifdef DISPLAY_GFX
        static unsigned long previousRefresh = millis();

        if (millis() - previousRefresh < 1000 / MAX_FPS)
            return;
        previousRefresh += 1000 / MAX_FPS;

        for (DisplayInterface *display = DisplayInterface::getFirst();
             display != nullptr; display = display->getNext())
            display->clearDisplay();

        for (DisplayElement *de = DisplayElement::getFirst(); de != nullptr;
             de = de->getNext())
            de->draw();

        for (DisplayInterface *display = DisplayInterface::getFirst();
             display != nullptr; display = display->getNext())
            display->display();
#endif
    }
};

extern Control_Surface_ &Control_Surface;