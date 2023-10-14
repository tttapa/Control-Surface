#include <gtest/gtest.h>

#include <MIDI_Inputs/NoteCCKPRange.hpp>
#include <MIDI_Inputs/NoteCCKPValue.hpp>

using namespace cs;

TEST(NoteValue, NoteOnNoteOff) {
    NoteValue mn({0x3C, Channel_5});
    mn.begin();

    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NoteOn, Channel_5, 0x3C, 0x7E});
    EXPECT_EQ(mn.getValue(), 0x7E);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NoteOff, Channel_5, 0x3C, 0x7E});
    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();

    // Second note off shouldn't set the dirty flag
    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NoteOn, Channel_5, 0x3C, 0x00});
    EXPECT_EQ(mn.getValue(), 0);
    EXPECT_FALSE(mn.getDirty());
}

TEST(NoteRange, NoteOnNoteOff) {
    NoteRange<2> mn {{0x3C, Channel_5}};
    mn.begin();

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NoteOn, Channel_5, 0x3C, 0x7E});

    EXPECT_EQ(mn.getValue(0), 0x7E);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NoteOn, Channel_5, 0x3D, 0x7D});

    EXPECT_EQ(mn.getValue(0), 0x7E);
    EXPECT_EQ(mn.getValue(1), 0x7D);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();

    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NoteOff, Channel_5, 0x3C, 0x7E});

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x7D);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();

    // Second note off shouldn't set the dirty flag
    MIDIInputElementNote::updateAllWith(
        {MIDIMessageType::NoteOn, Channel_5, 0x3C, 0x00});
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x7D);
    EXPECT_FALSE(mn.getDirty());
}

// -------------------------------------------------------------------------- //

TEST(NoteRange, bankableInRange) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {bank, {0x10, Channel_5}};
    mn.begin();

    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    // First bank, first address
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10,
        0x20,
    };
    EXPECT_TRUE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // First bank, third address
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 2,
        0x21,
    };
    EXPECT_TRUE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // Second bank, second address
    ChannelMessage midimsg3 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 4 + 1,
        0x22,
    };
    EXPECT_TRUE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_FALSE(mn.getDirty());

    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // First bank, first address
    ChannelMessage midimsg4 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10,
        0x23,
    };
    EXPECT_TRUE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_EQ(mn.getValue(0, 0), 0x23);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_FALSE(mn.getDirty());

    bank.select(0);
    EXPECT_EQ(mn.getValue(0), 0x23);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    EXPECT_EQ(mn.getValue(0, 0), 0x23);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_TRUE(mn.getDirty());
}

TEST(NoteRange, bankableNotInRange) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {bank, {0x10, Channel_5}};
    mn.begin();

    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // Before first bank
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 - 1,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg1));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // First bank, fourth address
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 3,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg2));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Second bank, fourth address
    ChannelMessage midimsg3 = {MIDIMessageType::NoteOn, Channel_5, 0x10 + 4 + 3,
                               0x20};
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg3));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Before first bank, first address
    ChannelMessage midimsg4 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 - 4,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg4));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // After last bank, first address
    ChannelMessage midimsg5 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 8,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg5));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Different channel
    ChannelMessage midimsg6 = {
        MIDIMessageType::NoteOn,
        Channel_5 + 1,
        0x10,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg6));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
}

TEST(NoteRange, bankableInRangeOffset) {
    Bank<2> bank(4, 0, -1);
    Bankable::NoteRange<2, 3> mn = {bank, {0x10 + 4, Channel_5}};
    mn.begin();

    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    // First bank, first address
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10,
        0x20,
    };
    EXPECT_TRUE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // First bank, third address
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 2,
        0x21,
    };
    EXPECT_TRUE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // Second bank, second address
    ChannelMessage midimsg3 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 4 + 1,
        0x22,
    };
    EXPECT_TRUE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_FALSE(mn.getDirty());

    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_EQ(mn.getValue(0, 0), 0x20);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // First bank, first address
    ChannelMessage midimsg4 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10,
        0x23,
    };
    EXPECT_TRUE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_EQ(mn.getValue(0, 0), 0x23);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_FALSE(mn.getDirty());

    bank.select(0);
    EXPECT_EQ(mn.getValue(0), 0x23);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    EXPECT_EQ(mn.getValue(0, 0), 0x23);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x21);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x22);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);
    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    EXPECT_TRUE(mn.getDirty());
}

