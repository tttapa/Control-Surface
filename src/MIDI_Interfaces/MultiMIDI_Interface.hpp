#pragma once

#include <MIDI_Interfaces/MIDI_Interface.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for grouping multiple MIDI interfaces in parallel.
 *          Output is sent to all @c N interfaces, and incoming messages are 
 *          received from all @c N interfaces.
 * 
 * @tparam  N
 *          The number of interfaces.
 */
template <uint8_t N>
class MultiMIDI_Interface : public MIDI_Interface {
  public:
    /**
     * @brief   Constructor.
     * 
     * @param   interfaces 
     *          An array of pointers to the MIDI interfaces to group together.
     *          Only the pointers are saved, the MIDI interfaces themselves must
     *          outlive this MultiMIDI_Interface.
     */
    MultiMIDI_Interface(const Array<Parsing_MIDI_Interface *, N> &interfaces)
        : interfaces(interfaces) {}

    void begin() override {
        for (auto interface : interfaces)
            interface->begin();
    }

    void update() override {
        for (auto interface : interfaces)
            interface->update();
    }

    void setCallbacks(MIDI_Callbacks *cb) override {
        for (auto interface : interfaces)
            interface->setCallbacks(cb);
    }
    using MIDI_Interface::setCallbacks;

  protected:
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2,
                  uint8_t cn) override {
        for (auto interface : interfaces)
            interface->sendImpl(m, c, d1, d2, cn);
    }

    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t cn) override {
        for (auto interface : interfaces)
            interface->sendImpl(m, c, d1, cn);
    }

    void sendImpl(const uint8_t *data, size_t length, uint8_t cn) override {
        for (auto interface : interfaces)
            interface->sendImpl(data, length, cn);
    }

    void sendImpl(uint8_t rt, uint8_t cn) override {
        for (auto interface : interfaces)
            interface->sendImpl(rt, cn);
    }

  private:
    Array<Parsing_MIDI_Interface *, N> interfaces;
};

END_CS_NAMESPACE