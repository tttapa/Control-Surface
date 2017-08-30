#include <iostream>
#include <cstring>
#include "FIFO.h"

using namespace std;

void parseMIDI(FIFO &f);

int main()
{
    FIFO f;

    cout << -1 % f.bufferSize << endl;

    cout << endl;

    f.buffer[0] = 0xB0;
    f.buffer[1] = 0x30;
    f.buffer[2] = 0x7F;
    f.buffer[3] = 0xD2;
    f.buffer[4] = 0x10;
    f.messages = 2;
    f.writeIndex = 5;
    f.messageStartIndex = 5;
    f.readIndex = 0;

    parseMIDI(f);

    cout << endl;

    f.buffer[0] = 0x10;
    f.messages = 1;
    f.writeIndex = 1;
    f.messageStartIndex = 1;
    f.readIndex = 0;

    parseMIDI(f);

    cout << endl;

    f.buffer[0] = 0x10;
    f.messages = 0;
    f.writeIndex = 1;
    f.messageStartIndex = 1;
    f.readIndex = 0;

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.buffer[0] = 0x10;
    f.messages = 1;
    f.writeIndex = 0;
    f.messageStartIndex = 0;
    f.readIndex = 0;

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.buffer[0] = 0xB0;
    f.buffer[1] = 0x30;
    f.buffer[2] = 0x7F;
    f.buffer[3] = 0xD2;
    f.buffer[4] = 0x10;
    f.buffer[5] = 0xBB;
    f.buffer[6] = 0x31;
    f.buffer[7] = 0x20;
    f.messages = 3;
    f.writeIndex = 0;
    f.messageStartIndex = 0;
    f.readIndex = 0;

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.buffer[0] = 0xB0; // Header
    f.buffer[1] = 0x30;
    f.buffer[2] = 0x7F;
    f.buffer[3] = 0x0F; // Wrong (third data byte)
    f.buffer[4] = 0xD2; // Header
    f.buffer[5] = 0x10;
    f.messages = 2;
    f.writeIndex = 6;
    f.messageStartIndex = 6;
    f.readIndex = 0;

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.messages = 0;
    f.writeIndex = 0;
    f.readIndex = 0;
    f.messageStartIndex = 0;

    f.addToMessage(0xB0);
    f.addToMessage(0x30);
    f.finishMessage(0x7F);

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.messages = 0;
    f.writeIndex = 0;
    f.readIndex = 0;
    f.messageStartIndex = 0;

    f.addToMessage(0xB0);
    f.addToMessage(0x30);
    f.addToMessage(0x0F); // Wrong (third data byte)
    f.finishMessage(0x7F);

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.messages = 0;
    f.writeIndex = 0;
    f.readIndex = 0;
    f.messageStartIndex = 0;

    f.addToMessage(0xB0);
    f.addToMessage(0x30);
    f.finishMessage(0x7F);
    f.addToMessage(0xD2);
    f.finishMessage(0x10);
    f.addToMessage(0xBB);
    f.addToMessage(0x31);
    f.finishMessage(0x20);

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.messages = 0;
    f.writeIndex = 0;
    f.readIndex = 0;
    f.messageStartIndex = 0;

    f.addToMessage(0xB0);
    f.addToMessage(0x10);
    f.finishMessage(0x20);
    f.addToMessage(0xB1);
    f.addToMessage(0x11);
    f.finishMessage(0x21);
    f.addToMessage(0xB2);
    f.addToMessage(0x12);
    f.finishMessage(0x22); // overflow

    parseMIDI(f);

    cout << endl;

    f.finishMessage(0x22); // overflow is resolved by reading other messages

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.messages = 0;
    f.writeIndex = 0;
    f.readIndex = 0;
    f.messageStartIndex = 0;

    f.addToMessage(0xB0);
    f.addToMessage(0x10);
    f.finishMessage(0x20);
    f.addToMessage(0xB1);
    f.addToMessage(0x11);
    f.finishMessage(0x21);
    f.addToMessage(0xB2);
    f.addToMessage(0x12);
    f.finishMessage(0x22); // overflow

    parseMIDI(f);

    cout << endl;

    f.startMessage(); // delete unfinished message
    f.addToMessage(0xDB);
    f.finishMessage(0x0C); // no more overflow

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.messages = 0;
    f.writeIndex = 0;
    f.readIndex = 0;
    f.messageStartIndex = 0;

    f.addToMessage(0xF7); // SysExStart
    f.addToMessage(0x01);
    f.finishMessage(0x02);
    f.addToMessage(0x03);
    f.finishMessage(SysExEnd);

    parseMIDI(f);

    cout << endl;

    memset(f.buffer, 0, 8);
    f.messages = 0;
    f.writeIndex = 0;
    f.readIndex = 0;
    f.messageStartIndex = 0;

    f.addToMessage(0xF7); // SysExStart
    f.addToMessage(0x01);
    f.finishMessage(0x02);
    f.addToMessage(0x03);
    f.finishMessage(0x04); // No SysExEnd

    parseMIDI(f);

    return 0;
}

void parseMIDI(FIFO &f)
{
    if (f.messages == 0)
    {
        cout << "Buffer empty, can't parse" << endl;
        return;
    }
    do
    {
        uint8_t header = f.getNextHeader();
        if (!header)
        {
            cout << "Buffer empty, can't find next header" << endl;
            return;
        }
        uint8_t messageType = header & 0xF0;
        uint8_t targetChannel = header & 0x0F;
        uint8_t data1 = f.read(); // this is allowed, because all messages have at least 1 data byte

        if (messageType == 0xB0)
        {
            cout << "Control Change:  \tchannel = " << targetChannel + 1 << "\tdata1 = " << (int)data1 << "\tdata2 = " << (int)f.read() << endl;
        }
        else if (messageType == 0xD0)
        {
            cout << "Channel Pressure:\tchannel = " << targetChannel + 1 << "\tdata1 = " << (int)data1 << endl;
        }
        else if (header == 0xF7)
        {
            cout << "System Exclusive:";
            while (data1 != SysExEnd && f.messages != 0) // TODO: any status byte should end SysEx (MIDI 1.0 Detailed Specification 4.2 p.29)
            {
                cout << '\t' << (int)data1;
                data1 = f.read();
            }
            if (f.messages == 0 && data1 != SysExEnd)
            {
                cout << endl
                     << "Reached end of buffer without SysExEnd" << endl;
            }
            else
            {
                cout << "\tSysExEnd" << endl;
            }
        }
    } while (f.messages != 0);
}