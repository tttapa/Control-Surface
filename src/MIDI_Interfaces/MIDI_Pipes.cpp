#include "MIDI_Pipes.hpp"
#include <AH/Error/Error.hpp>
#include <AH/STL/utility>

#if defined(ESP32) || !defined(ARDUINO)
#include <mutex>
#endif

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

void MIDI_Sink::disconnectSourcePipes() {
    if (sourcePipe != nullptr) {
        sourcePipe->disconnectSourcePipes();
        sourcePipe->disconnect();
        sourcePipe = nullptr;
    }
}

void MIDI_Sink::disconnectSourcePipesShallow() {
    if (sourcePipe != nullptr) {
        sourcePipe->disconnectSink();
        sourcePipe = nullptr;
    }
}

bool MIDI_Sink::disconnect(TrueMIDI_Source &source) {
    if (!hasSourcePipe())
        return false;
    return sourcePipe->disconnect(source);
}

MIDI_Sink::~MIDI_Sink() { disconnectSourcePipes(); }

MIDI_Sink::MIDI_Sink(MIDI_Sink &&other)
    : sourcePipe(std::exchange(other.sourcePipe, nullptr)) {
    if (this->hasSourcePipe()) {
        this->sourcePipe->disconnectSink();
        this->sourcePipe->connectSink(this);
    }
}

MIDI_Sink &MIDI_Sink::operator=(MIDI_Sink &&other) {
    std::swap(this->sourcePipe, other.sourcePipe);
    if (this->hasSourcePipe()) {
        this->sourcePipe->disconnectSink();
        this->sourcePipe->connectSink(this);
    }
    if (other.hasSourcePipe()) {
        other.sourcePipe->disconnectSink();
        other.sourcePipe->connectSink(this);
    }
    return *this;
}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

void MIDI_Source::connectSinkPipe(MIDI_Pipe *sink) {
    if (this->sinkPipe == nullptr) {
        sink->connectSource(this);
        this->sinkPipe = sink;
    } else {
        this->sinkPipe->connectSinkPipe(sink);
    }
}

void MIDI_Source::disconnectSinkPipes() {
    if (sinkPipe != nullptr) {
        sinkPipe->disconnectSinkPipes();
        sinkPipe->disconnect();
        sinkPipe = nullptr;
    }
}

void MIDI_Source::disconnectSinkPipesShallow() {
    if (sinkPipe != nullptr) {
        sinkPipe->disconnectSource();
        sinkPipe = nullptr;
    }
}

bool MIDI_Source::disconnect(TrueMIDI_Sink &sink) {
    if (!hasSinkPipe())
        return false;
    return sinkPipe->disconnect(sink);
}

MIDI_Source::MIDI_Source(MIDI_Source &&other)
    : sinkPipe(std::exchange(other.sinkPipe, nullptr)) {
    if (this->hasSinkPipe()) {
        this->sinkPipe->disconnectSource();
        this->sinkPipe->connectSource(this);
    }
}

MIDI_Source &MIDI_Source::operator=(MIDI_Source &&other) {
    std::swap(this->sinkPipe, other.sinkPipe);
    if (this->hasSinkPipe()) {
        this->sinkPipe->disconnectSource();
        this->sinkPipe->connectSource(this);
    }
    if (other.hasSinkPipe()) {
        other.sinkPipe->disconnectSource();
        other.sinkPipe->connectSource(this);
    }
    return *this;
}

MIDI_Source::~MIDI_Source() { disconnectSinkPipes(); }

void MIDI_Source::sourceMIDItoPipe(ChannelMessage msg) {
    if (sinkPipe != nullptr) {
        assertNotStalled();
        sinkPipe->pipeMIDI(msg);
    }
}
void MIDI_Source::sourceMIDItoPipe(SysExMessage msg) {
    if (sinkPipe != nullptr) {
        assertNotStalled();
        sinkPipe->pipeMIDI(msg);
    }
}
void MIDI_Source::sourceMIDItoPipe(RealTimeMessage msg) {
    if (sinkPipe != nullptr) {
        sinkPipe->pipeMIDI(msg);
    }
}

void MIDI_Source::stall(MIDIStaller *cause) {
    if (hasSinkPipe())
        sinkPipe->stallDownstream(cause, this);
}

void MIDI_Source::unstall(MIDIStaller *cause) {
    if (hasSinkPipe())
        sinkPipe->unstallDownstream(cause, this);
}

bool MIDI_Source::isStalled() const {
    if (hasSinkPipe())
        return sinkPipe->isStalled();
    return false;
}

MIDIStaller *MIDI_Source::getStaller() const {
    if (hasSinkPipe())
        return sinkPipe->getStaller();
    return nullptr;
}

void MIDI_Source::assertNotStalled() const {
    if (isStalled()) {
        auto staller = sinkPipe->sink_staller ? sinkPipe->sink_staller
                                              : sinkPipe->through_staller;
        auto stallername = MIDIStaller::getNameNull(staller);
        FATAL_ERROR(F("MIDI source stalled by ") << stallername, 0x7281);
        (void)stallername;
    }
}

