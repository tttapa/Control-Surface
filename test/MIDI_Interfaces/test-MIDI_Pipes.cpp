#include <AH/Error/Error.hpp>
#include <MIDI_Interfaces/MIDI_Pipes.hpp>
#include <MIDI_Interfaces/MIDI_Staller.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

USING_CS_NAMESPACE;
using ::testing::StrictMock;

struct MockMIDI_Sink : TrueMIDI_Sink {
    MOCK_METHOD(void, sinkMIDIfromPipe, (ChannelMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (SysExMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (SysCommonMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (RealTimeMessage), (override));
};

struct MockMIDI_SinkSource : TrueMIDI_SinkSource {
    MOCK_METHOD(void, sinkMIDIfromPipe, (ChannelMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (SysExMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (SysCommonMessage), (override));
    MOCK_METHOD(void, sinkMIDIfromPipe, (RealTimeMessage), (override));
};

struct DummyMIDI_Sink : TrueMIDI_Sink {
    void sinkMIDIfromPipe(ChannelMessage) override {}
    void sinkMIDIfromPipe(SysExMessage) override {}
    void sinkMIDIfromPipe(SysCommonMessage) override {}
    void sinkMIDIfromPipe(RealTimeMessage) override {}
};

TEST(MIDI_Pipes, sourcePipeSink) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe;
    TrueMIDI_Source source;

    source >> pipe >> sink;

    {
        RealTimeMessage msg = {0xFF, CABLE_4};
        EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink);
    }
    {
        ChannelMessage msg{0x93, 0x10, 0x7F, CABLE_6};
        EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink);
    }
    {
        SysExMessage msg = {nullptr, 0, CABLE_11};
        EXPECT_CALL(sink, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink);
    }
}

TEST(MIDI_Pipes, sourcePipeSinkMap) {
    StrictMock<MockMIDI_Sink> sink1, sink2;
    MIDI_Pipe pipe1;
    TrueMIDI_Source source;

    struct CustomPipe : MIDI_Pipe {
        void mapForwardMIDI(ChannelMessage msg) override {
            msg.setChannel(CHANNEL_8);
            sourceMIDItoSink(msg);
        }
    } pipe2;

    source >> pipe1 >> sink1;
    source >> pipe2 >> sink2;

    {
        RealTimeMessage msg = {0xFF, CABLE_4};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        ChannelMessage msg{0x93, 0x10, 0x7F, CABLE_6};
        ChannelMessage msg2{0x97, 0x10, 0x7F, CABLE_6};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg2));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        SysExMessage msg = {nullptr, 0, CABLE_11};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
}

TEST(MIDI_Pipes, sourcePipeSinkFilter) {
    StrictMock<MockMIDI_Sink> sink1, sink2;
    MIDI_Pipe pipe1;
    TrueMIDI_Source source;

    struct CustomPipe : MIDI_Pipe {
        void mapForwardMIDI(ChannelMessage msg) override {
            if (msg.getChannel() == CHANNEL_7)
                return;
            sourceMIDItoSink(msg);
        }
    } pipe2;

    source >> pipe1 >> sink1;
    source >> pipe2 >> sink2;

    {
        RealTimeMessage msg = {0xFF, CABLE_4};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        ChannelMessage msg{0x93, 0x10, 0x7F, CABLE_6};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        ChannelMessage msg{0x96, 0x10, 0x7F, CABLE_6};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        // sink2 shouldn't receive it
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        SysExMessage msg = {nullptr, 0, CABLE_11};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
}

TEST(MIDI_Pipes, sourceX2PipeSink) {
    StrictMock<MockMIDI_Sink> sink;
    MIDI_Pipe pipe1, pipe2;
    TrueMIDI_Source source1, source2;

    source1 >> pipe1 >> sink;
    source2 >> pipe2 >> sink;

    RealTimeMessage msg = {0xFF, CABLE_4};
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

    RealTimeMessage msg = {0xFF, CABLE_4};

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
        RealTimeMessage msg = {0xFF, CABLE_4};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        ChannelMessage msg{0x93, 0x10, 0x7F, CABLE_6};
        EXPECT_CALL(sink1, sinkMIDIfromPipe(msg));
        EXPECT_CALL(sink2, sinkMIDIfromPipe(msg));
        source.sourceMIDItoPipe(msg);
        ::testing::Mock::VerifyAndClear(&sink1);
        ::testing::Mock::VerifyAndClear(&sink2);
    }
    {
        SysExMessage msg = {nullptr, 0, CABLE_11};
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

    RealTimeMessage msg = {0xFF, CABLE_4};

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

    RealTimeMessage msg = {0xFF, CABLE_4};
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

    RealTimeMessage msg = {0xFF, CABLE_4};

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

    RealTimeMessage msg = {0xFF, CABLE_4};

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

    EXPECT_EQ(source2.getSinkPipe(), &pipe3);
    EXPECT_EQ(pipe3.getSource(), &source2);
    EXPECT_EQ(sink1.getSourcePipe(), &pipe3);
    EXPECT_EQ(pipe3.getSink(), &sink1);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe4);
    EXPECT_EQ(pipe4.getSink(), &sink2);
    EXPECT_EQ(pipe3.getThroughOut(), &pipe4);
    EXPECT_EQ(pipe4.getSource(), (MIDI_Source *)&pipe3);
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
    EXPECT_FALSE(pipe1.hasSource());
    EXPECT_TRUE(pipe2.hasSink());
    EXPECT_TRUE(pipe2.hasSource());
    EXPECT_FALSE(pipe3.hasSink());
    EXPECT_FALSE(pipe3.hasSource());
    EXPECT_TRUE(pipe4.hasSink());
    EXPECT_TRUE(pipe4.hasSource());

    EXPECT_FALSE(pipe1.hasThroughIn());
    EXPECT_FALSE(pipe1.hasThroughOut());
    EXPECT_TRUE(pipe2.hasThroughIn());
    EXPECT_FALSE(pipe2.hasThroughOut());
    EXPECT_FALSE(pipe3.hasThroughIn());
    EXPECT_FALSE(pipe3.hasThroughOut());
    EXPECT_FALSE(pipe4.hasThroughIn());
    EXPECT_FALSE(pipe4.hasThroughOut());

    EXPECT_EQ(source1.getSinkPipe(), &pipe2);
    EXPECT_EQ(pipe2.getSource(), &source1);
    EXPECT_EQ(source2.getSinkPipe(), &pipe4);
    EXPECT_EQ(pipe4.getSource(), &source2);
    EXPECT_EQ(sink2.getSourcePipe(), &pipe2);
    EXPECT_EQ(pipe2.getSink(), &sink2);
    EXPECT_EQ(pipe2.getThroughIn(), &pipe4);
    EXPECT_EQ(pipe4.getSink(), (MIDI_Sink *)&pipe2);
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

    DummyMIDI_Sink sink3(std::move(sink1));

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

    swap(sink1, sink2);

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

    // Calls move constructor and move assignment
    std::swap(sink1, sink2);

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

    swap(source1, source2);

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

    // Calls move constructor and move assignment
    std::swap(source1, source2);

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
}

struct MockMIDIStaller : MIDIStaller {
    MockMIDIStaller(const char *name, MIDI_Source *stalled = nullptr)
        : name(name), stalled(stalled) {}
    MOCK_METHOD(void, stallHandled, ());
    void handleStall() override {
        if (!stalled)
            throw std::runtime_error("Not stalling any sources");
        stallHandled();
        stalled->unstall(this);
    }
    const char *getName() const override { return name; }
    const char *name;
    MIDI_Source *stalled;
};

TEST(MIDI_Pipes, stall) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_FALSE(sources[1].isStalled());
    EXPECT_FALSE(sources[2].isStalled());
    EXPECT_FALSE(sources[3].isStalled());

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    // Sources                  Pipes                   Sinks
    //
    //   [0]   ────────────────────[2]────────────┐
    //                    ┌────────────[1]────────┴───   [0]
    //   [1]   ───────────┴────────[0]────────┬───────   [1]
    //   [2]   ────────────────[3]────────┬───┘
    //   [3]   ────────────[4]────────────┘

