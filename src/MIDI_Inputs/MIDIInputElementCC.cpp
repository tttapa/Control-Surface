#include "MIDIInputElementCC.hpp"

DoublyLinkedList<MIDIInputElementCC> MIDIInputElementCC::elements;
#ifdef ESP32
std::mutex MIDIInputElementCC::mutex;
#endif