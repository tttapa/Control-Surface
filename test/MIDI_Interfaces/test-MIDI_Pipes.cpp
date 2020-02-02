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

struct DummyMIDI_Sink : TrueMIDI_Sink {
    void sinkMIDIfromPipe(ChannelMessage) override {}
    void sinkMIDIfromPipe(SysExMessage) override {}
    void sinkMIDIfromPipe(RealTimeMessage) override {}
};

TEST(MIDI_Pipes, sourcePipeSink) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe;
    TrueMIDI_Source source;

    source >> pipe >> sink;

    {
        RealTimeMessage msg = {0xFF, 3};
        EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink);
    }
    {
        ChannelMessage msg{0x93, 0x10, 0x7F, 5};
        EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink);
    }
    {
        SysExMessage msg = {nullptr, 0, 10};
        EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink);
    }
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

    {
        RealTimeMessage msg = {0xFF, 3};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        ChannelMessage msg{0x93, 0x10, 0x7F, 5};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        SysExMessage msg = {nullptr, 0, 10};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
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

    source1.disconnectSinkPipes();

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

    sink1.disconnectSourcePipes();

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

TEST(MIDI_Pipes, checkConnectionsMoveSink) {
    DummyMIDI_Sink sink1;
    MIDI_Pipe pipe;
    TrueMIDI_Source source;

    source >> pipe >> sink1;

    EXPECT_TRUE(source.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());

    EXPECT_TRUE(pipe.hasSink());
    EXPECT_EQ(pipe.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe);
    EXPECT_TRUE(pipe.hasSource());
    EXPECT_EQ(pipe.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe(), &pipe);

    EXPECT_FALSE(pipe.hasThroughIn());
    EXPECT_FALSE(pipe.hasThroughOut());

    DummyMIDI_Sink sink2 = std::move(sink1);

    EXPECT_FALSE(sink1.hasSourcePipe());

    EXPECT_TRUE(source.hasSinkPipe());
    EXPECT_TRUE(sink2.hasSourcePipe());
    EXPECT_FALSE(sink1.hasSourcePipe());

    EXPECT_TRUE(pipe.hasSink());
    EXPECT_EQ(pipe.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe);
    EXPECT_EQ(sink1.getSourcePipe(), nullptr);
    EXPECT_TRUE(pipe.hasSource());
    EXPECT_EQ(pipe.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe(), &pipe);

    EXPECT_FALSE(pipe.hasThroughIn());
    EXPECT_FALSE(pipe.hasThroughOut());
}

TEST(MIDI_Pipes, checkConnectionsMoveSink2Sources) {
    DummyMIDI_Sink sink1;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source1, source2;

    source1 >> pipe1 >> sink1;
    source2 >> pipe2 >> sink1;

    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);

    EXPECT_TRUE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());

    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe2);
    EXPECT_EQ(sink1.getSourcePipe()->getThroughIn(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());

    DummyMIDI_Sink sink2 = std::move(sink1);

    EXPECT_FALSE(sink1.hasSourcePipe());

    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);

    EXPECT_TRUE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());

    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
}

TEST(MIDI_Pipes, checkConnectionsMoveSink2Sinks) {
    DummyMIDI_Sink sink1, sink2;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source;

    source >> pipe1 >> sink1;
    source >> pipe2 >> sink2;

    EXPECT_TRUE(source.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_TRUE(pipe1.hasThroughOut());

    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe()->getThroughOut(), &pipe2);

    DummyMIDI_Sink sink3 = std::move(sink1);

    EXPECT_FALSE(sink1.hasSourcePipe());
    
    EXPECT_TRUE(source.hasSinkPipe());
    EXPECT_TRUE(sink3.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink3);
    EXPECT_EQ(sink3.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_TRUE(pipe1.hasThroughOut());

    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe()->getThroughOut(), &pipe2);

    DummyMIDI_Sink sink4 = std::move(sink2);

    EXPECT_FALSE(sink2.hasSourcePipe());
    
    EXPECT_TRUE(source.hasSinkPipe());
    EXPECT_TRUE(sink3.hasSourcePipe());
    EXPECT_TRUE(sink4.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink3);
    EXPECT_EQ(sink3.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_TRUE(pipe1.hasThroughOut());

    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink4);
    EXPECT_EQ(sink4.getSourcePipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source);
    EXPECT_EQ(source.getSinkPipe()->getThroughOut(), &pipe2);
}

