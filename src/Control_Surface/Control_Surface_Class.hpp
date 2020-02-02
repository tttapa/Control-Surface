/* ✔ */

#pragma once

#include <AH/Containers/Updatable.hpp>
#include <AH/Hardware/FilteredAnalog.hpp>
#include <AH/Timing/MillisMicrosTimer.hpp>
#include <Display/DisplayElement.hpp>
#include <Display/DisplayInterface.hpp>
#include <MIDI_Interfaces/MIDI_Interface.hpp>
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

using AH::FilteredAnalog;
using AH::NormalUpdatable;
using AH::Timer;
using AH::Updatable;

/** 
 * @brief   This class ensures initialization, updating, and interaction between
 *          all other classes, it's the glue that holds everything together.
 * 
 * @ingroup ControlSurfaceModule
 */
class Control_Surface_ : public MIDI_Sender, public TrueMIDI_SinkSource {
  public:
    // Copying is not allowed
    Control_Surface_(Control_Surface_ const &) = delete;
    void operator=(Control_Surface_ const &) = delete;

    /**
     * @brief   Return the static Control_Surface_ instance.
     *          (Control_Surface_ is a singleton.)
     */
    static Control_Surface_ &getInstance();

    /**
     * @brief   Initialize the Control_Surface.
     */
    void begin();

    /**
     * @brief   Update all MIDI elements, send MIDI events and read MIDI input.
     */
    void loop();

    /**
     * @brief   Get the MIDI interface of the Control Surface.
     *
     * @return  A reference to the Control Surface's MIDI interface.
     * 
     * @deprecated
     */
    MIDI_Sender &MIDI();

    /// Connect Control Surface to the default MIDI interface:
    bool connectDefaultMIDI();

    /// Accept an incoming MIDI Channel message.
    void sinkMIDIfromPipe(ChannelMessage) override;
    /// Accept an incoming MIDI System Exclusive message.
    void sinkMIDIfromPipe(SysExMessage) override;
    /// Accept an incoming MIDI Real-Time message.
    void sinkMIDIfromPipe(RealTimeMessage) override;

    /// Low-level function for sending a 3-byte MIDI message.
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2,
                  uint8_t cn) override {
        uint8_t h = m | c;
        sourceMIDItoPipe(ChannelMessage{h, d1, d2, cn});
    }
    /**
     * @brief   Low-level function for sending a 2-byte MIDI message.
     */
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t cn) override {
        uint8_t h = m | c;
        sourceMIDItoPipe(ChannelMessage{h, d1, 0, cn});
    }

    /**
     * @brief   Low-level function for sending a system exclusive MIDI message.
     */
    void sendImpl(const uint8_t *data, size_t length, uint8_t cn) override {
        sourceMIDItoPipe(SysExMessage{data, length, cn});
    }

    /** 
     * @brief   Low-level function for sending a single-byte MIDI message.
     */
    void sendImpl(uint8_t rt, uint8_t cn) override {
        sourceMIDItoPipe(RealTimeMessage{rt, cn});
    }

    /**
     * @brief   Update all MIDIInputElement%s.
     */
    void updateInputs();

    /** 
     * @brief   Clear, draw and display all displays.
     */
    void updateDisplays();

  private:
    /**
     * @brief   Control_Surface_ is a singleton, so the constructor is private.
     */
    Control_Surface_() = default;

    /// The pipes used to connect to the MIDI interface.
    BidirectionalMIDI_Pipe pipe;

    /// A timer to know when to update the analog inputs.
    Timer<micros> potentiometerTimer = {AH::FILTERED_INPUT_UPDATE_INTERVAL};
    /// A timer to know when to refresh the displays.
    Timer<micros> displayTimer = {1000000 / MAX_FPS};

  public:
    /// Callback function type for channel messages. Return true if handling is
    /// done in the user-provided callback, false if `Control_Surface`
    /// should handle the message.
    using ChannelMessageCallback = bool (*)(ChannelMessage);
    /// Callback function type for SysEx messages. Return true if handling is
    /// done in the user-provided callback, false if `Control_Surface`
    /// should handle the message.
    using SysExMessageCallback = bool (*)(SysExMessage);
    /// Callback function type for Real-Time messages. Return true if handling
    /// is done in the user-provided callback, false if `Control_Surface`
    /// should handle the message.
    using RealTimeMessageCallback = bool (*)(RealTimeMessage);

    /// Set the MIDI input callbacks.
    void
    setMIDIInputCallbacks(ChannelMessageCallback channelMessageCallback,
                          SysExMessageCallback sysExMessageCallback,
                          RealTimeMessageCallback realTimeMessageCallback) {
        this->channelMessageCallback = channelMessageCallback;
        this->sysExMessageCallback = sysExMessageCallback;
        this->realTimeMessageCallback = realTimeMessageCallback;
    }

  private:
    ChannelMessageCallback channelMessageCallback = nullptr;
    SysExMessageCallback sysExMessageCallback = nullptr;
    RealTimeMessageCallback realTimeMessageCallback = nullptr;
};

/// A predefined instance of the Control Surface to use in the Arduino sketches.
extern Control_Surface_ &Control_Surface;

END_CS_NAMESPACE