    MockMIDIStaller staller("staller");

    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_FALSE(sources[1].isStalled());
    EXPECT_FALSE(sources[2].isStalled());
    EXPECT_FALSE(sources[3].isStalled());

    sources[3].stall(&staller);
    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_TRUE(sources[1].isStalled());
    EXPECT_TRUE(sources[2].isStalled());
    EXPECT_TRUE(sources[3].isStalled());

    sources[3].unstall(&staller);
    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_FALSE(sources[1].isStalled());
    EXPECT_FALSE(sources[2].isStalled());
    EXPECT_FALSE(sources[3].isStalled());

    sources[2].stall(&staller);
    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_TRUE(sources[1].isStalled());
    EXPECT_TRUE(sources[2].isStalled());
    EXPECT_TRUE(sources[3].isStalled());

    sources[2].unstall(&staller);
    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_FALSE(sources[1].isStalled());
    EXPECT_FALSE(sources[2].isStalled());
    EXPECT_FALSE(sources[3].isStalled());

    sources[1].stall(&staller);
    EXPECT_TRUE(sources[0].isStalled());
    EXPECT_TRUE(sources[1].isStalled());
    EXPECT_TRUE(sources[2].isStalled());
    EXPECT_TRUE(sources[3].isStalled());

    sources[1].unstall(&staller);
    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_FALSE(sources[1].isStalled());
    EXPECT_FALSE(sources[2].isStalled());
    EXPECT_FALSE(sources[3].isStalled());