void MIDI_Source::handleStallers() const {
    if (hasSinkPipe())
        sinkPipe->handleStallers();
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
        sink->disconnectSourcePipesShallow();
        this->disconnectSourcePipesShallow(); // disconnect throughIn
        oldSink->connectSourcePipe(oldThroughIn);
    }
    if (hasSource() && hasThroughOut()) {
        auto oldSource = source;
        auto oldThroughOut = throughOut;
        source->disconnectSinkPipesShallow();
        this->disconnectSinkPipesShallow(); // disconnect throughOut
        oldSource->connectSinkPipe(oldThroughOut);
    }
    if (hasSink())
        sink->disconnectSourcePipesShallow();

    if (hasSource())
        source->disconnectSinkPipesShallow();

    if (hasThroughIn() || hasThroughOut())
        FATAL_ERROR(F("Invalid state"), 0x9147); // LCOV_EXCL_LINE
}

MIDI_Pipe::~MIDI_Pipe() { disconnect(); }

void MIDI_Pipe::stallDownstream(MIDIStaller *cause, MIDI_Source *stallsrc) {
    if (!sinkIsUnstalledOrStalledBy(cause)) {
        FATAL_ERROR(F("Cannot stall pipe from ")
                        << MIDIStaller::getNameNull(cause)
                        << F(" because pipe is already stalled by ")
                        << MIDIStaller::getNameNull(sink_staller),
                    0x6665);
    }
    sink_staller = cause;
    if (hasThroughOut() && stallsrc == source)
        throughOut->stallDownstream(cause, this);
    if (hasSink())
        sink->stallDownstream(cause, this);
    if (hasSource() && source != stallsrc) {
        // If our through output is stalled, that means our upstream is stalled
        // as well by this staller. Unstall it first, and replace it by the new
        // staller
        if (through_staller != nullptr)
            source->unstallUpstream(through_staller, this);
        source->stallUpstream(cause, this);
    }
    if (hasThroughIn() && throughIn != stallsrc)
        throughIn->stallUpstream(cause, this);
}

void MIDI_Pipe::stallUpstream(MIDIStaller *cause, MIDI_Sink *stallsrc) {
    if (stallsrc == sink) {
        if (!sinkIsUnstalledOrStalledBy(cause)) {
            FATAL_ERROR(F("Cannot stall pipe from ")
                            << MIDIStaller::getNameNull(cause)
                            << F(" because pipe is already stalled by ")
                            << MIDIStaller::getNameNull(sink_staller),
                        0x6666);
        }
        sink_staller = cause;
        if (hasSource())
            source->stallUpstream(cause, this);
        if (hasThroughIn())
            throughIn->stallUpstream(cause, this);
    } else {
        if (through_staller == nullptr) {
            through_staller = cause;
            if (hasSource())
                source->stallUpstream(cause, this);
        }
    }
}

void MIDI_Pipe::unstallDownstream(MIDIStaller *cause, MIDI_Source *stallsrc) {
    if (!sinkIsUnstalledOrStalledBy(cause)) {
        FATAL_ERROR(F("Cannot unstall pipe from ")
                        << MIDIStaller::getNameNull(cause)
                        << F(" because pipe is stalled by ")
                        << MIDIStaller::getNameNull(sink_staller),
                    0x6655);
    }
    this->sink_staller = nullptr;
    if (hasThroughOut() && stallsrc == source)
        throughOut->unstallDownstream(cause, this);
    if (hasSink())
        sink->unstallDownstream(cause, this);
    if (hasSource() && source != stallsrc) {
        source->unstallUpstream(cause, this);
        // If the through output of this pipe is stalled, we cannot just unstall
        // our upstream, we have to update it our through output staller
        if (through_staller != nullptr)
            source->stallUpstream(through_staller, this);
    }
    if (hasThroughIn() && throughIn != stallsrc)
        throughIn->unstallUpstream(cause, this);
}

void MIDI_Pipe::unstallUpstream(MIDIStaller *cause, MIDI_Sink *stallsrc) {
    if (stallsrc == sink) {
        if (cause != sink_staller) {
            FATAL_ERROR(F("Cannot unstall pipe from ")
                            << MIDIStaller::getNameNull(cause)
                            << F(" because pipe is stalled by ")
                            << MIDIStaller::getNameNull(sink_staller),
                        0x6656);
        }
        sink_staller = nullptr;
        if (hasSource())
            source->unstallUpstream(cause, this);
        if (hasThroughIn())
            throughIn->unstallUpstream(cause, this);
    } else {
        if (cause == through_staller) {
            through_staller = nullptr;
            if (hasSource())
                source->unstallUpstream(cause, this);
        }
    }
}

void MIDI_Pipe::handleStallers() const {
    if (!isStalled())
        return;
    if (sink_staller == eternal_stall || through_staller == eternal_stall)
        FATAL_ERROR(F("Unable to unstall pipe (eternal stall)"), 0x4827);
    while(isStalled()) {
        if (sink_staller)
            sink_staller->handleStall();
        if (through_staller)
            through_staller->handleStall();
    }
}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

const char *MIDIStaller::getNameNull(MIDIStaller *s) {
    if (s == nullptr)
        return "(null)";
    if (s == eternal_stall)
        return "(eternal stall)";
    return s->getName();
}

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()