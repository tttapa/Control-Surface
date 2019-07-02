#include "MIDIInputElementChannelPressure.hpp"

DoublyLinkedList<MIDIInputElementChannelPressure>
    MIDIInputElementChannelPressure::elements;
#ifdef ESP32
std::mutex MIDIInputElementChannelPressure::mutex;
#endif