    sources[0].stall(&staller);
    EXPECT_TRUE(sources[0].isStalled());
    EXPECT_TRUE(sources[1].isStalled());
    EXPECT_FALSE(sources[2].isStalled());
    EXPECT_FALSE(sources[3].isStalled());

    sources[0].unstall(&staller);
    EXPECT_FALSE(sources[0].isStalled());
    EXPECT_FALSE(sources[1].isStalled());
    EXPECT_FALSE(sources[2].isStalled());
    EXPECT_FALSE(sources[3].isStalled());
}

TEST(MIDI_Pipes, stallHandleStallerSink) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    MockMIDIStaller staller("staller", &sources[3]);

    sources[3].stall(&staller);
    EXPECT_CALL(staller, stallHandled());
    sources[1].handleStallers();
}

TEST(MIDI_Pipes, stallHandleStallerThrough) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    MockMIDIStaller staller("staller", &sources[0]);

    sources[0].stall(&staller);
    EXPECT_CALL(staller, stallHandled());
    sources[1].handleStallers();
}

TEST(MIDI_Pipes, stallHandleStallerNotUnstalling) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    struct TestStaller : MIDIStaller {
        void handleStall() override { count++; }
        size_t count = 0;
    } staller;

    sources[3].stall(&staller);
    sources[1].handleStallers();
    // Should try at least twice, then fail and return
    EXPECT_GT(staller.count, 1);
}

TEST(MIDI_Pipes, stallHandleStallerEternal) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    sources[2].stall();
    try {
        sources[1].handleStallers();
        FAIL();
    } catch (AH::ErrorException &e) {
        std::cerr << e.what() << std::endl;
        EXPECT_EQ(e.getErrorCode(), 0x4827);
    }
}

TEST(MIDI_Pipes, stallTwiceSameCause) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    MockMIDIStaller staller_A("staller_A"), staller_B("staller_B");

    sources[3].stall(&staller_A);
    try {
        sources[3].stall(&staller_A);
    } catch (AH::ErrorException &e) {
        FAIL() << e.what() << std::endl;
    }
}

