#include "MIDI_Pipes.hpp"
#include <AH/Error/Error.hpp>

AH_DIAGNOSTIC_WERROR()

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
        sinkPipe->disconnectSource();
        sinkPipe = nullptr;
    }
}

MIDI_Source::~MIDI_Source() { disconnectSinkPipe(); }

void MIDI_Source::sendMIDI(ChannelMessage msg) {
    if (sinkPipe != nullptr)
        sinkPipe->pipeMIDI(msg);
}
void MIDI_Source::sendMIDI(SysExMessage msg) {
    if (sinkPipe != nullptr)
        sinkPipe->pipeMIDI(msg);
}
void MIDI_Source::sendMIDI(RealTimeMessage msg) {
    if (sinkPipe != nullptr)
        sinkPipe->pipeMIDI(msg);
}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

void MIDI_Pipe::connectSink(MIDI_Sink *sink) {
    assert(this->sink == nullptr);
    this->sink = sink;
}

void MIDI_Pipe::disconnectSink() { this->sink = nullptr; }

void MIDI_Pipe::connectSource(MIDI_Source *source) {
    assert(this->source == nullptr);
    this->source = source;
}

void MIDI_Pipe::disconnectSource() { this->source = nullptr; }

void MIDI_Pipe::disconnect() {
    if (hasSink() && hasThroughIn()) {
        auto oldSink = sink;
        sink->disconnectSourcePipe();
        throughIn->disconnectSink();
        oldSink->connectSourcePipe(throughIn);
    }
    if (hasSource() && hasThroughOut()) {
        auto oldSource = source;
        source->disconnectSinkPipe();
        throughOut->disconnectSource();
        oldSource->connectSinkPipe(throughOut);
    }
    if (hasSink()) {
        sink->disconnectSourcePipe();
    }
    if (hasSource()) {
        source->disconnectSinkPipe();
    }
}

MIDI_Pipe::~MIDI_Pipe() { disconnect(); }

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()