TEST(MIDI_Pipes, checkConnectionsSwapSink) {
    DummyMIDI_Sink sink1, sink2;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source1, source2;

    source1 >> pipe1 >> sink1;
    source2 >> pipe2 >> sink2;

    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());

    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());

    std::swap(sink1, sink2);

    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(sink2.hasSourcePipe());

    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());

    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_TRUE(sink1.hasSourcePipe());

    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
}

TEST(MIDI_Pipes, checkConnectionsMoveSource) {
    TrueMIDI_Source source1;
    MIDI_Pipe pipe;
    DummyMIDI_Sink sink;

    sink << pipe << source1;

    EXPECT_TRUE(sink.hasSourcePipe());
    EXPECT_TRUE(source1.hasSinkPipe());

    EXPECT_TRUE(pipe.hasSource());
    EXPECT_EQ(pipe.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe);
    EXPECT_TRUE(pipe.hasSink());
    EXPECT_EQ(pipe.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe(), &pipe);

    EXPECT_FALSE(pipe.hasThroughIn());
    EXPECT_FALSE(pipe.hasThroughOut());

    TrueMIDI_Source source2 = std::move(source1);

    EXPECT_TRUE(sink.hasSourcePipe());
    EXPECT_TRUE(source2.hasSinkPipe());
    EXPECT_FALSE(source1.hasSinkPipe());

    EXPECT_TRUE(pipe.hasSource());
    EXPECT_EQ(pipe.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe);
    EXPECT_EQ(source1.getSinkPipe(), nullptr);
    EXPECT_TRUE(pipe.hasSink());
    EXPECT_EQ(pipe.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe(), &pipe);

    EXPECT_FALSE(pipe.hasThroughIn());
    EXPECT_FALSE(pipe.hasThroughOut());
}

TEST(MIDI_Pipes, checkConnectionsMoveSource2Sinks) {
    TrueMIDI_Source source1;
    MIDI_Pipe pipe1, pipe2;
    DummyMIDI_Sink sink1, sink2;

    sink1 << pipe1 << source1;
    sink2 << pipe2 << source1;

    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(sink2.hasSourcePipe());
    EXPECT_TRUE(source1.hasSinkPipe());

    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);

    EXPECT_TRUE(pipe1.hasThroughOut());
    EXPECT_FALSE(pipe1.hasThroughIn());

    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);
    EXPECT_EQ(source1.getSinkPipe()->getThroughOut(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe2.hasThroughIn());

    TrueMIDI_Source source2 = std::move(source1);

    EXPECT_FALSE(source1.hasSinkPipe());

    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(source2.hasSinkPipe());

    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);

    EXPECT_TRUE(pipe1.hasThroughOut());
    EXPECT_FALSE(pipe1.hasThroughIn());

    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe2.hasThroughIn());
}

