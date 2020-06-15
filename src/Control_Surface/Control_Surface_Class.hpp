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
class Control_Surface_ : public MIDI_Sender<Control_Surface_>,
                         public TrueMIDI_SinkSource {

    friend class MIDI_Sender<Control_Surface_>;

    /// @name Singleton boilerplate
    /// @{

  public:
    /// Copying is not allowed
    Control_Surface_(Control_Surface_ const &) = delete;
    /// Copying is not allowed
    Control_Surface_ &operator=(Control_Surface_ const &) = delete;

    /**
     * @brief   Return the static Control_Surface_ instance.
     *          (Control_Surface_ is a singleton.)
     */
    static Control_Surface_ &getInstance();

  private:
    /**
     * @brief   Control_Surface_ is a singleton, so the constructor is private.
     */
    Control_Surface_() = default;

    /// @}

  public:
    /**
     * @brief   Initialize the Control_Surface.
     */
    void begin();

    /**
     * @brief   Update all MIDI elements, send MIDI events and read MIDI input.
     */
    void loop();

    /**
     * @brief   Connect Control Surface to the default MIDI interface.
     */
    bool connectDefaultMIDI_Interface();

    /**
     * @brief   Disconnect Control Surface from the MIDI interfaces it's 
     *          connected to.
     */
    void disconnectMIDI_Interfaces();

    /**
     * @brief   Get a reference to the MIDI sender.
     * 
     * @deprecated  Use `Control_Surface.send(...)` directly instead of 
     *              `Control_Surface.MIDI().send(...)`.
     */
    [[deprecated("Use Control_Surface.send(...) directly, instead of "
                 "Control_Surface.MIDI().send(...)")]] //
    MIDI_Sender<Control_Surface_> &
    MIDI() {
        return *this;
    }
    /** 
     * @brief   Update all MIDI interfaces to receive new MIDI events.
     */
    void updateMidiInput();

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
     * @brief   Low-level function for sending a 3-byte MIDI message.
     */
    void sendImpl(uint8_t header, uint8_t d1, uint8_t d2, uint8_t cn);
    /**
     * @brief   Low-level function for sending a 2-byte MIDI message.
     */
    void sendImpl(uint8_t header, uint8_t d1, uint8_t cn);

    /**
     * @brief   Low-level function for sending a system exclusive MIDI message.
     */
    void sendImpl(const uint8_t *data, size_t length, uint8_t cn);

    /** 
     * @brief   Low-level function for sending a single-byte MIDI message.
     */
    void sendImpl(uint8_t rt, uint8_t cn);

  private:
    void sinkMIDIfromPipe(ChannelMessage msg) override;
    void sinkMIDIfromPipe(SysExMessage msg) override;
    void sinkMIDIfromPipe(RealTimeMessage msg) override;

  private:
    /// A timer to know when to update the analog inputs.
    Timer<micros> potentiometerTimer = {AH::FILTERED_INPUT_UPDATE_INTERVAL};
    /// A timer to know when to refresh the displays.
    Timer<micros> displayTimer = {1000000UL / MAX_FPS};

  public:
    /// @name MIDI Input Callbacks
    /// @{

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

    /// @}

  private:
    ChannelMessageCallback channelMessageCallback = nullptr;
    SysExMessageCallback sysExMessageCallback = nullptr;
    RealTimeMessageCallback realTimeMessageCallback = nullptr;
    MIDI_Pipe inpipe, outpipe;
};

/// A predefined instance of the Control Surface to use in the Arduino sketches.
extern Control_Surface_ &Control_Surface;

END_CS_NAMESPACE