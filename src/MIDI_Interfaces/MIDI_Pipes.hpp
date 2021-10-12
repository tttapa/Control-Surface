#pragma once

#include <AH/Containers/BitArray.hpp>
#include <AH/STL/cstdint>
#include <AH/STL/limits>
#include <AH/STL/utility>
#include <AH/Settings/Warnings.hpp>
#include <MIDI_Parsers/MIDI_MessageTypes.hpp>
#include <Settings/NamespaceSettings.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

struct MIDIStaller;
MIDIStaller *const eternal_stall =
    reinterpret_cast<MIDIStaller *>(std::numeric_limits<std::uintptr_t>::max());

/**
 * @addtogroup MIDI_Routing MIDI Routing
 * @brief   Operators and utilities for MIDI routing.
 * 
 * Two or more MIDI interfaces can be connected
 * using @ref MIDI_Pipe "MIDI Pipes". The simplest pipe just carries messages 
 * from the input interface to the output interface, but you can write rules for 
 * filtering out certain messages, changing the channel of some messages, etc.
 * 
 * Allows you to use syntax like:
 * 
 * ~~~cpp
 * HardwareSerialMIDI_Interface 
 *     midiA = Serial1, midiB = Serial2, midiC = Serial3;
 * MIDI_PipeFactory<3> pipes; // Factory that can produce 3 pipes
 * 
 * midiA >> pipes >> midiB;
 * midiC >> pipes >> midiB;
 * midiC << pipes << midiB;
 * ~~~
 * 
 * Or for bidirectional connections:
 * 
 * ~~~cpp
 * HardwareSerialMIDI_Interface 
 *     midiA = Serial1, midiB = Serial2, midiC = Serial3;
 * BidirectionalMIDI_PipeFactory<2> pipes; // Factory that can produce 2 pipes
 * 
 * midiA | pipes | midiB;
 * midiA | pipes | midiC;
 * ~~~
 * 
 * Have a look at the following examples on MIDI routing:
 * 
 *  - @ref MIDI_Pipes-Routing.ino
 *  - @ref Dual-MIDI-Interface.ino
 * 
 * If you're interested how the pipes work, see the documentation for 
 * @ref MIDI_Pipe.
 * 
 * @{ 
 */

class MIDI_Pipe;
class MIDI_Source;
class MIDI_Sink;
/// A MIDI_Sink that is not a MIDI_Pipe.
using TrueMIDI_Sink = MIDI_Sink;
/// A MIDI_Source that is not a MIDI_Pipe.
using TrueMIDI_Source = MIDI_Source;

/// Receives MIDI messages from a MIDI pipe.
class MIDI_Sink {
  public:
    /// Default constructor.
    MIDI_Sink() = default;

    /// Copy constructor (copying not allowed).
    MIDI_Sink(const MIDI_Sink &) = delete;
    /// Copy assignment (copying not allowed).
    MIDI_Sink &operator=(const MIDI_Sink &) = delete;

    /// Move constructor.
    MIDI_Sink(MIDI_Sink &&other);
    /// Move assignment.
    MIDI_Sink &operator=(MIDI_Sink &&other);

    /// Destructor.
    virtual ~MIDI_Sink();

    /// @name Sending data over a MIDI Pipe
    /// @{

    /// Accept an incoming MIDI Channel message.
    virtual void sinkMIDIfromPipe(ChannelMessage) = 0;
    /// Accept an incoming MIDI System Exclusive message.
    virtual void sinkMIDIfromPipe(SysExMessage) = 0;
    /// Accept an incoming MIDI System Common message.
    virtual void sinkMIDIfromPipe(SysCommonMessage) = 0;
    /// Accept an incoming MIDI Real-Time message.
    virtual void sinkMIDIfromPipe(RealTimeMessage) = 0;

    /// @}

    /// @name Connecting and disconnecting MIDI Pipes
    /// @{

