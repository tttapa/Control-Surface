# Purpose of the Control_Surface singleton {#control-surface-purpose}

When writing code for a MIDI Control Surface, you'll likely come across the
@ref Control_Surface singleton. The program structure usually looks something
like this:

```cpp
#include <Control_Surface.h>

// Instantiate MIDI interfaces, ExtIO elements, MIDI elements, etc.

void setup() {
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
```

The Control_Surface singleton is responsible for the following important tasks:

 - **Initializing** all MIDI interfaces, ExtIO elements, MIDI elements, selectors,
   display elements, etc. This is done by
   @ref Control_Surface_::begin "Control_Surface.begin()".
 - Periodically **updating** all elements, so they can poll digital or analog
   input pins etc. This happens when you call
   @ref Control_Surface_::loop "Control_Surface.loop()", so it is important
   to call this function frequently and without blocking for a long time.
 - **Routing** all MIDI messages sent by @ref MIDIOutputElements to the right
   MIDI interface. The default interface can be set using
   @ref MIDI_Interface::setAsDefault() function or using manual routing as
   explained in the @ref midi_md-routing section of the @ref midi-tutorial.
 - **Dispatching** incoming MIDI messages to the appropriate
   @ref MIDIInputElements.
 - **Refreshing** all displays and @ref DisplayElements.

Control Surface keeps track of elements that inherit from the
@ref AH::UpdatableCRTP base class. Specifically, it keeps a list of all
@ref AH::ExtendedIOElement%s, @ref MIDI_Interface%s, @ref MIDIOutputElements,
@ref MIDIInputElements and @ref DisplayElements.
If you have certain elements that should not
be initialized or updated by Control Surface, you can call
@ref AH::UpdatableCRTP::disable() before calling `Control_Surface.begin()`. 
It is then your responsibility to call the @ref AH::Updatable::begin()
and @ref AH::Updatable::update() functions whenever appropriate.
You can also enable these disabled elements again using
@ref AH::UpdatableCRTP::enable().
