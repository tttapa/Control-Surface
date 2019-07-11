#include "MIDIInputElementNote.hpp"

DoublyLinkedList<MIDIInputElementNote> MIDIInputElementNote::elements;
#ifdef ESP32
std::mutex MIDIInputElementNote::mutex;
#endif