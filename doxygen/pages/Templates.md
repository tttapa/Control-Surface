### MIDI output

```cpp
#include "../Settings/SettingsWrapper.h"
#include "./MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

class Element : public MIDI_Control_Element
{
public:
  Element(...);  // Constructor
  void map(int (*fn)(int));  // Change the function pointer for analogMap to a new function.
  void invert();  // Invert

private:
  void refresh();  // Read the input value, check if it changed since last time, if so, send over MIDI
  
  uint8_t controllerNumber, channel;
  int (*mapFn)(int) = identity; // function pointer to identity function f(x) → x

  static int identity(int x)  // identity function f(x) → x
  {
    return x;
  };
}

```