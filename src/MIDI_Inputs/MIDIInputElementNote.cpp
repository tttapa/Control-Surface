#include "MIDIInputElementNote.hpp"

BEGIN_CS_NAMESPACE

DoublyLinkedList<MIDIInputElementNote> MIDIInputElementNote::elements;
#ifdef ESP32
std::mutex MIDIInputElementNote::mutex;
#endif

END_CS_NAMESPACE