TEST(NoteRange, bankableNotInRangeOffset) {
    Bank<2> bank(4, 0, -1);
    Bankable::NoteRange<2, 3> mn = {bank, {0x10 + 4, Channel_5}};
    mn.begin();

    EXPECT_TRUE(mn.getDirty());
    mn.clearDirty();
    EXPECT_FALSE(mn.getDirty());

    // Before first bank
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 - 1,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg1));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // First bank, fourth address
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 3,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg2));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Second bank, fourth address
    ChannelMessage midimsg3 = {MIDIMessageType::NoteOn, Channel_5, 0x10 + 4 + 3,
                               0x20};
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg3));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Before first bank, first address
    ChannelMessage midimsg4 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 - 4,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg4));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // After last bank, first address
    ChannelMessage midimsg5 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 8,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg5));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Different channel
    ChannelMessage midimsg6 = {
        MIDIMessageType::NoteOn,
        Channel_5 + 1,
        0x10,
        0x20,
    };
    mn.clearDirty();
    EXPECT_FALSE(mn.updateWith(midimsg6));
    EXPECT_FALSE(mn.getDirty());

    EXPECT_EQ(mn.getValue(0, 0), 0x00);
    EXPECT_EQ(mn.getValue(0, 1), 0x00);
    EXPECT_EQ(mn.getValue(0, 2), 0x00);
    EXPECT_EQ(mn.getValue(1, 0), 0x00);
    EXPECT_EQ(mn.getValue(1, 1), 0x00);
    EXPECT_EQ(mn.getValue(1, 2), 0x00);

    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
}

TEST(NoteRange, bankableInRangeChangeChannel) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {{bank, ChangeChannel}, {0x10, Channel_5}};
    mn.begin();

    // First bank, first address
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10,
        0x20,
    };
    EXPECT_TRUE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);

    // First bank, third address
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 2,
        0x21,
    };
    EXPECT_TRUE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);

    // Second bank, second address
    ChannelMessage midimsg3 = {
        MIDIMessageType::NoteOn,
        Channel_5 + 4,
        0x10 + 1,
        0x22,
    };
    EXPECT_TRUE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x20);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);

    // First bank, first address
    ChannelMessage midimsg4 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10,
        0x23,
    };
    EXPECT_TRUE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x22);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
    EXPECT_EQ(mn.getValue(0), 0x23);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x21);
}

TEST(NoteRange, bankableNotInRangeChangeChannel) {
    Bank<2> bank(4);
    Bankable::NoteRange<2, 3> mn = {{bank, ChangeChannel}, {0x10, Channel_5}};
    mn.begin();

    // Before first bank
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 - 1,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg1));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // First bank, fourth address
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x10 + 3,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg2));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Before second bank
    ChannelMessage midimsg3 = {
        MIDIMessageType::NoteOn,
        Channel_5 + 4,
        0x10 - 1,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg3));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Second bank, fourth address
    ChannelMessage midimsg4 = {
        MIDIMessageType::NoteOn,
        Channel_5 + 4,
        0x10 + 3,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg4));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // Before first bank, first address
    ChannelMessage midimsg5 = {
        MIDIMessageType::NoteOn,
        Channel_5 - 4,
        0x10,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg5));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);

    // After last bank, first address
    ChannelMessage midimsg6 = {
        MIDIMessageType::NoteOn,
        Channel_5 + 8,
        0x10,
        0x20,
    };
    EXPECT_FALSE(mn.updateWith(midimsg6));
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(1);
    EXPECT_EQ(mn.getValue(0), 0x00);
    EXPECT_EQ(mn.getValue(1), 0x00);
    EXPECT_EQ(mn.getValue(2), 0x00);
    bank.select(0);
}

// -------------------------------------------------------------------------- //

#include <MIDI_Inputs/LEDs/NoteCCKPLED.hpp>