    /// Fully connect a source pipe to this sink.
    void connectSourcePipe(MIDI_Pipe *source);
    /// Disconnect all source pipes that sink to this sink (recursively).
    void disconnectSourcePipes();
    /// Disconnect the given source from this sink. Leaves other sources
    /// connected.
    /// Returns true if the source was found and disconnected, false if the
    /// given source was not a direct or indirect source to this sink.
    bool disconnect(TrueMIDI_Source &source);
    bool disconnect(MIDI_Pipe &) = delete;
    /// Check if this sink is connected to a source pipe.
    bool hasSourcePipe() const { return sourcePipe != nullptr; }
    /// Get a pointer to the pipe this sink is connected to, or `nullptr` if
    /// not connected.
    MIDI_Pipe *getSourcePipe() { return sourcePipe; }

    /// @}

  private:
    /// Base case for recursive stall function.
    /// @see    MIDI_Pipe::stallDownstream
    virtual void stallDownstream(MIDIStaller *, MIDI_Source *) {}
    /// Base case for recursive un-stall function.
    /// @see    MIDI_Pipe::unstallDownstream
    virtual void unstallDownstream(MIDIStaller *, MIDI_Source *) {}
    /// Base case for recursive function.
    /// @see    MIDI_Pipe::getFinalSink
    virtual MIDI_Sink *getFinalSink() { return this; }
    /// Disconnect only the first pipe connected to this sink. Leaves the
    /// other pipes connected to the original pipe, which doesn't have a sink
    /// anymore when this function finishes.
    /// Used to disconnect a MIDI_Pipe while preserving the connections of its
    /// “through” inputs.
    void disconnectSourcePipesShallow();

  protected:
    MIDI_Pipe *sourcePipe = nullptr;

    friend class MIDI_Pipe;

  public:
    static void swap(MIDI_Sink &a, MIDI_Sink &b);
    friend void swap(MIDI_Sink &a, MIDI_Sink &b) { swap(a, b); }
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// Class that can send MIDI messages to a MIDI pipe.
class MIDI_Source {
  public:
    /// Default constructor.
    MIDI_Source() = default;

    /// Copy constructor (copying not allowed).
    MIDI_Source(const MIDI_Source &) = delete;
    /// Copy assignment (copying not allowed).
    MIDI_Source &operator=(const MIDI_Source &) = delete;

    /// Move constructor.
    MIDI_Source(MIDI_Source &&other);
    /// Move assignment.
    MIDI_Source &operator=(MIDI_Source &&other);

    /// Destructor.
    virtual ~MIDI_Source();

    /// @name Sending data over a MIDI Pipe
    /// @{

    /// Send a MIDI Channel Message down the pipe.
    void sourceMIDItoPipe(ChannelMessage);
    /// Send a MIDI System Exclusive message down the pipe.
    void sourceMIDItoPipe(SysExMessage);
    /// Send a MIDI System Common message down the pipe.
    void sourceMIDItoPipe(SysCommonMessage);
    /// Send a MIDI Real-Time message down the pipe.
    void sourceMIDItoPipe(RealTimeMessage);

    /// @}

    /// @name Stalling the sink pipes and exclusive access
    /// @{

    /// Stall this MIDI source.
    /// This means that this becomes the only source that can sink to the sinks
    /// connected to this source. Other sources have to wait until this source
    /// un-stalls the pipe before they can send again.
    /// @param  cause
    ///         Pointer to the reason for this stall, can be called back to
    ///         un-stall the pipes.
    void stall(MIDIStaller *cause = eternal_stall);
    /// Un-stall the pipes connected to this source, so other sources
    /// are allowed to send again.
    /// @param  cause
    ///         Pointer to the reason for the stall (this has to be the same one
    ///         that was used to stall).
    void unstall(MIDIStaller *cause = eternal_stall);
    /// Check if this source can write to the sinks it connects to.
    bool isStalled() const;
    /// Get a pointer to whatever is causing this MIDI source to be stalled.
    /// There could be multiple stallers, this function just returns one.
    MIDIStaller *getStaller() const;
    /// Get the name of whatever is causing this MIDI source to be stalled.
    /// There could be multiple stallers, this function just returns one.
    const char *getStallerName() const;
    /// Give the code that is stalling the MIDI sink pipes the opportunity to do
    /// its job and un-stall the pipes.
    void handleStallers() const;

