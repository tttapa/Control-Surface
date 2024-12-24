#include <MIDI_Interfaces/USBMIDI_Sender.hpp>
#include <MIDI_Parsers/LambdaPuller.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

std::ostream &operator<<(std::ostream &os, FlashString_t s) {
    return os << reinterpret_cast<const char *>(s);
}

int main(int argc, char *argv[]) {
    assert(argc >= 1);
    if (argc < 3) {
        std::cout << "Usage:\t" << argv[0] << " input.syx output.h"
                  << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream input(argv[1], std::ios::binary);
    auto puller = cs::LambdaPuller([&](uint8_t &b) {
        b = input.get();
        return !input.eof();
    });
    cs::SerialMIDI_Parser parser;

    std::ofstream output(argv[2]);
    auto send = [&](cs::Cable cn, cs::MIDICodeIndexNumber cin, uint8_t d0,
                    uint8_t d1, uint8_t d2) {
        uint32_t m = d2;
        m = (m << 8) | d1;
        m = (m << 8) | d0;
        m = (m << 8) | (cn.getRaw() << 4) | uint8_t(cin);
        output << "    0x" << std::setw(8) << std::setfill('0') << std::hex << m
               << ",\n";
    };
    cs::USBMIDI_Sender sender;
    output << "#include <cstdint>\n"
              "\n"
              "constexpr uint32_t messages[] {\n";
    while (true) {
        auto msg = parser.pull(puller);
        if (msg == cs::MIDIReadEvent::NO_MESSAGE)
            break;
        output << "// " << enum_to_string(msg) << "\n";
        switch (msg) {
            case cs::MIDIReadEvent::CHANNEL_MESSAGE:
                sender.sendChannelMessage(parser.getChannelMessage(), send);
                break;
            case cs::MIDIReadEvent::REALTIME_MESSAGE:
                sender.sendRealTimeMessage(parser.getRealTimeMessage(), send);
                break;
            case cs::MIDIReadEvent::SYSCOMMON_MESSAGE:
                sender.sendSysCommonMessage(parser.getSysCommonMessage(), send);
                break;
            case cs::MIDIReadEvent::SYSEX_MESSAGE:
            case cs::MIDIReadEvent::SYSEX_CHUNK:
                sender.sendSysEx(parser.getSysExMessage(), send);
                break;
            default:;
        }
    }
    output << "};" << std::endl;
}