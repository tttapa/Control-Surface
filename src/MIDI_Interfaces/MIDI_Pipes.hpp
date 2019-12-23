#pragma once

#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

class MIDI_Pipe;

class MIDI_Sink {
  public:
    /// Accept an incoming MIDI Channel message.
    virtual void sinkMIDIfromPipe(ChannelMessage) = 0;
    /// Accept an incoming MIDI System Exclusive message.
    virtual void sinkMIDIfromPipe(SysExMessage) = 0;
    /// Accept an incoming MIDI Real-Time message.
    virtual void sinkMIDIfromPipe(RealTimeMessage) = 0;

    virtual ~MIDI_Sink();

    void connectSourcePipe(MIDI_Pipe *source);
    void disconnectSourcePipe();

    bool hasSourcePipe() const { return sourcePipe != nullptr; }

  protected:
    MIDI_Pipe *sourcePipe = nullptr;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

class MIDI_Source {
  public:
    /// Send a MIDI Channel Message.
    void sourceMIDItoPipe(ChannelMessage);
    /// Send a MIDI System Exclusive message.
    void sourceMIDItoPipe(SysExMessage);
    /// Send a MIDI Real-Time message.
    void sourceMIDItoPipe(RealTimeMessage);

    virtual ~MIDI_Source();

    void connectSinkPipe(MIDI_Pipe *sink);
    void disconnectSinkPipe();

    bool hasSinkPipe() const { return sinkPipe != nullptr; }

  protected:
    MIDI_Pipe *sinkPipe = nullptr;

    friend class MIDI_Pipe;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

class MIDI_Pipe : private MIDI_Sink, private MIDI_Source {
  public:
    MIDI_Pipe() = default;

    MIDI_Pipe(const MIDI_Pipe &) = delete;
    MIDI_Pipe &operator=(const MIDI_Pipe &) = delete;

    /// TODO: move construction/assignment
    MIDI_Pipe(MIDI_Pipe &&) = delete;
    /// TODO: move construction/assignment
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

  public:
    void disconnect();

  private:
    MIDI_Sink *sink = nullptr;
    MIDI_Source *source = nullptr;
    MIDI_Pipe *&throughOut = MIDI_Source::sinkPipe;
    MIDI_Pipe *&throughIn = MIDI_Sink::sourcePipe;

    friend class MIDI_Sink;
    friend class MIDI_Source;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

struct TrueMIDI_Sink : MIDI_Sink {};
struct TrueMIDI_Source : MIDI_Source {};

struct BidirectionalMIDI_Pipe {
    MIDI_Pipe dir1, dir2;
};

struct TrueMIDI_SinkSource : TrueMIDI_Sink, TrueMIDI_Source {};

inline MIDI_Pipe &operator>>(TrueMIDI_Source &source, MIDI_Pipe &pipe) {
    source.connectSinkPipe(&pipe);
    return pipe;
}

inline TrueMIDI_Sink &operator>>(MIDI_Pipe &pipe, TrueMIDI_Sink &sink) {
    sink.connectSourcePipe(&pipe);
    return sink;
}

inline MIDI_Pipe &operator<<(TrueMIDI_Sink &sink, MIDI_Pipe &pipe) {
    sink.connectSourcePipe(&pipe);
    return pipe;
}

inline TrueMIDI_Source &operator<<(MIDI_Pipe &pipe, TrueMIDI_Source &source) {
    source.connectSinkPipe(&pipe);
    return source;
}

inline TrueMIDI_SinkSource &operator|(BidirectionalMIDI_Pipe &pipe,
                                      TrueMIDI_SinkSource &sinksource) {
    sinksource.connectSinkPipe(&pipe.dir1);
    sinksource.connectSourcePipe(&pipe.dir2);
    return sinksource;
}

inline BidirectionalMIDI_Pipe &operator|(TrueMIDI_SinkSource &sinksource,
                                         BidirectionalMIDI_Pipe &pipe) {
    sinksource.connectSinkPipe(&pipe.dir2);
    sinksource.connectSourcePipe(&pipe.dir1);
    return pipe;
}

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()