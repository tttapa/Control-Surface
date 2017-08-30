#ifndef FIFO_H_
#define FIFO_H_

#include <iostream>

using namespace std;

const uint8_t SysExEnd = 0xF0;

class FIFO
{
  public:
    FIFO() {}
    ~FIFO() {}
    void startMessage()
    {
        writeIndex = messageStartIndex;
    }
    bool addToMessage(uint8_t data)
    {
        if (!hasSpaceLeft())
        {
            cout << "No more space left in the buffer\t ReadIndex = " << (int)readIndex << "\t writeIndex = " << (int)writeIndex << endl;
            return false;
        }
        buffer[writeIndex] = data;
        incrementWriteIndex();
        return true;
    }
    bool hasSpaceLeft(size_t bytes = 1)
    {
        return messages == 0 || (readIndex - writeIndex) % bufferSize >= bytes;
    }
    bool finishMessage(uint8_t data)
    {
        if (!addToMessage(data))
            return false;
        messageStartIndex = writeIndex;
        messages++;
        return true;
    }
    uint8_t read()
    {
        if (messages == 0)
            return 0;

        uint8_t midiByte = buffer[readIndex];
        incrementReadIndex();
        if (messageStartIndex == readIndex)
            messages = 0;
        else if (isHeader(buffer[readIndex])) // if the next byte is a header byte, the previous event was finished
            messages--;
        return midiByte;
    }
    uint8_t peek()
    {
        if (messages == 0)
            return 0;
        return buffer[readIndex];
    }
    uint8_t getNextHeader()
    {
        if (isHeader(buffer[readIndex]))
        {
            // cout << "This is the next header" << endl;
            return read();
        }
        do
        {
            incrementReadIndex();
            cout << "Not a header, incrementing readIndex: " << (int)readIndex << endl;
        } while (!isHeader(buffer[readIndex]) && messageStartIndex != readIndex);

        if (messageStartIndex == readIndex) // if the buffer is empty
        {
            cout << "Reached end of buffer, didn't find a header" << endl;
            messages = 0;
            return 0;
        }
        else // next header is reached, so previous event was finished
        {
            cout << "Header found" << endl;
            messages--;
            return read();
        }
    }
    inline bool isHeader(uint8_t data)
    {
        return (data & (1 << 7)) && data != SysExEnd;
    }
    void incrementWriteIndex()
    {
        writeIndex = (writeIndex + 1) % bufferSize;
    }
    void incrementReadIndex()
    {
        readIndex = (readIndex + 1) % bufferSize;
    }

    // private:
    const static size_t bufferSize = 8;
    size_t readIndex = 0, messageStartIndex = 0, writeIndex = 0;
    size_t messages = 0;
    uint8_t buffer[8];
};

#endif // FIFO_H_