#ifndef Control_Surface_CLASS_H
#define Control_Surface_CLASS_H

#include "../MIDI_Interfaces/USBMIDI_Interface.h"

#ifdef DISPLAY_GFX
#include "../Display/Display.hpp"
#include "../Display/DisplayElement.hpp"
#endif

class Control_Surface_
{
public:
  static Control_Surface_ &getInstance();
  Control_Surface_(Control_Surface_ const &) = delete;
  void operator=(Control_Surface_ const &) = delete;
  ~Control_Surface_();

  void begin();
  MIDI_Interface *MIDI();

private:
  Control_Surface_() {}
  MIDI_Interface *new_midi = nullptr;
  MIDI_Interface *midi = nullptr;
  void updateControls();
  void updateSelectors();
  void updateMidiInput();
  void updateInputs();
#ifdef DISPLAY_GFX
  // void updateDisplays(); // (see below)
#endif

public:
  void update()
  {
    if (midi == nullptr)
      begin(); // make sure that midi != nullptr

    updateControls();  // update all control elements
    updateSelectors(); // update all bank selectors

    updateMidiInput();
    updateInputs();
#ifdef DISPLAY_GFX
    updateDisplays();
#endif
  }

private:
#ifdef DISPLAY_GFX
  void updateDisplays()
  {
    static unsigned long previousRefresh = millis();

    if (millis() - previousRefresh < 1000 / MAX_FPS)
      return;
    previousRefresh += 1000 / MAX_FPS;

    for (DisplayInterface *display = DisplayInterface::getFirst(); display != nullptr; display = display->getNext())
      display->clearDisplay();
    
    for (DisplayElement *de = DisplayElement::getFirst(); de != nullptr; de = de->getNext())
      de->draw();
    
    for (DisplayInterface *display = DisplayInterface::getFirst(); display != nullptr; display = display->getNext())
      display->display();
      
  }
#endif
};

extern Control_Surface_ &Control_Surface;

#endif // Control_Surface_CLASS_H