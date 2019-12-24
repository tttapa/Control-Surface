#include "MIDI_Pipes.hpp"
#include <AH/Error/Error.hpp>

AH_DIAGNOSTIC_WERROR()

#if defined(ESP32) || !defined(ARDUINO)
#include <mutex>
#define GUARD_PIPE_LOCK std::lock_guard<std::mutex> guard_(pipe_mutex)
static std::mutex pipe_mutex;
#else
#define GUARD_PIPE_LOCK
#endif

BEGIN_CS_NAMESPACE

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

void MIDI_Sink::connectSourcePipe(MIDI_Pipe *source) {
    if (this->sourcePipe == nullptr) {
        source->connectSink(this);
        this->sourcePipe = source;
    } else {
        this->sourcePipe->connectSourcePipe(source);
    }
}

void MIDI_Sink::disconnectSourcePipe() {
    if (sourcePipe != nullptr) {
        sourcePipe->disconnectSourcePipe();
        sourcePipe->disconnectSink();
        sourcePipe = nullptr;
    }
}

MIDI_Sink::~MIDI_Sink() { disconnectSourcePipe(); }

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

void MIDI_Source::connectSinkPipe(MIDI_Pipe *sink) {
    if (this->sinkPipe == nullptr) {
        sink->connectSource(this);
        this->sinkPipe = sink;
    } else {
        this->sinkPipe->connectSinkPipe(sink);
    }
}

void MIDI_Source::disconnectSinkPipe() {
    if (sinkPipe != nullptr) {
        sinkPipe->disconnectSinkPipe();
        sinkPipe->disconnectSource();
        sinkPipe = nullptr;
    }
}

MIDI_Source::~MIDI_Source() { disconnectSinkPipe(); }

void MIDI_Source::sourceMIDItoPipe(ChannelMessage msg) {
    if (sinkPipe != nullptr) {
        GUARD_PIPE_LOCK;
        sinkPipe->pipeMIDI(msg);
    }
}
void MIDI_Source::sourceMIDItoPipe(SysExMessage msg) {
    if (sinkPipe != nullptr) {
        GUARD_PIPE_LOCK;
        sinkPipe->pipeMIDI(msg);
    }
}
void MIDI_Source::sourceMIDItoPipe(RealTimeMessage msg) {
    if (sinkPipe != nullptr) {
        GUARD_PIPE_LOCK;
        sinkPipe->pipeMIDI(msg);
    }
}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

void MIDI_Pipe::connectSink(MIDI_Sink *sink) {
    if (this->sink != nullptr) {
        FATAL_ERROR(F("This pipe is already connected to a sink"), 0x9145);
        return; // LCOV_EXCL_LINE
    }
    this->sink = sink;
}

void MIDI_Pipe::disconnectSink() { this->sink = nullptr; }

void MIDI_Pipe::connectSource(MIDI_Source *source) {
    if (this->source != nullptr) {
        FATAL_ERROR(F("This pipe is already connected to a source"), 0x9146);
        return; // LCOV_EXCL_LINE
    }
    this->source = source;
}

void MIDI_Pipe::disconnectSource() { this->source = nullptr; }

void MIDI_Pipe::disconnect() {
    if (hasSink() && hasThroughIn()) {
        auto oldSink = sink;
        auto oldThroughIn = throughIn;
        sink->disconnectSourcePipe();
        this->disconnectSourcePipe(); // disconnect throughIn
        oldSink->connectSourcePipe(oldThroughIn);
    }
    if (hasSource() && hasThroughOut()) {
        auto oldSource = source;
        auto oldThroughOut = throughOut;
        source->disconnectSinkPipe();
        this->disconnectSinkPipe(); // disconnect throughOut
        oldSource->connectSinkPipe(oldThroughOut);
    }
    if (hasSink())
        sink->disconnectSourcePipe();

    if (hasSource())
        source->disconnectSinkPipe();

    if (hasThroughIn() || hasThroughOut())
        FATAL_ERROR(F("Invalid state"), 0x9147); //LCOV_EXCL_LINE
}

MIDI_Pipe::~MIDI_Pipe() { disconnect(); }

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()