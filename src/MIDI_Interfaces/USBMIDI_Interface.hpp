#pragma once

#include "MIDI_Interface.hpp"
#include "USBMIDI/USBMIDI.hpp"
#include "USBMIDI_Sender.hpp"
#include <AH/Error/Error.hpp>
#include <AH/Teensy/TeensyUSBTypes.hpp>
#include <MIDI_Parsers/USBMIDI_Parser.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending MIDI messages over a USB MIDI
 *          connection.
 */
template <class Backend>
class GenericUSBMIDI_Interface : public MIDI_Interface {
  public:
    /**
     * @brief   Construct a new GenericUSBMIDI_Interface.
     */
    template <class... Args>
    GenericUSBMIDI_Interface(Args &&...args)
        : backend {std::forward<Args>(args)...},
          alwaysSendImmediately_(backend.preferImmediateSend()) {}

    GenericUSBMIDI_Interface(const GenericUSBMIDI_Interface &) = delete;
    GenericUSBMIDI_Interface(GenericUSBMIDI_Interface &&) = delete;
    GenericUSBMIDI_Interface &
    operator=(const GenericUSBMIDI_Interface &) = delete;
    GenericUSBMIDI_Interface &operator=(GenericUSBMIDI_Interface &&) = delete;

  private:
    // MIDI send implementations
    void sendChannelMessageImpl(ChannelMessage) override;
    void sendSysCommonImpl(SysCommonMessage) override;
    void sendSysExImpl(SysExMessage) override;
    void sendRealTimeImpl(RealTimeMessage) override;
    void sendNowImpl() override { backend.sendNow(); }

  private:
#if !DISABLE_PIPES
    void handleStall() override { MIDI_Interface::handleStall(this); }
#ifdef DEBUG_OUT
    const char *getName() const override { return "usb"; }
#endif
#endif

  public:
    /// @name   Initialization and polling
    /// @{

    /// Initialize.
    void begin() override;
    /// Poll the backend (if necessary) and invoke the callbacks for any
    /// received MIDI messages, as well as sending them over the pipes connected
    /// to this interface.
    void update() override;

    /// @}

  public:
    /// @name   Reading incoming MIDI messages
    /// @{

    /// Try reading and parsing a single incoming MIDI message.
    /// @return Returns the type of the message read, or
    ///         `MIDIReadEvent::NO_MESSAGE` if no MIDI message was available.
    MIDIReadEvent read();

    /// Return the received channel voice message.
    ChannelMessage getChannelMessage() const;
    /// Return the received system common message.
    SysCommonMessage getSysCommonMessage() const;
    /// Return the received real-time message.
    RealTimeMessage getRealTimeMessage() const;
    /// Return the received system exclusive message.
    SysExMessage getSysExMessage() const;

    /// @}

  public:
    /// @name Underlying USB communication
    /// @{

    /// The (platform-specific) backend used for MIDI over USB communication.
    Backend backend;

  private:
    /// Functor to send USB MIDI packets.
    struct Sender {
        GenericUSBMIDI_Interface *iface;
        void operator()(Cable cn, MIDICodeIndexNumber cin, uint8_t d0,
                        uint8_t d1, uint8_t d2) {
            uint8_t cn_cin = (cn.getRaw() << 4) | uint8_t(cin);
            iface->backend.write({cn_cin, d0, d1, d2});
        }
    };
    /// @}

  private:
    /// Parses USB packets into MIDI messages.
    USBMIDI_Parser parser;
    /// Sends USB MIDI messages.
    USBMIDI_Sender sender;
    /// @see neverSendImmediately()
    bool alwaysSendImmediately_ = true;

  public:
    /// @name   Buffering USB packets
    /// @{

    /// Check if this USB interface always sends its USB packets immediately
    /// after sending a MIDI message. The default value depends on the MIDI USB
    /// backend being used: `true` for the `MIDIUSB` library, and `false` for
    /// the Teensy Core USB MIDI functions (because they have a short timeout).
    bool alwaysSendsImmediately() const { return alwaysSendImmediately_; }
    /// Don't send the USB packets immediately after sending a MIDI message.
    /// By disabling immediate transmission, packets are buffered until you
    /// call @ref sendNow() or until a timeout is reached, so multiple MIDI
    /// messages can be transmitted in a single USB packet. This is more
    /// efficient and results in a higher maximum bandwidth, but it could
    /// increase latency when used incorrectly.
    void neverSendImmediately() { alwaysSendImmediately_ = false; }
    /// Send the USB packets immediately after sending a MIDI message.
    /// @see @ref neverSendImmediately()
    void alwaysSendImmediately() { alwaysSendImmediately_ = true; }

    /// @}
};

END_CS_NAMESPACE

#include "USBMIDI_Interface.ipp"

#if defined(TEENSYDUINO) && !defined(TEENSY_MIDIUSB_ENABLED)
#pragma message(                                                               \
    "Teensy: USB MIDI not enabled. Enable it from the Tools > USB Type menu.")
#define CS_USB_MIDI_DISABLED
#endif

// If MIDI over USB is supported
#if (!defined(CS_USB_MIDI_NOT_SUPPORTED) && !defined(CS_USB_MIDI_DISABLED)) || \
    !defined(ARDUINO)

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending MIDI messages over a USB MIDI
 *          connection.
 * 
 * On boards that support it, this will create a native MIDI over USB interface
 * using the platform-specific libraries (e.g. MIDIUSB for Arduino Leonardo, or 
 * the Core usbMIDI library for Teensy).  
 * On boards without native USB support, it'll fall back to a serial MIDI 
 * interface at the default @ref MIDI_BAUD "MIDI baud rate" on the UART 
 * connected to the Serial to USB chip. This can be used with custom 
 * MIDI over USB firmware for the Serial to USB chip.
 * 
 * @note    See @ref md_pages_MIDI-over-USB for more information.
 * 
 * @ingroup MIDIInterfaces
 */
class USBMIDI_Interface
    : public GenericUSBMIDI_Interface<USBDeviceMIDIBackend> {
  public:
    using backend_t = USBDeviceMIDIBackend;
    using GenericUSBMIDI_Interface<backend_t>::GenericUSBMIDI_Interface;
    using MIDIUSBPacket_t = backend_t::MIDIUSBPacket_t;
};

END_CS_NAMESPACE

// If the main MCU doesn't have a USB connection:
// Fall back on Serial connection at the hardware MIDI baud rate.
// (Can be used with HIDUINO or USBMidiKliK.)
#elif !defined(CS_USB_MIDI_DISABLED)

#include "SerialMIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for MIDI interfaces sending MIDI messages over a USB MIDI
 *          connection.
 * 
 * @note    See @ref md_pages_MIDI-over-USB for more information.
 * 
 * @ingroup MIDIInterfaces
 */
class USBMIDI_Interface : public USBSerialMIDI_Interface {
  public:
    /**
     * @brief   Construct a new USBMIDI_Interface.
     */
    USBMIDI_Interface() : USBSerialMIDI_Interface(MIDI_BAUD) {}
};

END_CS_NAMESPACE

#endif
