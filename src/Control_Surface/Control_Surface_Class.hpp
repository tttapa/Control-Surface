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
class Control_Surface_ : public MIDI_Callbacks {
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
     * @todo    This violate's the Law of Demeter.
     */
    MIDI_Interface &MIDI();

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
     * @brief   Control_Surface_ is a singleton, so the constructor is private.
     */
    Control_Surface_() = default;

    /** 
     * @brief   The callback to be called when a MIDI channel message is
     *          received.
     */
    void onChannelMessage(Parsing_MIDI_Interface &midi) override;

    /** 
     * @brief   The callback to be called when a MIDI System Exclusive message
     *          is received.
     */
    void onSysExMessage(Parsing_MIDI_Interface &midi) override;

    /** 
     * @brief   The callback to be called when a MIDI Real-Time message is 
     *          received.
     */
    void onRealtimeMessage(Parsing_MIDI_Interface &midi,
                           uint8_t message) override;

    /// A timer to know when to update the analog inputs.
    Timer<micros> potentiometerTimer = {AH::FILTERED_INPUT_UPDATE_INTERVAL};
    /// A timer to know when to refresh the displays.
    Timer<micros> displayTimer = {1000000UL / MAX_FPS};

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