TEST(MIDI_Pipes, stallTwiceDifferentCauseSink) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    MockMIDIStaller staller_A("staller_A"), staller_B("staller_B");

    sources[3].stall(&staller_A);
    try {
        sources[3].stall(&staller_B);
        FAIL();
    } catch (AH::ErrorException &e) {
        std::cerr << e.what() << std::endl;
        EXPECT_EQ(e.getErrorCode(), 0x6665);
    }
}

TEST(MIDI_Pipes, stallUnstallDifferentCauseSink) {
    StrictMock<MockMIDI_Sink> sinks[2];
    MIDI_PipeFactory<6> pipes;
    TrueMIDI_Source sources[4];

    sources[1] >> pipes >> sinks[1];
    sources[1] >> pipes >> sinks[0];

    sources[0] >> pipes >> sinks[0];

    sources[2] >> pipes >> sinks[1];
    sources[3] >> pipes >> sinks[1];

    MockMIDIStaller staller_A("staller_A"), staller_B("staller_B");

    sources[3].stall(&staller_A);
    try {
        sources[3].unstall(&staller_B);
        FAIL();
    } catch (AH::ErrorException &e) {
        std::cerr << e.what() << std::endl;
        EXPECT_EQ(e.getErrorCode(), 0x6666);
    }
}

TEST(MIDI_Pipes, stallNames) {
    StrictMock<MockMIDI_Sink> sinks[3];
    MIDI_Pipe pipes[6];
    TrueMIDI_Source sources[4];

    sources[0] >> pipes[0] >> sinks[0];
    sources[0] >> pipes[1] >> sinks[1];
    sources[0] >> pipes[2] >> sinks[2];

    sources[1] >> pipes[3] >> sinks[0];
    sources[2] >> pipes[4] >> sinks[1];
    sources[3] >> pipes[5] >> sinks[2];

    MockMIDIStaller stallers[]{{"staller_1"}, {"staller_2"}, {"staller_3"}};

    sources[1].stall(&stallers[0]);
    EXPECT_STREQ(pipes[0].getSinkStallerName(), "staller_1");
    EXPECT_STREQ(pipes[0].getThroughStallerName(), "(null)");
    sources[2].stall(&stallers[1]);
    EXPECT_STREQ(pipes[0].getSinkStallerName(), "staller_1");
    EXPECT_STREQ(pipes[0].getThroughStallerName(), "staller_2");
    sources[3].stall();
    EXPECT_STREQ(pipes[2].getSinkStallerName(), "(eternal stall)");
    sources[3].unstall();

    struct DefaultNameMIDIStaller : MIDIStaller {
        void handleStall() override {}
    } default_staller;
    sources[3].stall(&default_staller);
    EXPECT_STREQ(pipes[2].getSinkStallerName(), "<?>");
}

TEST(MIDI_Pipes, getStaller) {
    StrictMock<MockMIDI_Sink> sinks[3];
    MIDI_Pipe pipes[6];
    TrueMIDI_Source sources[4];

    sources[0] >> pipes[0] >> sinks[0];
    sources[0] >> pipes[1] >> sinks[1];
    sources[0] >> pipes[2] >> sinks[2];

    sources[1] >> pipes[3] >> sinks[0];
    sources[2] >> pipes[4] >> sinks[1];
    sources[3] >> pipes[5] >> sinks[2];

    MockMIDIStaller stallers[]{{"staller_1"}, {"staller_2"}, {"staller_3"}};

    EXPECT_EQ(pipes[0].getStaller(), nullptr);
    EXPECT_EQ(sources[0].getStaller(), nullptr);
    EXPECT_STREQ(pipes[0].getStallerName(), "(null)");
    EXPECT_STREQ(sources[0].getStallerName(), "(null)");
    sources[3].stall(&stallers[2]);
    EXPECT_EQ(pipes[0].getStaller(), &stallers[2]);   // through staller
    EXPECT_EQ(sources[0].getStaller(), &stallers[2]); // through staller
    EXPECT_STREQ(pipes[0].getStallerName(), "staller_3");
    EXPECT_STREQ(sources[0].getStallerName(), "staller_3");
    sources[1].stall(&stallers[0]);
    EXPECT_EQ(pipes[0].getStaller(), &stallers[0]);   // sink staller
    EXPECT_EQ(sources[0].getStaller(), &stallers[0]); // sink staller
    EXPECT_STREQ(pipes[0].getStallerName(), "staller_1");
    EXPECT_STREQ(sources[0].getStallerName(), "staller_1");

    sources[0].disconnectSinkPipes();
    EXPECT_EQ(sources[0].getStaller(), nullptr);
    EXPECT_STREQ(sources[0].getStallerName(), "(null)");
}

