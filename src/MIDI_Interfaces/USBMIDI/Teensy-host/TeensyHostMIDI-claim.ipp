// The following function was reused from the USBHost_t36 library
// https://github.com/PaulStoffregen/USBHost_t36/blob/d864c8cc33d8dbcd9501e354e777d90f6ac88037/midi.cpp#L62-L220

/* USB EHCI Host for Teensy 3.6
 * Copyright 2017 Paul Stoffregen (paul@pjrc.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// Audio Class-Specific Descriptor Types (audio 1.0, page 99)
//   CS_UNDEFINED     0x20
//   CS_DEVICE        0x21
//   CS_CONFIGURATION 0x22
//   CS_STRING        0x23
//   CS_INTERFACE     0x24
//   CS_ENDPOINT      0x25
// MS Class-Specific Interface Descriptor Subtypes (midi 1.0, page 36)
//   MS_DESCRIPTOR_UNDEFINED 0x00
//   MS_HEADER               0x01
//   MIDI_IN_JACK            0x02
//   MIDI_OUT_JACK           0x03
//   ELEMENT                 0x04
// MS Class-Specific Endpoint Descriptor Subtypes (midi 1.0, page 36)
//   DESCRIPTOR_UNDEFINED 0x00
//   MS_GENERAL           0x01
// MS MIDI IN and OUT Jack types (midi 1.0, page 36)
//   JACK_TYPE_UNDEFINED 0x00
//   EMBEDDED            0x01
//   EXTERNAL            0x02
// Endpoint Control Selectors (midi 1.0, page 36)
//   EP_CONTROL_UNDEFINED 0x00
//   ASSOCIATION_CONTROL  0x01

BEGIN_CS_NAMESPACE

template <uint16_t MaxPacketSize>
bool TeensyHostMIDI<MaxPacketSize>::claim_if_midi(Device_t *dev, int type,
                                                  const uint8_t *descriptors,
                                                  uint32_t len) {
    static_cast<void>(dev);
    static_cast<void>(type);
    USBHost::println_("MIDIDevice claim this=", (uintptr_t)this, HEX);
    USBHost::println_("len = ", len);

    const uint8_t *p = descriptors;
    const uint8_t *end = p + len;

    if (p[0] != 9 || p[1] != 4)
        return false; // interface descriptor
    // USBHost::println_("  bInterfaceClass=", p[5]);
    // USBHost::println_("  bInterfaceSubClass=", p[6]);
    bool ismidi = false;
    if (p[5] == 1 && p[6] == 3) {
        USBHost::println_(
            "  Interface is MIDI"); // p[5] is bInterfaceClass: 1 = Audio class
        ismidi = true;              // p[6] is bInterfaceSubClass: 3 = MIDI
    } else {
        if (p[5] >= 2 && p[5] <= 18)
            return false; // definitely not MIDI
        // Yamaha uses vendor specific class, but can be
        // identified as MIDI from CS_INTERFACE descriptors.
        //  https://forum.pjrc.com/threads/55142?p=199162&viewfull=1#post199162
        USBHost::println_("  Interface is unknown (might be Yahama)");
    }
    p += 9;
    rx_ep = 0;
    tx_ep = 0;

    while (p < end) {
        len = *p;
        if (len < 4)
            return false; // all audio desc are at least 4 bytes
        if (p + len > end)
            return false; // reject if beyond end of data
        uint32_t type = p[1];
        USBHost::print_("type: ", type);
        USBHost::println_(", len: ", len);
        if (type == 4 || type == 11)
            break;          // interface or IAD, not for us
        if (type == 0x24) { // 0x24 = Audio CS_INTERFACE, audio 1.0, page 99
            uint32_t subtype = p[2];
            // USBHost::println_("subtype: ", subtype);
            if (subtype == 1) {
                // Interface Header, midi 1.0, page 21
                USBHost::println_("    MIDI Header (ignored)");
                ismidi = true;
            } else if (subtype == 2) {
                // MIDI IN Jack, midi 1.0, page 22
                USBHost::println_("    MIDI IN Jack (ignored)");
                ismidi = true;
            } else if (subtype == 3) {
                // MIDI OUT Jack, midi 1.0, page 22
                USBHost::println_("    MIDI OUT Jack (ignored)");
                ismidi = true;
            } else if (subtype == 4) {
                // Element Descriptor, midi 1.0, page 23-24
                USBHost::println_("    MIDI Element (ignored)");
                ismidi = true;
            } else if (subtype == 0xF1 && p[3] == 2) {
                // see Linux sound/usb/quirks.c create_roland_midi_quirk()
                USBHost::println_("    Roland vendor-specific (ignored)");
                ismidi = true;
            } else {
                USBHost::println_("    Unknown MIDI CS_INTERFACE descriptor!");
                return false; // unknown
            }
        } else if (type == 5) {
            // endpoint descriptor
            if (p[0] < 7)
                return false; // at least 7 bytes
            if (p[3] != 2 && p[3] != 3)
                return false; // must be bulk or interrupt type
            USBHost::println_("    MIDI Endpoint: ", p[2], HEX);
            switch (p[2] & 0xF0) {
                case 0x80:
                    // IN endpoint
                    if (rx_ep == 0) {
                        rx_ep = p[2] & 0x0F;
                        rx_ep_type = p[3];
                        rx_size = p[4] | (p[5] << 8);
                        USBHost::println_("      rx_size = ", rx_size);
                    }
                    break;
                case 0x00:
                    // OUT endpoint
                    if (tx_ep == 0) {
                        tx_ep = p[2];
                        tx_ep_type = p[3];
                        tx_size = p[4] | (p[5] << 8);
                        USBHost::println_("      tx_size = ", tx_size);
                    }
                    break;
                default: return false;
            }
        } else if (type == 37) {
            // MIDI endpoint info, midi 1.0: 6.2.2, page 26
            USBHost::println_("    MIDI Endpoint Jack Association (ignored)");
        } else {
            USBHost::println_("    Unknown descriptor, type=", type);
            return false; // unknown
        }
        p += len;
    }

    return ismidi;
}

END_CS_NAMESPACE
