#include <Settings/SettingsWrapper.hpp>
#if !DISABLE_PIPES

#include "MIDI_Pipes.hpp"
#include "MIDI_Staller.hpp"
#include <AH/Error/Error.hpp>
#include <AH/STL/utility>

#if defined(ESP32) || !defined(ARDUINO)
#include <mutex>
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

void MIDI_Sink::swap(MIDI_Sink &a, MIDI_Sink &b) {
    std::swap(a.sourcePipe, b.sourcePipe);
    if (a.hasSourcePipe()) {
        a.sourcePipe->disconnectSink();
        a.sourcePipe->connectSink(&a);
    }
    if (b.hasSourcePipe()) {
        b.sourcePipe->disconnectSink();
        b.sourcePipe->connectSink(&b);
    }
}

MIDI_Sink &MIDI_Sink::operator=(MIDI_Sink &&other) {
    swap(*this, other);
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

void MIDI_Source::swap(MIDI_Source &a, MIDI_Source &b) {
    std::swap(a.sinkPipe, b.sinkPipe);
    if (a.hasSinkPipe()) {
        a.sinkPipe->disconnectSource();
        a.sinkPipe->connectSource(&a);
    }
    if (b.hasSinkPipe()) {
        b.sinkPipe->disconnectSource();
        b.sinkPipe->connectSource(&b);
    }
}

MIDI_Source &MIDI_Source::operator=(MIDI_Source &&other) {
    swap(*this, other);
    return *this;
}

MIDI_Source::~MIDI_Source() { disconnectSinkPipes(); }

void MIDI_Source::sourceMIDItoPipe(ChannelMessage msg) {
    if (sinkPipe != nullptr) {
        handleStallers();
        sinkPipe->acceptMIDIfromSource(msg);
    }
}
void MIDI_Source::sourceMIDItoPipe(SysExMessage msg) {
    if (sinkPipe != nullptr) {
        handleStallers();
        sinkPipe->acceptMIDIfromSource(msg);
    }
}
void MIDI_Source::sourceMIDItoPipe(SysCommonMessage msg) {
    if (sinkPipe != nullptr) {
        handleStallers();
        sinkPipe->acceptMIDIfromSource(msg);
    }
}
void MIDI_Source::sourceMIDItoPipe(RealTimeMessage msg) {
    if (sinkPipe != nullptr) {
        // Always send write to pipe, don't check if it's stalled or not
        sinkPipe->acceptMIDIfromSource(msg);
    }
}

void MIDI_Source::stall(MIDIStaller *cause) {
    if (hasSinkPipe())
        sinkPipe->stallDownstream(cause, this);
    DEBUGFN(F("Stalled MIDI source. Cause: ") << getStallerName());
}

void MIDI_Source::unstall(MIDIStaller *cause) {
    DEBUGFN(F("Un-stalling MIDI source. Cause: ") << getStallerName());
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

const char *MIDI_Source::getStallerName() const {
    return MIDIStaller::getNameNull(getStaller());
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
        auto oldThroughIn = getThroughIn();
        sink->disconnectSourcePipesShallow();
        this->disconnectSourcePipesShallow(); // disconnect throughIn
        oldSink->connectSourcePipe(oldThroughIn);
    }
    if (hasSource() && hasThroughOut()) {
        auto oldSource = source;
        auto oldThroughOut = getThroughOut();
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
    } // LCOV_EXCL_LINE
    sink_staller = cause;
    if (hasThroughOut() && stallsrc == source)
        getThroughOut()->stallDownstream(cause, this);
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
    if (hasThroughIn() && getThroughIn() != stallsrc)
        getThroughIn()->stallUpstream(cause, this);
}

void MIDI_Pipe::stallUpstream(MIDIStaller *cause, MIDI_Sink *stallsrc) {
    if (stallsrc == sink) {
        // This cannot be a different cause, because then our sink would
        // already have caught it in stallDownstream().
        sink_staller = cause;
        if (hasSource())
            source->stallUpstream(cause, this);
        if (hasThroughIn())
            getThroughIn()->stallUpstream(cause, this);
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
                    0x6666);
    } // LCOV_EXCL_LINE
    this->sink_staller = nullptr;
    if (hasThroughOut() && stallsrc == source)
        getThroughOut()->unstallDownstream(cause, this);
    if (hasSink())
        sink->unstallDownstream(cause, this);
    if (hasSource() && source != stallsrc) {
        source->unstallUpstream(cause, this);
        // If the through output of this pipe is stalled, we cannot just unstall
        // our upstream, we have to update it our through output staller
        if (through_staller != nullptr)
            source->stallUpstream(through_staller, this);
    }
    if (hasThroughIn() && getThroughIn() != stallsrc)
        getThroughIn()->unstallUpstream(cause, this);
}

void MIDI_Pipe::unstallUpstream(MIDIStaller *cause, MIDI_Sink *stallsrc) {
    if (stallsrc == sink) {
        // This cannot be a different cause, because then our sink would
        // already have caught it in unstallDownstream().
        sink_staller = nullptr;
        if (hasSource())
            source->unstallUpstream(cause, this);
        if (hasThroughIn())
            getThroughIn()->unstallUpstream(cause, this);
    } else {
        if (cause == through_staller) {
            through_staller = nullptr;
            if (hasSource())
                source->unstallUpstream(cause, this);
        }
    }
}

const char *MIDI_Pipe::getSinkStallerName() const {
    return MIDIStaller::getNameNull(sink_staller);
}

const char *MIDI_Pipe::getThroughStallerName() const {
    return MIDIStaller::getNameNull(through_staller);
}

MIDIStaller *MIDI_Pipe::getStaller() const {
    return sink_staller ? sink_staller : through_staller;
}

const char *MIDI_Pipe::getStallerName() const {
    return MIDIStaller::getNameNull(getStaller());
}

void MIDI_Pipe::handleStallers() const {
    if (!isStalled())
        return;
    if (sink_staller == eternal_stall || through_staller == eternal_stall)
        FATAL_ERROR(F("Unable to unstall pipe (eternal stall)"), 0x4827);
    uint8_t iterations = 10;
    while (isStalled() && iterations-- > 0) {
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

#endif