TEST(MIDI_Pipes, stallFromThroughInAndThroughOut) {
    StrictMock<MockMIDI_Sink> sinks[3];
    MIDI_Pipe pipes[6];
    TrueMIDI_Source sources[4];

    sources[0] >> pipes[0] >> sinks[0];
    sources[0] >> pipes[1] >> sinks[1];
    sources[0] >> pipes[2] >> sinks[2];

    sources[1] >> pipes[3] >> sinks[0];
    sources[2] >> pipes[4] >> sinks[1];
    sources[3] >> pipes[5] >> sinks[2];

    MockMIDIStaller stallers[]{{"staller_1"}, {"staller_2"}, {"staller_3"}};

    sources[1].stall(&stallers[0]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[0].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[1].isStalled());
    EXPECT_FALSE(pipes[2].isStalled());
    EXPECT_FALSE(pipes[4].isStalled());
    EXPECT_FALSE(pipes[5].isStalled());

    sources[2].stall(&stallers[1]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[1].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[2].isStalled());
    EXPECT_FALSE(pipes[5].isStalled());

    sources[3].stall(&stallers[2]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
}

TEST(MIDI_Pipes, stallFromThroughInAndThroughOutUnlockMiddle) {
    StrictMock<MockMIDI_Sink> sinks[3];
    MIDI_Pipe pipes[6];
    TrueMIDI_Source sources[4];

    sources[0] >> pipes[0] >> sinks[0];
    sources[0] >> pipes[1] >> sinks[1];
    sources[0] >> pipes[2] >> sinks[2];

    sources[1] >> pipes[3] >> sinks[0];
    sources[2] >> pipes[4] >> sinks[1];
    sources[3] >> pipes[5] >> sinks[2];

    MockMIDIStaller stallers[]{{"staller_1"}, {"staller_2"}, {"staller_3"}};

    // Same as test above
    sources[1].stall(&stallers[0]);
    sources[2].stall(&stallers[1]);
    sources[3].stall(&stallers[2]);

    sources[2].unstall(&stallers[1]);
    // Should unlock pipe[4] sink,
    //        unlock pipe[1] sink
    //        change pipe[0] staller from staller[1] to staller[2]
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[4].isStalled());

    // Unstalling should be idempotent
    sources[2].unstall(&stallers[1]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[4].isStalled());

    // Lock it again to make sure unlocking was reversible
    sources[2].stall(&stallers[1]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);

    // Stalling should be idempotent
    sources[2].stall(&stallers[1]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);

    sources[2].unstall(&stallers[1]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[4].isStalled());

    sources[3].unstall(&stallers[2]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[0].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[1].isStalled());
    EXPECT_FALSE(pipes[2].isStalled());
    EXPECT_FALSE(pipes[4].isStalled());
    EXPECT_FALSE(pipes[5].isStalled());

    sources[1].unstall(&stallers[0]);
    EXPECT_FALSE(pipes[0].isStalled());
    EXPECT_FALSE(pipes[1].isStalled());
    EXPECT_FALSE(pipes[2].isStalled());
    EXPECT_FALSE(pipes[3].isStalled());
    EXPECT_FALSE(pipes[4].isStalled());
    EXPECT_FALSE(pipes[5].isStalled());
}

