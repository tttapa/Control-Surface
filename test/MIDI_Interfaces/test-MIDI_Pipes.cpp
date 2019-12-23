#include <MIDI_Interfaces/MIDI_Pipes.hpp>
#include <gmock-wrapper.h>
#include <gtest-wrapper.h>

USING_CS_NAMESPACE;
using ::testing::Return;
using ::testing::Sequence;

W_SUGGEST_OVERRIDE_OFF

struct MockMIDI_Sink : MIDI_Sink {
    MOCK_METHOD(void, sinkMIDI, (ChannelMessage), (override));
    MOCK_METHOD(void, sinkMIDI, (SysExMessage), (override));
    MOCK_METHOD(void, sinkMIDI, (RealTimeMessage), (override));
};

W_SUGGEST_OVERRIDE_ON

TEST(MIDI_Pipes, sourcePipeSink) {
    MockMIDI_Sink sink;
    MIDI_Pipe pipe;
    MIDI_Source source;

    source >> pipe >> sink;

    RealTimeMessage msg = {0xFF, 3};
    EXPECT_CALL(sink, sinkMIDI(msg));
    source.sendMIDI(msg);
}