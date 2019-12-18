#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

namespace USBMIDI {

using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
MIDIUSBPacket_t read();
void write(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1, uint8_t d2);
void flush();

} // namespace USBMIDI

END_CS_NAMESPACE