TEST(MIDI_Pipes, checkConnectionsMoveSource2Sources) {
    TrueMIDI_Source source1, source2;
    MIDI_Pipe pipe1, pipe2;
    DummyMIDI_Sink sink;

    sink << pipe1 << source1;
    sink << pipe2 << source2;

    EXPECT_TRUE(sink.hasSourcePipe());
    EXPECT_TRUE(source1.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());

    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe1.hasThroughIn());

    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe()->getThroughIn(), &pipe2);

    TrueMIDI_Source source3 = std::move(source1);

    EXPECT_FALSE(source1.hasSinkPipe());
    
    EXPECT_TRUE(sink.hasSourcePipe());
    EXPECT_TRUE(source3.hasSinkPipe());
    EXPECT_TRUE(source2.hasSinkPipe());

    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source3);
    EXPECT_EQ(source3.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe1.hasThroughIn());

    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe()->getThroughIn(), &pipe2);

    TrueMIDI_Source source4 = std::move(source2);

    EXPECT_FALSE(source2.hasSinkPipe());
    
    EXPECT_TRUE(sink.hasSourcePipe());
    EXPECT_TRUE(source3.hasSinkPipe());
    EXPECT_TRUE(source4.hasSinkPipe());

    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source3);
    EXPECT_EQ(source3.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe1.hasThroughIn());

    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source4);
    EXPECT_EQ(source4.getSinkPipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink);
    EXPECT_EQ(sink.getSourcePipe()->getThroughIn(), &pipe2);
}

TEST(MIDI_Pipes, checkConnectionsSwapSource) {
    TrueMIDI_Source source1, source2;
    MIDI_Pipe pipe1, pipe2;
    DummyMIDI_Sink sink1, sink2;

    sink1 << pipe1 << source1;
    sink2 << pipe2 << source2;

    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(source1.hasSinkPipe());

    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());

    EXPECT_TRUE(sink2.hasSourcePipe());
    EXPECT_TRUE(source2.hasSinkPipe());

    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());

    std::swap(source1, source2);

    EXPECT_TRUE(sink1.hasSourcePipe());
    EXPECT_TRUE(source2.hasSinkPipe());

    EXPECT_TRUE(pipe1.hasSource());
    EXPECT_EQ(pipe1.getInitialSource(), &source2);
    EXPECT_EQ(source2.getSinkPipe(), &pipe1);
    EXPECT_TRUE(pipe1.hasSink());
    EXPECT_EQ(pipe1.getFinalSink(), &sink1);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe1);

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());

    EXPECT_TRUE(sink2.hasSourcePipe());
    EXPECT_TRUE(source1.hasSinkPipe());

    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_EQ(pipe2.getInitialSource(), &source1);
    EXPECT_EQ(source1.getSinkPipe(), &pipe2);
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_EQ(pipe2.getFinalSink(), &sink2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);

    EXPECT_FALSE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
}

