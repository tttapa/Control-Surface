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

    /// Return the static Control_Surface_ instance (Control_Surface_ is a
    /// singleton.)
    static Control_Surface_ &getInstance();

  private:
    /// Control_Surface_ is a singleton, so the constructor is private.
    Control_Surface_() = default;

    /// @}

  public:
    /// Initialize the Control_Surface.
    /// @see @ref control-surface-purpose
    void begin();

    /// Update all MIDI elements, send MIDI events and read MIDI input.
    /// @see @ref control-surface-purpose
    void loop();

    /// Connect Control Surface to the default MIDI interface.
    bool connectDefaultMIDI_Interface();

    /// Disconnect Control Surface from the MIDI interfaces it's connected to.
    void disconnectMIDI_Interfaces();

    /// Update all MIDI interfaces to receive new MIDI events.
    void updateMidiInput();
    /// Update all MIDIInputElement%s.
    void updateInputs();
    /// Initialize all displays that have at least one display element.
    void beginDisplays();
    /// Clear, draw and display all displays that contain display elements that
    /// have changed.
    void updateDisplays();

  private:
    /// Low-level function for sending a MIDI channel voice message.
    void sendChannelMessageImpl(ChannelMessage);
    /// Low-level function for sending a MIDI system common message.
    void sendSysCommonImpl(SysCommonMessage);
    /// Low-level function for sending a system exclusive MIDI message.
    void sendSysExImpl(SysExMessage);
    /// Low-level function for sending a MIDI real-time message.
    void sendRealTimeImpl(RealTimeMessage);
    /// Low-level function for sending any buffered outgoing MIDI messages.
    /// @todo Implement this in MIDI_Pipe
    void sendNowImpl() { /* TODO */
    }

  private:
#if !DISABLE_PIPES
    void sinkMIDIfromPipe(ChannelMessage msg) override;
    void sinkMIDIfromPipe(SysExMessage msg) override;
    void sinkMIDIfromPipe(SysCommonMessage msg) override;
    void sinkMIDIfromPipe(RealTimeMessage msg) override;
#else
    void sinkMIDIfromPipe(ChannelMessage msg);
    void sinkMIDIfromPipe(SysExMessage msg);
    void sinkMIDIfromPipe(SysCommonMessage msg);
    void sinkMIDIfromPipe(RealTimeMessage msg);
#endif

  private:
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
    /// Callback function type for System Common messages. Return true if
    /// handling is done in the user-provided callback, false if
    /// `Control_Surface` should handle the message.
    using SysCommonMessageCallback = bool (*)(SysCommonMessage);
    /// Callback function type for Real-Time messages. Return true if handling
    /// is done in the user-provided callback, false if `Control_Surface`
    /// should handle the message.
    using RealTimeMessageCallback = bool (*)(RealTimeMessage);

    /// Set the MIDI input callbacks.
    void
    setMIDIInputCallbacks(ChannelMessageCallback channelMessageCallback,
                          SysExMessageCallback sysExMessageCallback,
                          SysCommonMessageCallback sysCommonMessageCallback,
                          RealTimeMessageCallback realTimeMessageCallback) {
        this->channelMessageCallback = channelMessageCallback;
        this->sysExMessageCallback = sysExMessageCallback;
        this->sysCommonMessageCallback = sysCommonMessageCallback;
        this->realTimeMessageCallback = realTimeMessageCallback;
    }

    /// @}

  private:
    ChannelMessageCallback channelMessageCallback = nullptr;
    SysExMessageCallback sysExMessageCallback = nullptr;
    SysCommonMessageCallback sysCommonMessageCallback = nullptr;
    RealTimeMessageCallback realTimeMessageCallback = nullptr;
#if !DISABLE_PIPES
    MIDI_Pipe inpipe, outpipe;
#endif
};

#if CS_TRUE_CONTROL_SURFACE_INSTANCE || defined(DOXYGEN)
/// A predefined instance of the Control Surface to use in the Arduino sketches.
extern Control_Surface_ &Control_Surface;
#else
// This is not a clean solution, but it's the only way to get the linker to
// optimize away all Control Surface-related code if the `Control_Surface`
// instance is never used.
// Even if it isn't used, and even though it's a global, the compiler has to
// generate the constructor and destructor, which pulls in variables and vtables
// from throughout the library, using a significant amount of memory.
// By using a macro here, Control_Surface is only constructed (and destructed)
// if it is used in user code.
#define Control_Surface (Control_Surface_::getInstance())
#endif

END_CS_NAMESPACE