TEST(MIDI_Pipes, stallFromThroughInAndThroughOutUnlockLast) {
    StrictMock<MockMIDI_Sink> sinks[3];
    MIDI_Pipe pipes[6];
    TrueMIDI_Source sources[4];

    sources[0] >> pipes[0] >> sinks[0];
    sources[0] >> pipes[1] >> sinks[1];
    sources[0] >> pipes[2] >> sinks[2];

    sources[1] >> pipes[3] >> sinks[0];
    sources[2] >> pipes[4] >> sinks[1];
    sources[3] >> pipes[5] >> sinks[2];

    MockMIDIStaller stallers[]{{"staller_1"}, {"staller_2"}, {"staller_3"}};

    // Same as test above
    sources[1].stall(&stallers[0]);
    sources[2].stall(&stallers[1]);
    sources[3].stall(&stallers[2]);

    sources[3].unstall(&stallers[2]);
    // Should unlock pipe[5] sink,
    //        unlock pipe[2] sink
    //        unlock pipe[1] through
    //        keep pipe[0] through
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[2].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[1].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[5].isStalled());

    // Unstalling should be idempotent
    sources[3].unstall(&stallers[2]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[2].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[1].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[5].isStalled());

    // Lock it again to make sure unlocking was reversible
    sources[3].stall(&stallers[2]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);

    // Stalling should be idempotent
    sources[3].stall(&stallers[2]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[5].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[2].getSinkStaller(), &stallers[2]);
    EXPECT_EQ(pipes[1].getThroughStaller(), &stallers[2]);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);

    sources[3].unstall(&stallers[2]);
    EXPECT_EQ(pipes[3].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), &stallers[0]);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[2].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[1].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[3].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[5].isStalled());

    sources[1].unstall(&stallers[0]);
    EXPECT_EQ(pipes[0].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[4].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[1].getSinkStaller(), &stallers[1]);
    EXPECT_EQ(pipes[0].getThroughStaller(), &stallers[1]);
    EXPECT_EQ(pipes[2].getSinkStaller(), nullptr);
    EXPECT_EQ(pipes[1].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[4].getThroughStaller(), nullptr);
    EXPECT_EQ(pipes[2].getThroughStaller(), nullptr);
    EXPECT_FALSE(pipes[3].isStalled());
    EXPECT_FALSE(pipes[5].isStalled());

    sources[2].unstall(&stallers[1]);
    EXPECT_FALSE(pipes[0].isStalled());
    EXPECT_FALSE(pipes[1].isStalled());
    EXPECT_FALSE(pipes[2].isStalled());
    EXPECT_FALSE(pipes[3].isStalled());
    EXPECT_FALSE(pipes[4].isStalled());
    EXPECT_FALSE(pipes[5].isStalled());
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
    EXPECT_CALL(midiA[0].backend, read())
        .WillOnce(Return(Packet_t{0x54, 0xF0, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{0x54, 0x77, 0x11, 0x22}))
        .WillOnce(Return(Packet_t{0x56, 0x33, 0xF7, 0x00}))
        .WillOnce(Return(Packet_t{}));
    uint8_t data1[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0xF7};
    EXPECT_CALL(midiB[0], sendSysExImpl(SysExMessage(data1, CABLE_6)));
    midiA[0].update();

    EXPECT_CALL(midiA[1].backend, read())
        .WillOnce(Return(Packet_t{0x94, 0xF0, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{0x94, 0x77, 0x11, 0x22}))
        .WillOnce(Return(Packet_t{0x95, 0xF7, 0x00, 0x00}))
        .WillOnce(Return(Packet_t{}));
    uint8_t data2[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0xF7};
    EXPECT_CALL(midiB[0], sendSysExImpl(SysExMessage(data2, CABLE_10)));
    EXPECT_CALL(midiB[1], sendSysExImpl(SysExMessage(data2, CABLE_10)));
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
    EXPECT_CALL(midiA[1].backend, read())
        .WillOnce(Return(Packet_t{0x94, 0xF0, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{0x94, 0x77, 0x11, 0x22}))
        .WillOnce(Return(Packet_t{0x95, 0xF7, 0x00, 0x00}))
        .WillOnce(Return(Packet_t{}));
    uint8_t data[] = {0xF0, 0x55, 0x66, 0x77, 0x11, 0x22, 0xF7};

    ChannelMessage msg = {MIDIMessageType::NOTE_ON, CHANNEL_1, 0x12, 0x34};
    auto unstall = [&](MIDIStaller *staller) {
        midiA[0].unstall(staller);
        midiA[0].sourceMIDItoPipe(msg);
    };
    auto staller = makeMIDIStaller(unstall);
    midiA[0].stall(staller);

    // When updating midiA[1], it attempts to send the SysEx message over the
    // pipe, but it's stalled, so it first calls back the staller, which sends
    // a Note On message first, and then un-stalls the pipe, allowing the SysEx
    // message to be sent.
    testing::Sequence s1, s2;
    EXPECT_CALL(midiB[0], sendChannelMessageImpl(msg)) //
        .InSequence(s1, s2);
    EXPECT_CALL(midiB[0], sendSysExImpl(SysExMessage(data, CABLE_10)))
        .InSequence(s2);
    EXPECT_CALL(midiB[1], sendSysExImpl(SysExMessage(data, CABLE_10)))
        .InSequence(s1);
    midiA[1].update();
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
    EXPECT_CALL(midiA[1].backend, read())
        .WillOnce(Return(Packet_t{0x98, 0x83, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{}));

    ChannelMessage msg = {MIDIMessageType::NOTE_ON, CHANNEL_1, 0x12, 0x34};
    auto unstall = [&](MIDIStaller *staller) {
        midiA[0].unstall(staller);
        midiA[0].sourceMIDItoPipe(msg);
    };
    auto staller = makeMIDIStaller(unstall);
    midiA[0].stall(staller);

    // When updating midiA[1], it attempts to send the Note Off message over the
    // pipe, but it's stalled, so it first calls back the staller, which sends
    // a Note On message first, and then un-stalls the pipe, allowing the
    // Note Off message to be sent.
    testing::Sequence s1, s2;
    ChannelMessage msg2 = {0x83, 0x55, 0x66, CABLE_10};
    EXPECT_CALL(midiB[0], sendChannelMessageImpl(msg)).InSequence(s1, s2);
    EXPECT_CALL(midiB[0], sendChannelMessageImpl(msg2)).InSequence(s2);
    EXPECT_CALL(midiB[1], sendChannelMessageImpl(msg2)).InSequence(s1);
    midiA[1].update();
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
    EXPECT_CALL(midiA[1].backend, read())
        .WillOnce(Return(Packet_t{0x9F, 0xF8, 0x00, 0x00}))
        .WillOnce(Return(Packet_t{}));

    ChannelMessage msg = {MIDIMessageType::NOTE_ON, CHANNEL_1, 0x12, 0x34};
    auto unstall = [&](MIDIStaller *staller) {
        midiA[0].unstall(staller);
        midiA[0].sourceMIDItoPipe(msg);
    };
    auto staller = makeMIDIStaller(unstall);
    midiA[0].stall(staller);

    // When updating midiA[1], it sends the Real-Time message immediately,
    // even though the pipe is stalled.
    EXPECT_CALL(midiB[0], sendRealTimeImpl(RealTimeMessage(0xF8, CABLE_10)));
    EXPECT_CALL(midiB[1], sendRealTimeImpl(RealTimeMessage(0xF8, CABLE_10)));
    midiA[1].update();

    testing::Mock::VerifyAndClear(&midiB[0]);
    testing::Mock::VerifyAndClear(&midiB[1]);
    EXPECT_CALL(midiB[0], sendChannelMessageImpl(msg));
    midiA[0].handleStallers();
}

TEST(MIDI_Pipes, USBInterfaceLoopBack) {
    StrictMock<USBMIDI_Interface> midi;

    MIDI_Pipe pipe;

    midi >> pipe >> midi;

    using Packet_t = USBMIDI_Interface::MIDIUSBPacket_t;
    EXPECT_CALL(midi.backend, read())
        .WillOnce(Return(Packet_t{0x99, 0x95, 0x55, 0x66}))
        .WillOnce(Return(Packet_t{}));

    EXPECT_CALL(midi.backend, write(0x99, 0x95, 0x55, 0x66));
    midi.update();
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

#include <MIDI_Interfaces/SerialMIDI_Interface.hpp>
#include <TestStream.hpp>
#include <random>

TEST(MIDI_Pipes, StreamMIDIInterfaceSysExChunks) {
    StrictMock<TestStream> streams[2];
    StreamMIDI_Interface midiA[2] = {streams[0], streams[1]};
    StrictMock<MockMIDI_Interface> midiB;

    MIDI_Pipe pipe1, pipe2;

    midiA[0] >> pipe1 >> midiB;
    midiA[1] >> pipe2 >> midiB;

    std::vector<uint8_t> sysex(2 * SYSEX_BUFFER_SIZE + 10);
    sysex.front() = 0xF0;
    sysex.back() = 0xF7;
    std::mt19937 rnd(0);
    std::uniform_int_distribution<uint8_t> dist(0, 127);
    std::generate(sysex.begin() + 1, sysex.end() - 1, std::bind(dist, rnd));

    // Enough for one chunk, should stall the pipe
    streams[0].toRead = std::queue<uint8_t>(std::deque<uint8_t>(
        sysex.begin(), sysex.begin() + SYSEX_BUFFER_SIZE + 1));

    SysExMessage chunk1 = SysExMessage(sysex.data(), SYSEX_BUFFER_SIZE);
    EXPECT_CALL(midiB, sendSysExImpl(chunk1));
    midiA[0].update();
    testing::Mock::VerifyAndClear(&midiB);

    EXPECT_TRUE(pipe1.isStalled());
    EXPECT_TRUE(pipe2.isStalled());

    streams[0].toRead = std::queue<uint8_t>(std::deque<uint8_t>(
        sysex.begin() + SYSEX_BUFFER_SIZE + 1, sysex.end()));

    std::vector<uint8_t> noteMsg = {0x92, 0x12, 0x13};
    streams[1].toRead = std::queue<uint8_t>(
        std::deque<uint8_t>(noteMsg.begin(), noteMsg.end()));

    SysExMessage chunk2 =
        SysExMessage(sysex.data() + SYSEX_BUFFER_SIZE, SYSEX_BUFFER_SIZE);
    SysExMessage chunk3 =
        SysExMessage(sysex.data() + 2 * SYSEX_BUFFER_SIZE, 10);
    ChannelMessage noteMsg1 = {MIDIMessageType::NOTE_ON, CHANNEL_3, 0x12, 0x13};
    testing::Sequence s;
    EXPECT_CALL(midiB, sendSysExImpl(chunk2)).InSequence(s);
    EXPECT_CALL(midiB, sendSysExImpl(chunk3)).InSequence(s);
    EXPECT_CALL(midiB, sendChannelMessageImpl(noteMsg1)).InSequence(s);
    EXPECT_CALL(ArduinoMock::getInstance(), millis()).WillRepeatedly(Return(0));
    // midiA[1] parses a message and tries to send it over the pipe, but it's
    // stalled by midiA[0], so it calls back to midiA[0], which reads and parses
    // the rest of the SysEx message and un-stalls the pipe, so midiA[1] can
    // send its message
    midiA[1].update();
    testing::Mock::VerifyAndClear(&midiB);
    testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}
