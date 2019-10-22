#include "MIDIInputElementChannelPressure.hpp"

BEGIN_CS_NAMESPACE

DoublyLinkedList<MIDIInputElementChannelPressure>
    MIDIInputElementChannelPressure::elements;
#ifdef ESP32
std::mutex MIDIInputElementChannelPressure::mutex;
#endif

END_CS_NAMESPACE