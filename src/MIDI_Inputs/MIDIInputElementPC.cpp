#include "MIDIInputElementPC.hpp"

DoublyLinkedList<MIDIInputElementPC> MIDIInputElementPC::elements;
#ifdef ESP32
std::mutex MIDIInputElementPC::mutex;
#endif