#include <MIDI_Interfaces/MIDI_Pipes.hpp>
#include <gmock-wrapper.h>
#include <gtest-wrapper.h>

USING_CS_NAMESPACE;
using ::testing::StrictMock;

struct MockMIDI_Sink : TrueMIDI_Sink {
    MOCK_METHOD(void, sinkMIDIfromPipe, (ChannelMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (SysExMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (RealTimeMessage), (override));
};

struct MockMIDI_SinkSource : TrueMIDI_SinkSource {
    MOCK_METHOD(void, sinkMIDIfromPipe, (ChannelMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (SysExMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (RealTimeMessage), (override));
};

TEST(MIDI_Pipes, sourcePipeSink) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe;
    TrueMIDI_Source source;

    source >> pipe >> sink;

    RealTimeMessage msg = {0xFF, 3};
    EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
    source.sourceMIDItoPipe(msg);
}

TEST(MIDI_Pipes, sourceX2PipeSink) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source1, source2;

    source1 >> pipe1 >> sink;
    source2 >> pipe2 >> sink;

    RealTimeMessage msg = {0xFF, 3};
    EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
    source1.sourceMIDItoPipe(msg);
    ::testing::Mock::VerifyAndClear(&sink);

    EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
    source2.sourceMIDItoPipe(msg);
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

    EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
    source2.sourceMIDItoPipe(msg);
    ::testing::Mock::VerifyAndClear(&sink);

    source1.sourceMIDItoPipe(msg); // shouldn't crash
    ::testing::Mock::VerifyAndClear(&sink);
}

TEST(MIDI_Pipes, sourcePipeSinkX2) {
    StrictMock<MockMIDI_Sink> sink1, sink2;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source;

    source >> pipe1 >> sink1;
    source >> pipe2 >> sink2;

    RealTimeMessage msg = {0xFF, 3};

    EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
    EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
    source.sourceMIDItoPipe(msg);
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

    EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
    source.sourceMIDItoPipe(msg);
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
    EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
    EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
    source1.sourceMIDItoPipe(msg);
    ::testing::Mock::VerifyAndClear(&sink1);
    ::testing::Mock::VerifyAndClear(&sink2);

    EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
    EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
    source2.sourceMIDItoPipe(msg);
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

    EXPECT_CALL(B, sinkMIDIfromPipe(msg));
    A.sourceMIDItoPipe(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);

    EXPECT_CALL(A, sinkMIDIfromPipe(msg));
    B.sourceMIDItoPipe(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);
}

TEST(MIDI_Pipes, sourcePipeSinkBidirectionalBidirectional) {
    StrictMock<MockMIDI_SinkSource> A, B;
    BidirectionalMIDI_Pipe pipe;

    A | pipe | B;

    RealTimeMessage msg = {0xFF, 3};

    EXPECT_CALL(B, sinkMIDIfromPipe(msg));
    A.sourceMIDItoPipe(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);

    EXPECT_CALL(A, sinkMIDIfromPipe(msg));
    B.sourceMIDItoPipe(msg);
    ::testing::Mock::VerifyAndClear(&A);
    ::testing::Mock::VerifyAndClear(&B);
}

TEST(MIDI_Pipes, checkConnections) {
    StrictMock<MockMIDI_Sink> sink1;
    StrictMock<MockMIDI_Sink> sink2;
    MIDI_Pipe pipe1;
    MIDI_Pipe pipe2;
    MIDI_Pipe pipe3;
    MIDI_Pipe pipe4;
    TrueMIDI_Source source1;
    TrueMIDI_Source source2;

    source1 >> pipe1 >> sink1;
    source1 >> pipe2 >> sink2;
    source2 >> pipe3 >> sink1;
    source2 >> pipe4 >> sink2;

    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_TRUE(pipe3.hasSink());
    EXPECT_TRUE(pipe3.hasSource());
    EXPECT_TRUE(pipe4.hasSink());
    EXPECT_TRUE(pipe4.hasSource());

    EXPECT_TRUE(pipe1.hasThroughIn());
    EXPECT_TRUE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe3.hasThroughIn());
    EXPECT_TRUE(pipe3.hasThroughOut());
    EXPECT_FALSE(pipe4.hasThroughIn());
    EXPECT_FALSE(pipe4.hasThroughOut());
}

TEST(MIDI_Pipes, disconnectPipe1) {
    StrictMock<MockMIDI_Sink> sink1;
    StrictMock<MockMIDI_Sink> sink2;
    MIDI_Pipe pipe1;
    MIDI_Pipe pipe2;
    MIDI_Pipe pipe3;
    MIDI_Pipe pipe4;
    TrueMIDI_Source source1;
    TrueMIDI_Source source2;

    source1 >> pipe1 >> sink1;
    source1 >> pipe2 >> sink2;
    source2 >> pipe3 >> sink1;
    source2 >> pipe4 >> sink2;

    pipe1.disconnect();

    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_FALSE(pipe1.hasSink());
    EXPECT_FALSE(pipe1.hasSource());
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_TRUE(pipe3.hasSink());
    EXPECT_TRUE(pipe3.hasSource());
    EXPECT_TRUE(pipe4.hasSink());
    EXPECT_TRUE(pipe4.hasSource());

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe3.hasThroughIn());
    EXPECT_TRUE(pipe3.hasThroughOut());
    EXPECT_FALSE(pipe4.hasThroughIn());
    EXPECT_FALSE(pipe4.hasThroughOut());
}

