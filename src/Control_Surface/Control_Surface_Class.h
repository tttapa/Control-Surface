#pragma once

#include "../MIDI_Interfaces/USBMIDI_Interface.h"

#ifdef DISPLAY_GFX
#include "../Display/Display.hpp"
#include "../Display/DisplayElement.hpp"
#endif

class Control_Surface_ {
  public:
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
    void update();
    MIDI_Interface *MIDI();

  private:
    Control_Surface_() {}
    MIDI_Interface *new_midi = nullptr;
    MIDI_Interface *midi = nullptr;
    void updateControls();
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