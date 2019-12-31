#pragma once

#include <AH/Containers/BitArray.hpp>
#include <AH/STL/utility>
#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>
#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

using cn_t = uint8_t;

class MIDI_Pipe;
struct TrueMIDI_Sink;
struct TrueMIDI_Source;

/// Class that can receive MIDI messages from a MIDI pipe.
class MIDI_Sink {
  public:
    /// Accept an incoming MIDI Channel message.
    virtual void sinkMIDIfromPipe(ChannelMessage) = 0;
    /// Accept an incoming MIDI System Exclusive message.
    virtual void sinkMIDIfromPipe(SysExMessage) = 0;
    /// Accept an incoming MIDI Real-Time message.
    virtual void sinkMIDIfromPipe(RealTimeMessage) = 0;

    /// Default constructor.
    MIDI_Sink() = default;

    /// Copy constructor.
    MIDI_Sink(const MIDI_Sink &) = delete;
    /// Copy assignment.
    MIDI_Sink &operator=(const MIDI_Sink &) = delete;

    /// Move constructor.
    MIDI_Sink(MIDI_Sink &&other);
    /// Move assignment.
    MIDI_Sink &operator=(MIDI_Sink &&other);

    /// Destructor.
    virtual ~MIDI_Sink();

    void connectSourcePipe(MIDI_Pipe *source);
    void disconnectSourcePipes();

    bool disconnect(TrueMIDI_Source &source);

    bool hasSourcePipe() const { return sourcePipe != nullptr; }

#ifndef ARDUINO
    MIDI_Pipe *getSourcePipe() { return sourcePipe; }
#endif

  private:
    virtual void lockDownstream(cn_t cn, bool lock) { (void)cn, (void)lock; }
    virtual MIDI_Sink *getFinalSink() { return this; }
    void disconnectSourcePipesShallow();

  protected:
    MIDI_Pipe *sourcePipe = nullptr;

    friend class MIDI_Pipe;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// Class that can send MIDI messages to a MIDI pipe.
class MIDI_Source {
  public:
    /// Send a MIDI Channel Message.
    void sourceMIDItoPipe(ChannelMessage);
    /// Send a MIDI System Exclusive message.
    void sourceMIDItoPipe(SysExMessage);
    /// Send a MIDI Real-Time message.
    void sourceMIDItoPipe(RealTimeMessage);

    /// Default constructor.
    MIDI_Source() = default;

    /// Copy constructor.
    MIDI_Source(const MIDI_Source &) = delete;
    /// Copy assignment.
    MIDI_Source &operator=(const MIDI_Source &) = delete;

    /// Move constructor.
    MIDI_Source(MIDI_Source &&other);
    /// Move assignment.
    MIDI_Source &operator=(MIDI_Source &&other);

    /// Destructor.
    virtual ~MIDI_Source();

    void connectSinkPipe(MIDI_Pipe *sink);
    void disconnectSinkPipes();

    bool disconnect(TrueMIDI_Sink &sink);

    bool hasSinkPipe() const { return sinkPipe != nullptr; }

    void exclusive(cn_t cn, bool exclusive = true);
    bool canWrite(cn_t cn) const;

#ifndef ARDUINO
    MIDI_Pipe *getSinkPipe() { return sinkPipe; }
#endif

  private:
    virtual MIDI_Source *getInitialSource() { return this; }
    void disconnectSinkPipesShallow();

  protected:
    MIDI_Pipe *sinkPipe = nullptr;

    friend class MIDI_Pipe;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

struct TrueMIDI_Sink : MIDI_Sink {};
struct TrueMIDI_Source : MIDI_Source {};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

/// Class that routes MIDI messages from a MIDI_Source to a MIDI_Sink.
class MIDI_Pipe : private MIDI_Sink, private MIDI_Source {
  public:
    MIDI_Pipe() = default;

    MIDI_Pipe(const MIDI_Pipe &) = delete;
    MIDI_Pipe &operator=(const MIDI_Pipe &) = delete;

    /// TODO: move construction/assignment?
    MIDI_Pipe(MIDI_Pipe &&) = delete;
    /// TODO: move construction/assignment?
    MIDI_Pipe &operator=(MIDI_Pipe &&) = delete;

    virtual ~MIDI_Pipe();

  private:
    void connectSink(MIDI_Sink *sink);
    void disconnectSink();
    void connectSource(MIDI_Source *source);
    void disconnectSource();

  public:
    bool hasSink() const { return sink != nullptr; }
    bool hasSource() const { return source != nullptr; }
    bool hasThroughOut() const { return throughOut != nullptr; }
    bool hasThroughIn() const { return throughIn != nullptr; }

  private:
    virtual void pipeMIDI(ChannelMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        // map(msg)
        sinkMIDIfromPipe(msg);
    }
    virtual void pipeMIDI(SysExMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        // map(msg)
        sinkMIDIfromPipe(msg);
    }
    virtual void pipeMIDI(RealTimeMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        // map(msg)
        sinkMIDIfromPipe(msg);
    }

    /// Forward from source through to output without processing;
    void sinkMIDIfromPipe(ChannelMessage msg) final override {
        if (hasSink())
            sink->sinkMIDIfromPipe(msg);
    }
    /// Forward froum source through to output without processing;
    void sinkMIDIfromPipe(SysExMessage msg) final override {
        if (hasSink())
            sink->sinkMIDIfromPipe(msg);
    }
    /// Forward froum source through to output without processing;
    void sinkMIDIfromPipe(RealTimeMessage msg) final override {
        if (hasSink())
            sink->sinkMIDIfromPipe(msg);
    }

    void lockDownstream(cn_t cn, bool lock) override {
        lockSelf(cn, lock);
        if (hasSink())
            sink->lockDownstream(cn, lock);
    }

    void lockUpstream(cn_t cn, bool lock) {
        lockSelf(cn, lock);
        if (hasThroughIn())
            throughIn->lockUpstream(cn, lock);
    }

    void lockSelf(cn_t cn, bool lock) { locks.set(cn, lock); }

  public:
    void disconnect();

    MIDI_Sink *getFinalSink() override {
        return hasSink() ? sink->getFinalSink() : nullptr;
    }
    MIDI_Source *getInitialSource() override {
        return hasSource() ? source->getInitialSource() : nullptr;
    }

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

#ifndef ARDUINO
    MIDI_Pipe *getThroughOut() { return throughOut; }
    MIDI_Pipe *getThroughIn() { return throughIn; }
#endif

    void exclusive(cn_t cn, bool exclusive = true);

    bool isLocked(cn_t cn) const { return locks.get(cn); }

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

struct TrueMIDI_SinkSource : TrueMIDI_Sink, TrueMIDI_Source {};

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