TEST(MIDI_Pipes, disconnectPipe12) {
    StrictMock<MockMIDI_Sink> sink1;
    StrictMock<MockMIDI_Sink> sink2;
    MIDI_Pipe pipe1;
    MIDI_Pipe pipe2;
    MIDI_Pipe pipe3;
    MIDI_Pipe pipe4;
    TrueMIDI_Source source1;
    TrueMIDI_Source source2;

    source1 >> pipe1 >> sink1;
    source1 >> pipe2 >> sink2;
    source2 >> pipe3 >> sink1;
    source2 >> pipe4 >> sink2;

    pipe1.disconnect();
    pipe2.disconnect();

    EXPECT_FALSE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_FALSE(pipe1.hasSink());
    EXPECT_FALSE(pipe1.hasSource());
    EXPECT_FALSE(pipe2.hasSink());
    EXPECT_FALSE(pipe2.hasSource());
    EXPECT_TRUE(pipe3.hasSink());
    EXPECT_TRUE(pipe3.hasSource());
    EXPECT_TRUE(pipe4.hasSink());
    EXPECT_TRUE(pipe4.hasSource());

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe3.hasThroughIn());
    EXPECT_TRUE(pipe3.hasThroughOut());
    EXPECT_FALSE(pipe4.hasThroughIn());
    EXPECT_FALSE(pipe4.hasThroughOut());
}

TEST(MIDI_Pipes, disconnectPipe123) {
    StrictMock<MockMIDI_Sink> sink1;
    StrictMock<MockMIDI_Sink> sink2;
    MIDI_Pipe pipe1;
    MIDI_Pipe pipe2;
    MIDI_Pipe pipe3;
    MIDI_Pipe pipe4;
    TrueMIDI_Source source1;
    TrueMIDI_Source source2;

    source1 >> pipe1 >> sink1;
    source1 >> pipe2 >> sink2;
    source2 >> pipe3 >> sink1;
    source2 >> pipe4 >> sink2;

    pipe1.disconnect();
    pipe2.disconnect();
    pipe3.disconnect();

    EXPECT_FALSE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_FALSE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_FALSE(pipe1.hasSink());
    EXPECT_FALSE(pipe1.hasSource());
    EXPECT_FALSE(pipe2.hasSink());
    EXPECT_FALSE(pipe2.hasSource());
    EXPECT_FALSE(pipe3.hasSink());
    EXPECT_FALSE(pipe3.hasSource());
    EXPECT_TRUE(pipe4.hasSink());
    EXPECT_TRUE(pipe4.hasSource());

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe3.hasThroughIn());
    EXPECT_FALSE(pipe3.hasThroughOut());
    EXPECT_FALSE(pipe4.hasThroughIn());
    EXPECT_FALSE(pipe4.hasThroughOut());
}

TEST(MIDI_Pipes, disconnectSource1) {
    StrictMock<MockMIDI_Sink> sink1;
    StrictMock<MockMIDI_Sink> sink2;
    MIDI_Pipe pipe1;
    MIDI_Pipe pipe2;
    MIDI_Pipe pipe3;
    MIDI_Pipe pipe4;
    TrueMIDI_Source source1;
    TrueMIDI_Source source2;

    source1 >> pipe1 >> sink1;
    source1 >> pipe2 >> sink2;
    source2 >> pipe3 >> sink1;
    source2 >> pipe4 >> sink2;

    source1.disconnectSinkPipe();

    EXPECT_FALSE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_FALSE(pipe1.hasSource());
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_FALSE(pipe2.hasSource());
    EXPECT_TRUE(pipe3.hasSink());
    EXPECT_TRUE(pipe3.hasSource());
    EXPECT_TRUE(pipe4.hasSink());
    EXPECT_TRUE(pipe4.hasSource());

    EXPECT_TRUE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe3.hasThroughIn());
    EXPECT_TRUE(pipe3.hasThroughOut());
    EXPECT_FALSE(pipe4.hasThroughIn());
    EXPECT_FALSE(pipe4.hasThroughOut());
}

TEST(MIDI_Pipes, disconnectSink1) {
    StrictMock<MockMIDI_Sink> sink1;
    StrictMock<MockMIDI_Sink> sink2;
    MIDI_Pipe pipe1;
    MIDI_Pipe pipe2;
    MIDI_Pipe pipe3;
    MIDI_Pipe pipe4;
    TrueMIDI_Source source1;
    TrueMIDI_Source source2;

    source1 >> pipe1 >> sink1;
    source1 >> pipe2 >> sink2;
    source2 >> pipe3 >> sink1;
    source2 >> pipe4 >> sink2;

    sink1.disconnectSourcePipe();

    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_FALSE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_FALSE(pipe1.hasSink());
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_FALSE(pipe3.hasSink());
    EXPECT_TRUE(pipe3.hasSource());
    EXPECT_TRUE(pipe4.hasSink());
    EXPECT_TRUE(pipe4.hasSource());

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_TRUE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe3.hasThroughIn());
    EXPECT_TRUE(pipe3.hasThroughOut());
    EXPECT_FALSE(pipe4.hasThroughIn());
    EXPECT_FALSE(pipe4.hasThroughOut());
}