TEST(MIDI_Pipes, exclusive) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_TRUE(sources[1].try_lock_mutex(0xC));
    sources[1].unlock_mutex(0xC);
    ASSERT_TRUE(sources[2].try_lock_mutex(0xC));
    sources[2].unlock_mutex(0xC);
    ASSERT_TRUE(sources[3].try_lock_mutex(0xC));
    sources[3].unlock_mutex(0xC);

    ASSERT_TRUE(sources[3].try_lock_mutex(0xC));
    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_FALSE(sources[1].try_lock_mutex(0xC));
    ASSERT_FALSE(sources[2].try_lock_mutex(0xC));
    sources[3].unlock_mutex(0xC);

    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_TRUE(sources[1].try_lock_mutex(0xC));
    sources[1].unlock_mutex(0xC);
    ASSERT_TRUE(sources[2].try_lock_mutex(0xC));
    sources[2].unlock_mutex(0xC);
    ASSERT_TRUE(sources[3].try_lock_mutex(0xC));
    sources[3].unlock_mutex(0xC);

    ASSERT_TRUE(sources[2].try_lock_mutex(0xC));
    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_FALSE(sources[1].try_lock_mutex(0xC));
    ASSERT_FALSE(sources[3].try_lock_mutex(0xC));
    sources[2].unlock_mutex(0xC);

    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_TRUE(sources[1].try_lock_mutex(0xC));
    sources[1].unlock_mutex(0xC);
    ASSERT_TRUE(sources[2].try_lock_mutex(0xC));
    sources[2].unlock_mutex(0xC);
    ASSERT_TRUE(sources[3].try_lock_mutex(0xC));
    sources[3].unlock_mutex(0xC);

    ASSERT_TRUE(sources[1].try_lock_mutex(0xC));
    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_FALSE(sources[2].try_lock_mutex(0xC));
    ASSERT_FALSE(sources[3].try_lock_mutex(0xC));
    sources[1].unlock_mutex(0xC);

    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_TRUE(sources[1].try_lock_mutex(0xC));
    sources[1].unlock_mutex(0xC);
    ASSERT_TRUE(sources[2].try_lock_mutex(0xC));
    sources[2].unlock_mutex(0xC);
    ASSERT_TRUE(sources[3].try_lock_mutex(0xC));
    sources[3].unlock_mutex(0xC);

    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    ASSERT_FALSE(sources[1].try_lock_mutex(0xC));
    ASSERT_TRUE(sources[2].try_lock_mutex(0xC));
    sources[2].unlock_mutex(0xC);
    ASSERT_TRUE(sources[3].try_lock_mutex(0xC));
    sources[3].unlock_mutex(0xC);

    sources[0].unlock_mutex(0xC);
    ASSERT_TRUE(sources[0].try_lock_mutex(0xC));
    sources[0].unlock_mutex(0xC);
    ASSERT_TRUE(sources[1].try_lock_mutex(0xC));
    sources[1].unlock_mutex(0xC);
    ASSERT_TRUE(sources[2].try_lock_mutex(0xC));
    sources[2].unlock_mutex(0xC);
    ASSERT_TRUE(sources[3].try_lock_mutex(0xC));
    sources[3].unlock_mutex(0xC);
}

TEST(MIDI_PipeFactory, notEnoughPipes) {
    StrictMock<MockMIDI_Sink> sinks[3];
    MIDI_PipeFactory<2> pipes;

    pipes >> sinks[0];
    pipes >> sinks[1];
    try {
        pipes >> sinks[2];
        FAIL();
    } catch (AH::ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x2459);
    }
}

#include <AH/Error/Error.hpp>

TEST(MIDI_Pipes, connectPipeToSinkTwice) {
    StrictMock<MockMIDI_Sink> sink1, sink2;
    MIDI_Pipe pipe;

    pipe >> sink1;

    try {
        pipe >> sink2;
        FAIL();
    } catch (AH::ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x9145);
    }
}

TEST(MIDI_Pipes, connectPipeToSourceTwice) {
    TrueMIDI_Source source1, source2;
    MIDI_Pipe pipe;

    source1 >> pipe;

    try {
        source2 >> pipe;
        FAIL();
    } catch (AH::ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x9146);
    }
}

#include <MIDI_Interfaces/USBMIDI_Interface.hpp>
#include <MockMIDI_Interface.hpp>

using ::testing::Return;

