#include "MIDIInputElementCC.hpp"

BEGIN_CS_NAMESPACE

DoublyLinkedList<MIDIInputElementCC> MIDIInputElementCC::elements;
#ifdef ESP32
std::mutex MIDIInputElementCC::mutex;
#endif

END_CS_NAMESPACE