    /// @}

    /// @name Connecting and disconnecting MIDI Pipes
    /// @{

    /// Fully connect a sink pipe to this source.
    void connectSinkPipe(MIDI_Pipe *sink);
    /// Disconnect all sink pipes that this source sinks to (recursively).
    void disconnectSinkPipes();
    /// Disconnect the given sink from this source. Leaves other sinks
    /// connected.
    /// Returns true if the sink was found and disconnected, false if the
    /// given sink was not a direct or indirect sink of this source.
    bool disconnect(TrueMIDI_Sink &sink);
    bool disconnect(MIDI_Pipe &) = delete;
    /// Check if this source is connected to a sink pipe.
    bool hasSinkPipe() const { return sinkPipe != nullptr; }
    /// Get a pointer to the pipe this source is connected to, or `nullptr` if
    /// not connected.
    MIDI_Pipe *getSinkPipe() { return sinkPipe; }

    /// @}

  private:
    /// Base case for recursive stall function.
    /// @see    MIDI_Pipe::stallUpstream
    virtual void stallUpstream(MIDIStaller *, MIDI_Sink *) {}
    /// Base case for recursive un-stall function.
    /// @see    MIDI_Pipe::unstallUpstream
    virtual void unstallUpstream(MIDIStaller *, MIDI_Sink *) {}
    /// Base case for recursive function.
    /// @see    MIDI_Pipe::getInitialSource
    virtual MIDI_Source *getInitialSource() { return this; }
    /// Disconnect only the first pipe connected to this source. Leaves the
    /// other pipes connected to the original pipe, which doesn't have a source
    /// anymore when this function finishes.
    /// Used to disconnect a @ref MIDI_Pipe while preserving the connections of
    /// its “through” outputs.
    void disconnectSinkPipesShallow();

  protected:
    MIDI_Pipe *sinkPipe = nullptr;

    friend class MIDI_Pipe;

  public:
    static void swap(MIDI_Source &a, MIDI_Source &b);
    friend void swap(MIDI_Source &a, MIDI_Source &b) { swap(a, b); }
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/** 
 * @brief   Class that routes MIDI messages from a MIDI_Source to a MIDI_Sink.
 * 
 * A pipe has at least two, at most four connections. In the simplest case, a 
 * pipe just has a source and a sink. The source sends MIDI messages down the 
 * pipe, and the pipe sends them to the sink. A mapping or filter can be applied
 * to the messages traveling down the pipe.
 * 
 * To be able to connect multiple pipes to a single sink or source, a pipe also
 * has a “through” output and a “through” input. Both are pipes, not sinks or 
 * sources. All data that comes from the source is sent to the “through” output
 * as well, and all input that comes in from the “through” input is sent to the
 * sink as well. The mapping or filter is not applied to the data going from/to
 * the “through” connections.
 * 
 * Merging data from multiple sources into a single sink can cause problems
 * because messages can be interleaved (e.g. RPN/NRPN or chunked system 
 * exclusive messages). To circumvent this issue, a source can request exclusive
 * access to the pipe it's connected to. This stalls all other pipes that sink
 * into the same sinks as the exclusive source.  
 * When other sources try to send to a stalled pipe, this will automatically 
 * call back the source that originally stalled the pipes, so it can finish its
 * message, and then un-stall the pipes so the other pipe can send its data.
 * 
 * **Pipe model**
 * 
 * ~~~
 *                  ╭────────────────>  through out
 *                  │ ┌────────┐
 *     source  >━━━━┷━┥ filter ┝━┯━━━>  sink
 *                    └────────┘ │
 * through in  >─────────────────╯
 * ~~~
 * 
 * For example, if you have one source that should connect to two sinks, the
 * configuration is as follows:
 * 
 * ~~~
 *                      through out         × ┌────────┐
 *                  ╭────────────────> >━━━━┷━┥ pipe 2 ┝━┯━━━>  sink 2
 *                  │ ┌────────┐              └────────┘ ×
 *     source  >━━━━┷━┥ pipe 1 ┝━┯━━━━━━━━━━━━━━━━━━━━━━━━━━━>  sink 1
 *                    └────────┘ ×
 * ~~~
 * 
 * If you have two sources that should connect to the same sink, a possible
 * configuration is:
 * 
 * ~~~
 *                                          × ┌────────┐
 *   source 1  >━━━━━━━━━━━━━━━━━━━━━━━━━━━━┷━┥ pipe 1 ┝━┯━━━>  sink
 *                  × ┌────────┐              └────────┘ │
 *   source 2  >━━━━┷━┥ pipe 2 ┝━┯━━━> >─────────────────╯
 *                    └────────┘ ×
 * ~~~
 * 
 * Each connection between a source and a sink has its own pipe, and no two 
 * pipes are connected in series (only through the “through“ inputs/outputs).
 */
class MIDI_Pipe : private MIDI_Sink, private MIDI_Source {
  public:
    /// Default constructor
    MIDI_Pipe() = default;

