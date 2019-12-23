#include <MIDI_Interfaces/MIDI_Pipes.hpp>
#include <gmock-wrapper.h>
#include <gtest-wrapper.h>

USING_CS_NAMESPACE;
using ::testing::StrictMock;

W_SUGGEST_OVERRIDE_OFF

struct MockMIDI_Sink : TrueMIDI_Sink {
    MOCK_METHOD(void, sinkMIDI, (ChannelMessage), (override));
    MOCK_METHOD(void, sinkMIDI, (SysExMessage), (override));
    MOCK_METHOD(void, sinkMIDI, (RealTimeMessage), (override));
};

W_SUGGEST_OVERRIDE_ON

W_SUGGEST_OVERRIDE_OFF

struct MockMIDI_SinkSource : TrueMIDI_SinkSource {
    MOCK_METHOD(void, sinkMIDI, (ChannelMessage), (override));
    MOCK_METHOD(void, sinkMIDI, (SysExMessage), (override));
    MOCK_METHOD(void, sinkMIDI, (RealTimeMessage), (override));
};

W_SUGGEST_OVERRIDE_ON

TEST(MIDI_Pipes, sourcePipeSink) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe;
    TrueMIDI_Source source;

    source >> pipe >> sink;

    RealTimeMessage msg = {0xFF, 3};
    EXPECT_CALL(sink, sinkMIDI(msg));
    source.sendMIDI(msg);
}

TEST(MIDI_Pipes, sourceX2PipeSink) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source1, source2;

    source1 >> pipe1 >> sink;
    source2 >> pipe2 >> sink;

    RealTimeMessage msg = {0xFF, 3};
    EXPECT_CALL(sink, sinkMIDI(msg));
    source1.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&sink);

    EXPECT_CALL(sink, sinkMIDI(msg));
    source2.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&sink);
}

TEST(MIDI_Pipes, sourceX2PipeSinkDisconnectPipe) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source1, source2;

    source1 >> pipe1 >> sink;
    source2 >> pipe2 >> sink;

    RealTimeMessage msg = {0xFF, 3};

    pipe1.disconnect();

    EXPECT_CALL(sink, sinkMIDI(msg));
    source2.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&sink);

    source1.sendMIDI(msg); // shouldn't crash
    ::testing::Mock::VerifyAndClear(&sink);
}

TEST(MIDI_Pipes, sourcePipeSinkX2) {
    StrictMock<MockMIDI_Sink> sink1, sink2;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source;

    source >> pipe1 >> sink1;
    source >> pipe2 >> sink2;

    RealTimeMessage msg = {0xFF, 3};

    EXPECT_CALL(sink1, sinkMIDI(msg));
    EXPECT_CALL(sink2, sinkMIDI(msg));
    source.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&sink1);
    ::testing::Mock::VerifyAndClear(&sink2);
}

TEST(MIDI_Pipes, sourcePipeSinkX2DisconnectPipe) {
    StrictMock<MockMIDI_Sink> sink1, sink2;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source;

    source >> pipe1 >> sink1;
    source >> pipe2 >> sink2;

    RealTimeMessage msg = {0xFF, 3};

    pipe1.disconnect();

    EXPECT_CALL(sink2, sinkMIDI(msg));
    source.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&sink1);
    ::testing::Mock::VerifyAndClear(&sink2);
}

TEST(MIDI_Pipes, sourceX2PipeSinkX2) {
    StrictMock<MockMIDI_Sink> sink1, sink2;
    MIDI_Pipe pipe1, pipe2, pipe3, pipe4;
    TrueMIDI_Source source1, source2;

    source1 >> pipe1 >> sink1;
    source1 >> pipe2 >> sink2;
    source2 >> pipe3 >> sink1;
    source2 >> pipe4 >> sink2;

    RealTimeMessage msg = {0xFF, 3};
    EXPECT_CALL(sink1, sinkMIDI(msg));
    EXPECT_CALL(sink2, sinkMIDI(msg));
    source1.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&sink1);
    ::testing::Mock::VerifyAndClear(&sink2);

    EXPECT_CALL(sink1, sinkMIDI(msg));
    EXPECT_CALL(sink2, sinkMIDI(msg));
    source2.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&sink1);
    ::testing::Mock::VerifyAndClear(&sink2);
}

TEST(MIDI_Pipes, sourcePipeSinkBidirectional) {
    struct MockMIDI_SinkSource : MockMIDI_Sink, TrueMIDI_Source {};

    StrictMock<MockMIDI_SinkSource> A, B;
    MIDI_Pipe pipe1, pipe2, pipe3, pipe4;

    A >> pipe1 >> B;
    A << pipe2 << B;

    RealTimeMessage msg = {0xFF, 3};

    EXPECT_CALL(B, sinkMIDI(msg));
    A.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);

    EXPECT_CALL(A, sinkMIDI(msg));
    B.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);
}

TEST(MIDI_Pipes, sourcePipeSinkBidirectionalBidirectional) {
    StrictMock<MockMIDI_SinkSource> A, B;
    BidirectionalMIDI_Pipe pipe;

    A | pipe | B;

    RealTimeMessage msg = {0xFF, 3};

    EXPECT_CALL(B, sinkMIDI(msg));
    A.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);

    EXPECT_CALL(A, sinkMIDI(msg));
    B.sendMIDI(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);
}
