#pragma once

#include <AH/Containers/BitArray.hpp>
#include <AH/STL/utility>
#include <AH/Settings/Warnings.hpp>
#include <MIDI_Parsers/MIDI_MessageTypes.hpp>
#include <Settings/NamespaceSettings.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

/// Data type for cable numbers.
using cn_t = uint8_t;

/**
 * @addtogroup MIDI_Routing MIDI Routing
 * @brief   Operators and utilities for MIDI routing.
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
 * @{ 
 */

class MIDI_Pipe;
struct TrueMIDI_Sink;
struct TrueMIDI_Source;

/// Class that can receive MIDI messages from a MIDI pipe.
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
    /// Check if this sink is connected to a source pipe.
    bool hasSourcePipe() const { return sourcePipe != nullptr; }

#ifndef ARDUINO
    MIDI_Pipe *getSourcePipe() { return sourcePipe; }
#endif

    /// @}

  private:
    /// Base case for recursive lock function.
    /// @see    MIDI_Pipe::lockDownstream
    virtual void lockDownstream(cn_t cn, bool lock) { (void)cn, (void)lock; }
    /// Base case for recursive function.
    /// @see    MIDI_Pipe::getFinalSink
    virtual MIDI_Sink *getFinalSink() { return this; }
    /// Disconnect only the first pipe connected to this sink. Leaves the
    /// other pipes connected to the original pipe, which doesn't have a sink
    /// anymore when this function finishes.
    /// Used to disconnect a MIDI_Pipe while preserving the connections of its
    /// "through" inputs.
    void disconnectSourcePipesShallow();

  protected:
    MIDI_Pipe *sourcePipe = nullptr;

    friend class MIDI_Pipe;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// Class that can send MIDI messages to a MIDI pipe.
class MIDI_Source {
  public:
    /// @name Sending data over a MIDI Pipe
    /// @{

    /// Send a MIDI Channel Message.
    void sourceMIDItoPipe(ChannelMessage);
    /// Send a MIDI System Exclusive message.
    void sourceMIDItoPipe(SysExMessage);
    /// Send a MIDI Real-Time message.
    void sourceMIDItoPipe(RealTimeMessage);

    /** 
     * @brief   Enter or exit exclusive mode for the given cable number.
     *          This means that this becomes the only source that can sink to 
     *          the sinks connected to this source. Other sources have to wait
     *          until this source exits exclusive mode until they can send 
     *          again.
     * @param   cn
     *          Cable number to set the exclusive mode for [0, 15].
     * @param   exclusive
     *          True to enable exclusive mode, false to disable.
     */
    void exclusive(cn_t cn, bool exclusive = true);
    /** 
     * @brief   Check if this source can write to the sinks it connects to.
     *          Returns false if any of the sinks have another source that is
     *          in exclusive mode.
     * @param   cn
     *          Cable number to check [0, 15].
     */
    bool canWrite(cn_t cn) const;

    /// @}

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
    /// Check if this source is connected to a sink pipe.
    bool hasSinkPipe() const { return sinkPipe != nullptr; }

#ifndef ARDUINO
    MIDI_Pipe *getSinkPipe() { return sinkPipe; }
#endif

    /// @}

  private:
    /// Base case for recursive function.
    /// @see    MIDI_Pipe::getInitialSource
    virtual MIDI_Source *getInitialSource() { return this; }
    /// Disconnect only the first pipe connected to this source. Leaves the
    /// other pipes connected to the original pipe, which doesn't have a source
    /// anymore when this function finishes.
    /// Used to disconnect a MIDI_Pipe while preserving the connections of its
    /// "through" outputs.
    void disconnectSinkPipesShallow();

  protected:
    MIDI_Pipe *sinkPipe = nullptr;