    /// Copy constructor (copying not allowed).
    MIDI_Pipe(const MIDI_Pipe &) = delete;
    /// Copy assignment (copying not allowed).
    MIDI_Pipe &operator=(const MIDI_Pipe &) = delete;

    /// Move constructor.
    /// @todo   Add move constructor.
    MIDI_Pipe(MIDI_Pipe &&) = delete;
    /// Move assignment.
    /// @todo   Add move assignment.
    MIDI_Pipe &operator=(MIDI_Pipe &&) = delete;

    /// Destructor.
    virtual ~MIDI_Pipe();

  private:
    /// @name Mapping and filtering
    /// @{

    /// Function that maps, edits or filters MIDI messages, and then forwards
    /// them to the sink of the pipe.
    /// The MIDI_Pipe base class just forwards the messages to the sink, but
    /// you can override this method to create pipes that filter out some
    /// messages, transposes notes, changes the channel, etc.
    virtual void mapForwardMIDI(ChannelMessage msg) { sourceMIDItoSink(msg); }
    /// @copydoc    mapForwardMIDI
    virtual void mapForwardMIDI(SysExMessage msg) { sourceMIDItoSink(msg); }
    /// @copydoc    mapForwardMIDI
    virtual void mapForwardMIDI(SysCommonMessage msg) { sourceMIDItoSink(msg); }
    /// @copydoc    mapForwardMIDI
    virtual void mapForwardMIDI(RealTimeMessage msg) { sourceMIDItoSink(msg); }

    /// @}

  public:
    /// @name Dealing with stalled pipes
    /// @{

    /// Check if this pipe is stalled.
    bool isStalled() const { return sink_staller || through_staller; }
    /// Get the staller (cause of the stall) that causes the sink of this pipe
    /// to be stalled.
    /// This pipe could sink to more than one stalled sink, this function just
    /// returns one of the causes.
    MIDIStaller *getSinkStaller() const { return sink_staller; }
    /// Get the name of the staller (cause of the stall) that causes the sink
    /// of this pipe to be stalled.
    const char *getSinkStallerName() const;
    /// Get the staller (cause of the stall) that causes the “through” output
    /// of this pipe to be stalled.
    /// The “through” output of this pipe could sink to more than one stalled
    /// sink, this function just returns one of the causes.
    MIDIStaller *getThroughStaller() const { return through_staller; }
    /// Get the name of the staller (cause of the stall) that causes the
    /// “through” output of this pipe to be stalled.
    const char *getThroughStallerName() const;
    /// Get any staller: returns @ref getSinkStaller() if it's not null,
    /// @ref getThroughStaller() otherwise.
    MIDIStaller *getStaller() const;
    /// Get the name of any staller.
    /// @see getStaller
    const char *getStallerName() const;
    /// Returns true if this pipe is either not stalled at all, or if the pipe
    /// is stalled by the given staller (cause).
    /// @see getSinkStaller
    bool sinkIsUnstalledOrStalledBy(MIDIStaller *cause) {
        return sink_staller == nullptr || sink_staller == cause;
    }
    /// Returns true if this pipe is either not stalled at all, or if the pipe
    /// is stalled by the given staller (cause).
    /// @see getThroughStaller
    bool throughIsUnstalledOrStalledBy(MIDIStaller *cause) {
        return through_staller == nullptr || through_staller == cause;
    }