TEST(NoteLED, NoteOnNoteOff) {
    NoteLED mnl = {2, {0x3C, Channel_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    MIDIInputElementNote::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg1);

    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOff,
        Channel_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg2);

    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BankableNoteLED, NoteOnNoteOff) {
    Bank<2> bank;
    Bankable::NoteLED<2> mnl = {bank, 2, {0x3C, Channel_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    MIDIInputElementNote::beginAll();
    EXPECT_TRUE(mnl.getDirty());
    mnl.clearDirty();
    EXPECT_FALSE(mnl.getDirty());

    // Bank 0 ON
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x7E);
    EXPECT_EQ(mnl.getValue(0), 0x7E);
    EXPECT_EQ(mnl.getValue(1), 0x00);
    EXPECT_TRUE(mnl.getDirty());
    mnl.clearDirty();

    // Bank 0 OFF
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOff,
        Channel_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg2);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x00);
    EXPECT_EQ(mnl.getValue(0), 0x00);
    EXPECT_EQ(mnl.getValue(1), 0x00);
    EXPECT_TRUE(mnl.getDirty());
    mnl.clearDirty();

    // Bank 1 ON
    ChannelMessage midimsg3 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x3D,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg3);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x00);
    EXPECT_EQ(mnl.getValue(0), 0x00);
    EXPECT_EQ(mnl.getValue(1), 0x7E);
    EXPECT_FALSE(mnl.getDirty());
    mnl.clearDirty();

    // Switch to Bank 1
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, HIGH));
    bank.select(1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x7E);
    EXPECT_EQ(mnl.getValue(0), 0x00);
    EXPECT_EQ(mnl.getValue(1), 0x7E);
    EXPECT_TRUE(mnl.getDirty());
}

#include <MIDI_Inputs/LEDs/NoteCCKPLEDPWM.hpp>

/// @todo TODO
TEST(NoteLED, PWM) {
    NoteLEDPWM mnl = {2, {0x3C, Channel_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    MIDIInputElementNote::beginAll();

    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 255));
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x3C,
        0x7F,
    };
    MIDIInputElementNote::updateAllWith(midimsg1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 16));
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x3C,
        0x08,
    };
    MIDIInputElementNote::updateAllWith(midimsg2);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());

    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 0));
    ChannelMessage midimsg3 = {
        MIDIMessageType::NoteOff,
        Channel_5,
        0x3C,
        0x7F,
    };
    MIDIInputElementNote::updateAllWith(midimsg3);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
}

TEST(BankableNoteLED, PWM) {
    Bank<2> bank;
    Bankable::NoteLEDPWM<2> mnl = {bank, 2, {0x3C, Channel_5}};

    ::testing::InSequence seq;

    EXPECT_CALL(ArduinoMock::getInstance(), pinMode(2, OUTPUT));
    EXPECT_CALL(ArduinoMock::getInstance(), digitalWrite(2, LOW));
    MIDIInputElementNote::beginAll();
    EXPECT_TRUE(mnl.getDirty());
    mnl.clearDirty();
    EXPECT_FALSE(mnl.getDirty());

    // Bank 0 ON
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 0x60));
    ChannelMessage midimsg1 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x3C,
        0x30,
    };
    MIDIInputElementNote::updateAllWith(midimsg1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x30);
    EXPECT_EQ(mnl.getValue(0), 0x30);
    EXPECT_EQ(mnl.getValue(1), 0x00);
    EXPECT_TRUE(mnl.getDirty());
    mnl.clearDirty();

    // Bank 0 OFF
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 0));
    ChannelMessage midimsg2 = {
        MIDIMessageType::NoteOff,
        Channel_5,
        0x3C,
        0x7E,
    };
    MIDIInputElementNote::updateAllWith(midimsg2);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x00);
    EXPECT_EQ(mnl.getValue(0), 0x00);
    EXPECT_EQ(mnl.getValue(1), 0x00);
    EXPECT_TRUE(mnl.getDirty());
    mnl.clearDirty();

    // Bank 1 ON
    ChannelMessage midimsg3 = {
        MIDIMessageType::NoteOn,
        Channel_5,
        0x3D,
        0x31,
    };
    MIDIInputElementNote::updateAllWith(midimsg3);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x00);
    EXPECT_EQ(mnl.getValue(0), 0x00);
    EXPECT_EQ(mnl.getValue(1), 0x31);
    EXPECT_FALSE(mnl.getDirty());
    mnl.clearDirty();

    // Switch to Bank 1
    EXPECT_CALL(ArduinoMock::getInstance(), analogWrite(2, 0x62));
    bank.select(1);
    ::testing::Mock::VerifyAndClear(&ArduinoMock::getInstance());
    EXPECT_EQ(mnl.getValue(), 0x31);
    EXPECT_EQ(mnl.getValue(0), 0x00);
    EXPECT_EQ(mnl.getValue(1), 0x31);
    EXPECT_TRUE(mnl.getDirty());
}