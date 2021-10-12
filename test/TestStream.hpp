#include <Arduino.h>
#include <queue>
#include <vector>

class TestStream : public Stream {
  public:
    size_t write(uint8_t data) override {
        sent.push_back(data);
        return 1;
    }
    int peek() override { return toRead.empty() ? -1 : toRead.front(); }
    int read() override {
        int retval = peek();
        if (!toRead.empty())
            toRead.pop();
        return retval;
    }
    int available() override { return toRead.size(); }

    std::vector<uint8_t> sent;
    std::queue<uint8_t> toRead;
};