    /// Give the code that is stalling the MIDI pipe the opportunity to do
    /// its job and unstall the pipe.
    void handleStallers() const;

    /// @}

  public:
    /// @name Check connections
    /// @{

    /// Check if this pipe is connected to a sink.
    bool hasSink() const { return sink != nullptr; }
    /// Check if this pipe is connected to a source.
    bool hasSource() const { return source != nullptr; }
    /// Check if this pipe has a “through” output that sends all incoming
    /// messages from the input (source) to another pipe.
    bool hasThroughOut() const { return throughOut != nullptr; }
    /// Check if this pipe has a “through” input that merges all messages from
    /// another pipe into the output (sink).
    bool hasThroughIn() const { return throughIn != nullptr; }

    /// @}

  public:
    /// @name MIDI Pipe connection management and inspection
    /// @{

    /// Disconnect this pipe from all other pipes, sources and sinks. If the
    /// “through” input and/or output were in use, they are reconnected to their
    /// original sink and/or source respectively, their behavior doesn't change.
    void disconnect();
    /// Disconnect the given sink from this pipe. The sink can be connected
    /// directly, or via the “through” output.
    /// Returns true if the sink was found and disconnected, false if the given
    /// sink was not a direct or indirect sink of this pipe.
    bool disconnect(TrueMIDI_Sink &sink) {
        if (getFinalSink() == &sink) {
            disconnect();
            return true;
        }
        if (hasThroughOut()) {
            return throughOut->disconnect(sink);
        }
        return false;
    }
    /// Disconnect the given source from this pipe. The source can be connected
    /// directly, or via the “through” input.
    /// Returns true if the source was found and disconnected, false if the
    /// given source was not a direct or indirect source to this pipe.
    bool disconnect(TrueMIDI_Source &source) {
        if (getInitialSource() == &source) {
            disconnect();
            return true;
        }
        if (hasThroughIn()) {
            return throughIn->disconnect(source);
        }
        return false;
    }
    bool disconnect(MIDI_Pipe &) = delete;

    /// Get the immediate source of this pipe.
    MIDI_Source *getSource() { return source; }
    /// Get the immediate sink of this pipe.
    MIDI_Sink *getSink() { return sink; }
    /// Get the pipe connected to the “through” output of this pipe.
    MIDI_Pipe *getThroughOut() { return throughOut; }
    /// Get the pipe connected to the “through” input of this pipe.
    MIDI_Pipe *getThroughIn() { return throughIn; }

    /// Get the sink this pipe eventually sinks to, following the chain
    /// recursively.
    MIDI_Sink *getFinalSink() override {
        return hasSink() ? sink->getFinalSink() : nullptr;
    }
    /// Get the original source that sources to this pipe, following the chain
    /// recursively.
    MIDI_Source *getInitialSource() override {
        return hasSource() ? source->getInitialSource() : nullptr;
    }

    /// @}

  private:
    /// @name Private functions to connect and disconnect sinks and sources
    /// @{

    /// Set the sink pointer to point to the given sink. Does not connect this
    /// pipe to the sink. Initiate the connection from the sink.
    void connectSink(MIDI_Sink *sink);
    /// Set the sink pointer to null. Does not disconnect this pipe from the
    /// sink. Initiate the disconnection from the sink.
    void disconnectSink();
    /// Set the source pointer to point to the given source. Does not connect
    /// this pipe to the source. Initiate the connection from the source.
    void connectSource(MIDI_Source *source);
    /// Set the source pointer to null. Does not disconnect this pipe from the
    /// source. Initiate the disconnection from the source.
    void disconnectSource();

    /// @}

