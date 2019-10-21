#include "MIDIInputElementSysEx.hpp"

BEGIN_CS_NAMESPACE
DoublyLinkedList<MIDIInputElementSysEx> MIDIInputElementSysEx::elements;
#ifdef ESP32
std::mutex MIDIInputElementSysEx::mutex;
#endif
END_CS_NAMESPACE