TEST(MIDI_Pipes, USBInterface) {
    StrictMock<USBMIDI_Interface> midiA[2];
    StrictMock<MockMIDI_Interface> midiB[2];

    BidirectionalMIDI_Pipe pipe1;
    MIDI_Pipe pipe2, pipe3;

    midiA[0] | pipe1 | midiB[0];
    midiA[1] >> pipe2 >> midiB[0];
    midiA[1] >> pipe3 >> midiB[1];

    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midiA[0], readUSBPacket())
        .WillOnce(Return(Packet_t{0x54, 0xF0, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{0x54, 0x77, 0x11, 0x22}))
        .WillOnce(Return(Packet_t{0x56, 0x33, 0xF7, 0x00}));
    const uint8_t *sysexData =
        midiA[0].getSysExMessage().data + 5 * sizeof(SysExBuffer);
    //                                    ^~~~ CN
    EXPECT_CALL(midiB[0], sendImpl(sysexData, 8, 5));
    midiA[0].update();

    EXPECT_CALL(midiA[1], readUSBPacket())
        .WillOnce(Return(Packet_t{0x94, 0xF0, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{0x94, 0x77, 0x11, 0x22}))
        .WillOnce(Return(Packet_t{0x95, 0xF7, 0x00, 0x00}));
    sysexData = midiA[1].getSysExMessage().data + 9 * sizeof(SysExBuffer);
    //                                            ^~~~ CN
    EXPECT_CALL(midiB[0], sendImpl(sysexData, 7, 9));
    EXPECT_CALL(midiB[1], sendImpl(sysexData, 7, 9));
    midiA[1].update();
}

TEST(MIDI_Pipes, USBInterfaceLockSysEx) {
    StrictMock<USBMIDI_Interface> midiA[2];
    StrictMock<MockMIDI_Interface> midiB[2];

    BidirectionalMIDI_Pipe pipe1;
    MIDI_Pipe pipe2, pipe3;

    midiA[0] | pipe1 | midiB[0];
    midiA[1] >> pipe2 >> midiB[0];
    midiA[1] >> pipe3 >> midiB[1];

    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midiA[1], readUSBPacket())
        .WillOnce(Return(Packet_t{0x94, 0xF0, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{0x94, 0x77, 0x11, 0x22}))
        .WillOnce(Return(Packet_t{0x95, 0xF7, 0x00, 0x00}));

    // lock pipes of all MIDI interfaces that pipe to the same sinks as midiA[0]
    // (i.e. midiA[1]) so that midiA[0] has exclusive access.
    ASSERT_TRUE(midiA[0].try_lock_mutex(9));
    // shouldn't send anything, sink pipe is locked
    midiA[1].update();

    ::testing::Mock::VerifyAndClear(&midiB[0]);
    ::testing::Mock::VerifyAndClear(&midiB[1]);

    // unlock the pipes
    midiA[0].unlock_mutex(9);

    // message is already in buffer, CN is already 9, so getSysExMessage()
    // returns 9th buffer
    const uint8_t *sysexData = midiA[1].getSysExMessage().data;
    EXPECT_CALL(midiB[0], sendImpl(sysexData, 7, 9));
    EXPECT_CALL(midiB[1], sendImpl(sysexData, 7, 9));
    midiA[1].update(); // should send old message now
}

TEST(MIDI_Pipes, USBInterfaceLockChannelMessage) {
    StrictMock<USBMIDI_Interface> midiA[2];
    StrictMock<MockMIDI_Interface> midiB[2];

    BidirectionalMIDI_Pipe pipe1;
    MIDI_Pipe pipe2, pipe3;

    midiA[0] | pipe1 | midiB[0];
    midiA[1] >> pipe2 >> midiB[0];
    midiA[1] >> pipe3 >> midiB[1];

    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midiA[1], readUSBPacket())
        .WillOnce(Return(Packet_t{0x99, 0x95, 0x55, 0x66}));

    // lock pipes of all MIDI interfaces that pipe to the same sinks as midiA[0]
    // (i.e. midiA[1]) so that midiA[0] has exclusive access.
    ASSERT_TRUE(midiA[0].try_lock_mutex(9));
    // shouldn't send anything, sink pipe is locked
    midiA[1].update();

    ::testing::Mock::VerifyAndClear(&midiB[0]);
    ::testing::Mock::VerifyAndClear(&midiB[1]);

    // unlock the pipes
    midiA[0].unlock_mutex(9);

    EXPECT_CALL(midiB[0], sendImpl(0x90, 0x05, 0x55, 0x66, 0x9));
    EXPECT_CALL(midiB[1], sendImpl(0x90, 0x05, 0x55, 0x66, 0x9));
    midiA[1].update(); // should send old message now
}

TEST(MIDI_Pipes, USBInterfaceLoopBack) {
    StrictMock<USBMIDI_Interface> midi;

    MIDI_Pipe pipe;

    midi >> pipe >> midi;

    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midi, readUSBPacket())
        .WillOnce(Return(Packet_t{0x99, 0x95, 0x55, 0x66}));

    EXPECT_CALL(midi, writeUSBPacket(0x9, 0x9, 0x95, 0x55, 0x66));
    midi.update();
}