  protected:
    /// Send the given MIDI message to the sink of this pipe.
    /// Useful when overriding @ref mapForwardMIDI.
    template <class Message>
    void sourceMIDItoSink(Message msg) {
        if (hasSink())
            sink->sinkMIDIfromPipe(msg);
    }

  protected:
    /// Accept a MIDI message from the source, forward it to the “through”
    /// output if necessary, map or filter the MIDI message if necessary,
    /// and send it to the sink. This function transfers messages from a
    /// @ref MIDI_Source to its @ref MIDI_Pipe.
    template <class Message>
    void acceptMIDIfromSource(Message msg) {
        if (hasThroughOut())
            throughOut->acceptMIDIfromSource(msg);
        mapForwardMIDI(msg);
    }

  private:
    /// Called when data arrives from an upstream pipe connected to our
    /// “through” input, this function forwards it to the sink.
    void sinkMIDIfromPipe(ChannelMessage msg) override {
        sourceMIDItoSink(msg);
    }
    /// @copydoc sinkMIDIfromPipe
    void sinkMIDIfromPipe(SysExMessage msg) override { sourceMIDItoSink(msg); }
    /// @copydoc sinkMIDIfromPipe
    void sinkMIDIfromPipe(SysCommonMessage msg) override {
        sourceMIDItoSink(msg);
    }
    /// @copydoc sinkMIDIfromPipe
    void sinkMIDIfromPipe(RealTimeMessage msg) override {
        sourceMIDItoSink(msg);
    }

  private:
    /// @name Private functions to stall and un-stall pipes
    /// @{

    /// Stall this pipe and all other pipes further downstream (following the
    /// path of the sink and the “through” output). Operates recursively until
    /// the end of the chain is reached, and then continues upstream (using
    /// @ref stallUpstream) to stall all pipes that connect to sources that
    /// sink to the same sink as this pipe and its “through” output.
    /// In short: stall all pipes that sink to the same sink as this pipe, and
    /// then stall all pipes that source to this first set of pipes.
    void stallDownstream(MIDIStaller *cause, MIDI_Source *stallsrc) override;
    /// Undoes the stalling by @ref stallDownstream.
    void unstallDownstream(MIDIStaller *cause, MIDI_Source *stallsrc) override;

    /// Stall this pipe and all other pipes further upstream (following the
    /// path of the "trough" input). Operates recursively until the end of the
    /// chain is reached. This function is called in the second stage of
    /// @ref stallDownstream.
    void stallUpstream(MIDIStaller *cause, MIDI_Sink *stallsrc) override;
    /// Undoes the stalling by @ref stallUpstream.
    void unstallUpstream(MIDIStaller *cause, MIDI_Sink *stallsrc) override;

    /// @}

  private:
    MIDI_Sink *sink = nullptr;
    MIDI_Source *source = nullptr;
    MIDI_Pipe *&throughOut = MIDI_Source::sinkPipe;
    MIDI_Pipe *&throughIn = MIDI_Sink::sourcePipe;
    MIDIStaller *sink_staller = nullptr;
    MIDIStaller *through_staller = nullptr;

