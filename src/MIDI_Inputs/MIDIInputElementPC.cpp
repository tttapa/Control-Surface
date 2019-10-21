#include "MIDIInputElementPC.hpp"

BEGIN_CS_NAMESPACE

DoublyLinkedList<MIDIInputElementPC> MIDIInputElementPC::elements;
#ifdef ESP32
std::mutex MIDIInputElementPC::mutex;
#endif

END_CS_NAMESPACE