TEST(MIDI_Pipes, USBInterfaceLockRealTime) {
    StrictMock<USBMIDI_Interface> midiA[2];
    StrictMock<MockMIDI_Interface> midiB[2];

    BidirectionalMIDI_Pipe pipe1;
    MIDI_Pipe pipe2, pipe3;

    midiA[0] | pipe1 | midiB[0];
    midiA[1] >> pipe2 >> midiB[0];
    midiA[1] >> pipe3 >> midiB[1];

    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midiA[1], readUSBPacket())
        .WillOnce(Return(Packet_t{0x9F, 0xF8, 0x00, 0x00}))
        .WillOnce(Return(Packet_t{}));

    ASSERT_TRUE(midiA[0].try_lock_mutex(9));

    // even though pipe is locked, real-time message should get through
    EXPECT_CALL(midiB[0], sendImpl(0xF8, 0x9));
    EXPECT_CALL(midiB[1], sendImpl(0xF8, 0x9));

    midiA[1].update();

    ::testing::Mock::VerifyAndClear(&midiB[0]);
    ::testing::Mock::VerifyAndClear(&midiB[1]);

    // unlock the pipes
    midiA[0].unlock_mutex(9);
    midiA[1].update(); // shouldn't send anything
}

TEST(MIDI_Pipes, disconnectSourceFromSink) {
    {
        StrictMock<MockMIDI_Sink> sinks[2];
        MIDI_PipeFactory<5> pipes;
        TrueMIDI_Source sources[4];

        sources[1] >> pipes >> sinks[1];
        sources[1] >> pipes >> sinks[0];

        sources[0] >> pipes >> sinks[0];

        sources[2] >> pipes >> sinks[1];
        sources[3] >> pipes >> sinks[1];

        ASSERT_FALSE(sources[3].disconnect(sinks[0]));
        ASSERT_TRUE(sources[3].disconnect(sinks[1]));
        ASSERT_FALSE(sources[3].disconnect(sinks[1]));
        ASSERT_FALSE(sources[3].hasSinkPipe());
        ASSERT_EQ(sources[1].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[1].getSinkPipe()->getThroughOut()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[0].getSinkPipe()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[2].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[3].getSinkPipe(), //
                  nullptr);
        ASSERT_EQ(pipes[4].getFinalSink(), //
                  nullptr);
    }
    {
        StrictMock<MockMIDI_Sink> sinks[2];
        MIDI_PipeFactory<5> pipes;
        TrueMIDI_Source sources[4];

        sources[1] >> pipes >> sinks[1];
        sources[1] >> pipes >> sinks[0];

        sources[0] >> pipes >> sinks[0];

        sources[2] >> pipes >> sinks[1];
        sources[3] >> pipes >> sinks[1];

        ASSERT_FALSE(sources[2].disconnect(sinks[0]));
        ASSERT_TRUE(sources[2].disconnect(sinks[1]));
        ASSERT_FALSE(sources[2].disconnect(sinks[1]));
        ASSERT_FALSE(sources[2].hasSinkPipe());
        ASSERT_EQ(sources[1].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[1].getSinkPipe()->getThroughOut()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[0].getSinkPipe()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[3].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[2].getSinkPipe(), //
                  nullptr);
        ASSERT_EQ(pipes[3].getFinalSink(), //
                  nullptr);
    }
    {
        StrictMock<MockMIDI_Sink> sinks[2];
        MIDI_PipeFactory<5> pipes;
        TrueMIDI_Source sources[4];

        sources[1] >> pipes >> sinks[1];
        sources[1] >> pipes >> sinks[0];

        sources[0] >> pipes >> sinks[0];

        sources[2] >> pipes >> sinks[1];
        sources[3] >> pipes >> sinks[1];

        ASSERT_TRUE(sources[1].disconnect(sinks[1]));
        ASSERT_FALSE(sources[1].disconnect(sinks[1]));
        ASSERT_TRUE(sources[1].hasSinkPipe());
        ASSERT_EQ(sources[1].getSinkPipe()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[0].getSinkPipe()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[3].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[2].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(pipes[0].getFinalSink(), //
                  nullptr);
    }
    {
        StrictMock<MockMIDI_Sink> sinks[2];
        MIDI_PipeFactory<5> pipes;
        TrueMIDI_Source sources[4];

        sources[1] >> pipes >> sinks[1];
        sources[1] >> pipes >> sinks[0];

        sources[0] >> pipes >> sinks[0];

        sources[2] >> pipes >> sinks[1];
        sources[3] >> pipes >> sinks[1];

        ASSERT_TRUE(sources[1].disconnect(sinks[0]));
        ASSERT_FALSE(sources[1].disconnect(sinks[0]));
        ASSERT_TRUE(sources[1].hasSinkPipe());
        ASSERT_EQ(sources[1].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[0].getSinkPipe()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[3].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[2].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(pipes[1].getFinalSink(), //
                  nullptr);
    }
    {
        StrictMock<MockMIDI_Sink> sinks[2];
        MIDI_PipeFactory<5> pipes;
        TrueMIDI_Source sources[4];

        sources[1] >> pipes >> sinks[1];
        sources[1] >> pipes >> sinks[0];

        sources[0] >> pipes >> sinks[0];

        sources[2] >> pipes >> sinks[1];
        sources[3] >> pipes >> sinks[1];

        ASSERT_FALSE(sources[0].disconnect(sinks[1]));
        ASSERT_TRUE(sources[0].disconnect(sinks[0]));
        ASSERT_FALSE(sources[0].disconnect(sinks[0]));
        ASSERT_FALSE(sources[0].hasSinkPipe());
        ASSERT_EQ(sources[1].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[1].getSinkPipe()->getThroughOut()->getFinalSink(), //
                  &sinks[0]);
        ASSERT_EQ(sources[0].getSinkPipe(), //
                  nullptr);
        ASSERT_EQ(sources[3].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(sources[2].getSinkPipe()->getFinalSink(), //
                  &sinks[1]);
        ASSERT_EQ(pipes[2].getFinalSink(), //
                  nullptr);
    }
}

TEST(MIDI_Pipes, disconnectSinkFromSource) {
    {
        TrueMIDI_Source sources[2];
        MIDI_PipeFactory<5> pipes;
        StrictMock<MockMIDI_Sink> sinks[4];

        sinks[1] << pipes << sources[1];
        sinks[1] << pipes << sources[0];

        sinks[0] << pipes << sources[0];

        sinks[2] << pipes << sources[1];
        sinks[3] << pipes << sources[1];

        ASSERT_FALSE(sinks[3].disconnect(sources[0]));
        ASSERT_TRUE(sinks[3].disconnect(sources[1]));
        ASSERT_FALSE(sinks[3].disconnect(sources[1]));
        ASSERT_FALSE(sinks[3].hasSourcePipe());
        ASSERT_EQ(sinks[1].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(
            sinks[1].getSourcePipe()->getThroughIn()->getInitialSource(), //
            &sources[0]);
        ASSERT_EQ(sinks[0].getSourcePipe()->getInitialSource(), //
                  &sources[0]);
        ASSERT_EQ(sinks[2].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(sinks[3].getSourcePipe(), //
                  nullptr);
        ASSERT_EQ(pipes[4].getInitialSource(), //
                  nullptr);
    }
    {
        TrueMIDI_Source sources[2];
        MIDI_PipeFactory<5> pipes;
        StrictMock<MockMIDI_Sink> sinks[4];

        sinks[1] << pipes << sources[1];
        sinks[1] << pipes << sources[0];

        sinks[0] << pipes << sources[0];

        sinks[2] << pipes << sources[1];
        sinks[3] << pipes << sources[1];

        ASSERT_FALSE(sinks[2].disconnect(sources[0]));
        ASSERT_TRUE(sinks[2].disconnect(sources[1]));
        ASSERT_FALSE(sinks[2].disconnect(sources[1]));
        ASSERT_FALSE(sinks[2].hasSourcePipe());
        ASSERT_EQ(sinks[1].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(
            sinks[1].getSourcePipe()->getThroughIn()->getInitialSource(), //
            &sources[0]);
        ASSERT_EQ(sinks[0].getSourcePipe()->getInitialSource(), //
                  &sources[0]);
        ASSERT_EQ(sinks[3].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(sinks[2].getSourcePipe(), //
                  nullptr);
        ASSERT_EQ(pipes[3].getInitialSource(), //
                  nullptr);
    }
    {
        TrueMIDI_Source sources[2];
        MIDI_PipeFactory<5> pipes;
        StrictMock<MockMIDI_Sink> sinks[4];

        sinks[1] << pipes << sources[1];
        sinks[1] << pipes << sources[0];

        sinks[0] << pipes << sources[0];

        sinks[2] << pipes << sources[1];
        sinks[3] << pipes << sources[1];

        ASSERT_TRUE(sinks[1].disconnect(sources[1]));
        ASSERT_FALSE(sinks[1].disconnect(sources[1]));
        ASSERT_TRUE(sinks[1].hasSourcePipe());
        ASSERT_EQ(sinks[1].getSourcePipe()->getInitialSource(), //
                  &sources[0]);
        ASSERT_EQ(sinks[0].getSourcePipe()->getInitialSource(), //
                  &sources[0]);
        ASSERT_EQ(sinks[3].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(sinks[2].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(pipes[0].getInitialSource(), //
                  nullptr);
    }
    {
        TrueMIDI_Source sources[2];
        MIDI_PipeFactory<5> pipes;
        StrictMock<MockMIDI_Sink> sinks[4];

        sinks[1] << pipes << sources[1];
        sinks[1] << pipes << sources[0];

        sinks[0] << pipes << sources[0];

        sinks[2] << pipes << sources[1];
        sinks[3] << pipes << sources[1];

        ASSERT_TRUE(sinks[1].disconnect(sources[0]));
        ASSERT_FALSE(sinks[1].disconnect(sources[0]));
        ASSERT_TRUE(sinks[1].hasSourcePipe());
        ASSERT_EQ(sinks[1].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(sinks[0].getSourcePipe()->getInitialSource(), //
                  &sources[0]);
        ASSERT_EQ(sinks[3].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(sinks[2].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(pipes[1].getInitialSource(), //
                  nullptr);
    }
    {
        TrueMIDI_Source sources[2];
        MIDI_PipeFactory<5> pipes;
        StrictMock<MockMIDI_Sink> sinks[4];

        sinks[1] << pipes << sources[1];
        sinks[1] << pipes << sources[0];

        sinks[0] << pipes << sources[0];

        sinks[2] << pipes << sources[1];
        sinks[3] << pipes << sources[1];

        ASSERT_FALSE(sinks[0].disconnect(sources[1]));
        ASSERT_TRUE(sinks[0].disconnect(sources[0]));
        ASSERT_FALSE(sinks[0].disconnect(sources[0]));
        ASSERT_FALSE(sinks[0].hasSourcePipe());
        ASSERT_EQ(sinks[1].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(
            sinks[1].getSourcePipe()->getThroughIn()->getInitialSource(), //
            &sources[0]);
        ASSERT_EQ(sinks[0].getSourcePipe(), //
                  nullptr);
        ASSERT_EQ(sinks[3].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(sinks[2].getSourcePipe()->getInitialSource(), //
                  &sources[1]);
        ASSERT_EQ(pipes[2].getInitialSource(), //
                  nullptr);
    }
}