    friend class MIDI_Sink;
    friend class MIDI_Source;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// A struct that is both a TrueMIDI_Sink and a TrueMIDI_Source.
struct TrueMIDI_SinkSource : TrueMIDI_Sink, TrueMIDI_Source {};

/// A bidirectional pipe consists of two unidirectional pipes.
using BidirectionalMIDI_Pipe = std::pair<MIDI_Pipe, MIDI_Pipe>;

/// Connect a source to a pipe (`source >> pipe`).
inline MIDI_Pipe &operator>>(TrueMIDI_Source &source, MIDI_Pipe &pipe) {
    source.connectSinkPipe(&pipe);
    return pipe;
}

/// Connect a pipe to a sink (`pipe >> sink`).
inline TrueMIDI_Sink &operator>>(MIDI_Pipe &pipe, TrueMIDI_Sink &sink) {
    sink.connectSourcePipe(&pipe);
    return sink;
}

/// Connect a sink to a pipe (`sink << pipe`).
inline MIDI_Pipe &operator<<(TrueMIDI_Sink &sink, MIDI_Pipe &pipe) {
    sink.connectSourcePipe(&pipe);
    return pipe;
}

/// Connect a pipe to a source (`pipe << source`).
inline TrueMIDI_Source &operator<<(MIDI_Pipe &pipe, TrueMIDI_Source &source) {
    source.connectSinkPipe(&pipe);
    return source;
}

/// Don't connect two pipes to eachother.
MIDI_Pipe &operator<<(MIDI_Pipe &, MIDI_Pipe &) = delete;

/// Connect a pipe to a sink+source (`pipe | source+sink`).
inline TrueMIDI_SinkSource &operator|(BidirectionalMIDI_Pipe &pipe,
                                      TrueMIDI_SinkSource &sinksource) {
    sinksource.connectSinkPipe(&pipe.first);
    sinksource.connectSourcePipe(&pipe.second);
    return sinksource;
}

/// Connect a sink+source to a pipe (`source+sink | pipe`).
inline BidirectionalMIDI_Pipe &operator|(TrueMIDI_SinkSource &sinksource,
                                         BidirectionalMIDI_Pipe &pipe) {
    sinksource.connectSinkPipe(&pipe.second);
    sinksource.connectSourcePipe(&pipe.first);
    return pipe;
}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/**
 * @brief   Class that produces multiple MIDI_Pipe%s.
 * 
 * @tparam  N 
 *          The maximum number of pipes it can produce.
 * @tparam  Pipe 
 *          The type of pipes to produce.
 */
template <size_t N, class Pipe = MIDI_Pipe>
struct MIDI_PipeFactory {
    Pipe pipes[N];
    size_t index = 0;

    Pipe &getNext() {
        if (index >= N)
            FATAL_ERROR(F("Not enough pipes available"), 0x2459);
        return pipes[index++];
    }
    Pipe &operator[](size_t i) { return pipes[i]; }
    const Pipe &operator[](size_t i) const { return pipes[i]; }
};

template <size_t N>
using BidirectionalMIDI_PipeFactory =
    MIDI_PipeFactory<N, BidirectionalMIDI_Pipe>;

template <size_t N, class Pipe>
inline MIDI_Pipe &operator>>(TrueMIDI_Source &source,
                             MIDI_PipeFactory<N, Pipe> &pipe_fact) {
    return source >> pipe_fact.getNext();
}

template <size_t N, class Pipe>
MIDI_Pipe &operator>>(MIDI_Pipe &, MIDI_PipeFactory<N, Pipe> &) = delete;

template <size_t N, class Pipe>
inline TrueMIDI_Sink &operator>>(MIDI_PipeFactory<N, Pipe> &pipe_fact,
                                 TrueMIDI_Sink &sink) {
    return pipe_fact.getNext() >> sink;
}

template <size_t N, class Pipe>
MIDI_Pipe &operator>>(MIDI_PipeFactory<N, Pipe> &, MIDI_Pipe &) = delete;

template <size_t N, class Pipe>
inline MIDI_Pipe &operator<<(TrueMIDI_Sink &sink,
                             MIDI_PipeFactory<N, Pipe> &pipe_fact) {
    return sink << pipe_fact.getNext();
}

template <size_t N, class Pipe>
inline TrueMIDI_Source &operator<<(MIDI_PipeFactory<N, Pipe> &pipe_fact,
                                   TrueMIDI_Source &source) {
    return pipe_fact.getNext() << source;
}

template <size_t N>
inline TrueMIDI_SinkSource &
operator|(BidirectionalMIDI_PipeFactory<N> &pipe_fact,
          TrueMIDI_SinkSource &sinksource) {
    return pipe_fact.getNext() | sinksource;
}

template <size_t N>
inline BidirectionalMIDI_Pipe &
operator|(TrueMIDI_SinkSource &sinksource,
          BidirectionalMIDI_PipeFactory<N> &pipe_fact) {
    return sinksource | pipe_fact.getNext();
}

/// @}

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()