#pragma once

#include <Settings/SettingsWrapper.hpp>
#if !DISABLE_PIPES

#include <AH/STL/utility> // std::forward
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// Struct that can cause a MIDI_Pipe to be stalled.
struct MIDIStaller {
    virtual ~MIDIStaller() = default;
    /// Get the staller's name for debugging purposes.
    virtual const char *getName() const { return "<?>"; }
    /// Call back that should finish any MIDI messages that are in progress, and
    /// un-stall the pipe or MIDI source as quickly as possible.
    virtual void handleStall() = 0;

    /// Get the staller's name for debugging purposes. Correctly deals with
    /// null pointers or eternal stallers.
    static const char *getNameNull(MIDIStaller *s);
};

/// Allocate a MIDIStaller that executes the given callback and deletes itself
/// when @ref MIDIStaller::handleStall is called.
/// @note   Don't lose the pointer! If you never call `handleStall`, the memory
///         won't be deallocated.
template <class Callback>
auto makeMIDIStaller(Callback &&callback) -> MIDIStaller * {

    struct AutoCleanupMIDIStaller : MIDIStaller {
        AutoCleanupMIDIStaller(Callback &&callback)
            : callback(std::forward<Callback>(callback)) {}

        void handleStall() override {
            callback(this);
            delete this;
        }

        Callback callback;
    };
    return new AutoCleanupMIDIStaller(std::forward<Callback>(callback));
}

END_CS_NAMESPACE

#else

BEGIN_CS_NAMESPACE

struct MIDIStaller {};

END_CS_NAMESPACE

#endif