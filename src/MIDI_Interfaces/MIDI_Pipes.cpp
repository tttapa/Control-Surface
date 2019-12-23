#include "MIDI_Pipes.hpp"
#include <AH/Error/Error.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

bool MIDI_Sink::connectSourcePipe(MIDI_Pipe *source) {
    if (this->sourcePipe != nullptr) {
        ERROR("MIDI_Sink already has a pipe connected to it", 0x9412);
        return false;
    }
    this->sourcePipe = source;
    return true;
}

void MIDI_Sink::disconnectSourcePipe() { this->sourcePipe = nullptr; }

MIDI_Sink::~MIDI_Sink() {
    if (sourcePipe != nullptr)
        sourcePipe->disconnectSink();
}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

bool MIDI_Source::connectSinkPipe(MIDI_Pipe *sink) {
    if (this->sinkPipe != nullptr) {
        ERROR("MIDI_Sink already has a pipe connected to it", 0x9412);
        return false;
    }
    this->sinkPipe = sink;
    return true;
}

void MIDI_Source::disconnectSinkPipe() { this->sinkPipe = nullptr; }

MIDI_Source::~MIDI_Source() {
    if (sinkPipe != nullptr)
        sinkPipe->disconnectSource();
}

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
    if (hasSink())
        this->sink->disconnectSourcePipe();
    if (sink->connectSourcePipe(this))
        this->sink = sink;
}

void MIDI_Pipe::disconnectSink() {
    if (hasSink())
        this->sink->disconnectSourcePipe();
}

void MIDI_Pipe::connectSource(MIDI_Source *source) {
    if (hasSource())
        this->source->disconnectSinkPipe();
    if (source->connectSinkPipe(this))
        this->sink = sink;
}

void MIDI_Pipe::disconnectSource() {
    if (hasSource())
        this->source->disconnectSinkPipe();
}

MIDI_Pipe::~MIDI_Pipe() {
    if (hasSink() && hasThroughIn()) {
        throughIn->connectSink(sink);
        sink->disconnectSourcePipe();
        sink->connectSourcePipe(throughIn);
    }
    if (hasSource() && hasThroughOut()) {
        throughOut->connectSource(source);
        source->disconnectSinkPipe();
        source->connectSinkPipe(throughIn);
    }
}

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()