    friend class MIDI_Pipe;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// A MIDI_Sink that is not a MIDI_Pipe.
struct TrueMIDI_Sink : MIDI_Sink {};
/// A MIDI_Source that is not a MIDI_Pipe.
struct TrueMIDI_Source : MIDI_Source {};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/** 
 * @brief   Class that routes MIDI messages from a MIDI_Source to a MIDI_Sink.
 * 
 * A pipe has at least two, at most four connections. In the simplest case, a 
 * pipe just has a sourca and a sink. The source sends data down the pipe, and
 * the pipe sends it to the sink. A mapping or filter can be applied to the 
 * data traveling down the pipe.
 * 
 * To be able to connect multiple pipes to a single sink or source, a pipe also
 * has a "through" output and a "through" input, both are pipes, not sinks or 
 * sources. All data that comes from the source is sent to the "through" output
 * as well, and all input that comes in from the "through" input is sent to the
 * sink as well. The mapping or filter is not applied to the data going from/to
 * the "through" connections.
 * 
 * Merging data from multiple sources into a single sink can cause problems
 * because messages can be interleaved (e.g. RPN/NRPN messages). To circumvent
 * this issue, a source can request exclusive access to the pipe it's connected
 * to. This locks all other pipes that sink into the same sinks as the exclusive
 * source.  
 * Other sources must then query its pipe before sending, to make sure it's not
 * locked by another source that has exclusive access.
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
 * Each connection between a source and a sink has its own pipe, and no two 
 * pipes are connected in series.
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

  public:
    /// Check if this pipe is connected to a sink.
    bool hasSink() const { return sink != nullptr; }
    /// Check if this pipe is connected to a source.
    bool hasSource() const { return source != nullptr; }
    /// Check if this pipe has a "through" output that sends all incoming
    /// messages from the input (source) to another pipe.
    bool hasThroughOut() const { return throughOut != nullptr; }
    /// Check if this pipe has a "through" input that merges all messages from
    /// another pipe into the output (sink).
    bool hasThroughIn() const { return throughIn != nullptr; }

  protected:
    /// Accept a MIDI message from the source, forward it to the "through"
    /// output if necessary, map or filter the MIDI message if necessary,
    /// and send it to the sink.
    void pipeMIDI(ChannelMessage msg) {
        // Forward 
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        mapForwardMIDI(msg);
    }
    /// @copydoc pipeMIDI
    void pipeMIDI(SysExMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        mapForwardMIDI(msg);
    }
    /// @copydoc pipeMIDI
    void pipeMIDI(RealTimeMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        mapForwardMIDI(msg);
    }

  protected:
    /// Send the given MIDI message to the sink of this pipe.
    void sourceMIDItoSink(ChannelMessage msg) {
        if (hasSink())
            sink->sinkMIDIfromPipe(msg);
    }

    /// @copydoc sourceMIDItoSink
    void sourceMIDItoSink(SysExMessage msg) {
        if (hasSink())
            sink->sinkMIDIfromPipe(msg);
    }

    /// @copydoc sourceMIDItoSink
    void sourceMIDItoSink(RealTimeMessage msg) {
        if (hasSink())
            sink->sinkMIDIfromPipe(msg);
    }

  private:
    /// Function that maps, edits or filters MIDI messages, and then forwards 
    /// them to the sink of the pipe.
    virtual void mapForwardMIDI(ChannelMessage msg) { 
        // Optionally edit the message before passing it on
        sourceMIDItoSink(msg); 
    }

    /// @copydoc    mapForwardMIDI
    virtual void mapForwardMIDI(SysExMessage msg) { 
        // Optionally edit the message before passing it on
        sourceMIDItoSink(msg); 
    }

    /// @copydoc    mapForwardMIDI
    virtual void mapForwardMIDI(RealTimeMessage msg) { 
        // Optionally edit the message before passing it on
        sourceMIDItoSink(msg); 
    }

  private:
    void sinkMIDIfromPipe(ChannelMessage msg) override {
        // Called when data from Through In arrives, forward it to the sink
        sourceMIDItoSink(msg);
    }
    void sinkMIDIfromPipe(SysExMessage msg) override {
        // Called when data from Through In arrives, forward it to the sink
        sourceMIDItoSink(msg);
    }
    void sinkMIDIfromPipe(RealTimeMessage msg) override {
        // Called when data from Through In arrives, forward it to the sink
        sourceMIDItoSink(msg);
    }

  private:
    /// Lock this pipe and all other pipes further downstream (following the
    /// path of the sink). Operates recursively until the end of the
    /// chain is reached.
    void lockDownstream(cn_t cn, bool lock) override {
        lockSelf(cn, lock);
        if (hasSink())
            sink->lockDownstream(cn, lock);
    }

    /// Lock this pipe and all other pipes further upstream (following the
    /// path of the "trough" input). Operates recursively until the end of the
    /// chain is reached.
    void lockUpstream(cn_t cn, bool lock) {
        lockSelf(cn, lock);
        if (hasThroughIn())
            throughIn->lockUpstream(cn, lock);
    }

    /// Lock this pipe, so sources cannot send messages through it.
    void lockSelf(cn_t cn, bool lock) { locks.set(cn, lock); }

  public:
    /// Disconnect this pipe from all other pipes, sources and sinks. If the
    /// "through" input and/or output were in use, they are reconnected to the
    /// original sink and/or source respectively.
    void disconnect();

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

    /// Disconnect the given sink from this pipe. The sink can be connected
    /// directly, or via the "through" output.
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
    /// directly, or via the "through" input.
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

#ifdef ARDUINO
  protected:
#endif
    MIDI_Pipe *getThroughOut() { return throughOut; }
    MIDI_Pipe *getThroughIn() { return throughIn; }
    MIDI_Source *getSource() { return source; }
    MIDI_Sink *getSink() { return sink; }

  public:
    /// @copydoc    MIDI_Source::exclusive
    void exclusive(cn_t cn, bool exclusive = true);

    /// Check if this pipe is locked for a given cable number.
    bool isLocked(cn_t cn) const { return locks.get(cn); }

    /** 
     * @brief   Check if any of the sinks or outputs of this chain of pipes are
     *          locked for the given cable number.
     * @param   cn
     *          The cable number to check.
     */
    bool isAvailableForWrite(cn_t cn) const {
        return !isLocked(cn) &&
               (!hasThroughOut() || throughOut->isAvailableForWrite(cn));
    }

  private:
    MIDI_Sink *sink = nullptr;
    MIDI_Source *source = nullptr;
    MIDI_Pipe *&throughOut = MIDI_Source::sinkPipe;
    MIDI_Pipe *&throughIn = MIDI_Sink::sourcePipe;
    AH::BitArray<16> locks;

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
inline TrueMIDI_Sink &operator>>(MIDI_PipeFactory<N, Pipe> &pipe_fact,
                                 TrueMIDI_Sink &sink) {
    return pipe_fact.getNext() >> sink;
}

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