#include "PollingBLEMIDISender.hpp"

#include <AH/Containers/CRTP.hpp>

BEGIN_CS_NAMESPACE

template <class Derived>
PollingBLEMIDISender<Derived>::~PollingBLEMIDISender() = default;

template <class Derived>
void PollingBLEMIDISender<Derived>::begin() {}

template <class Derived>
auto PollingBLEMIDISender<Derived>::acquirePacket() -> ProtectedBuilder {
    if (packet.getSize() == 0)
        packet_start_time = millis();
    return {&packet};
}

template <class Derived>
void PollingBLEMIDISender<Derived>::releasePacketAndNotify(ProtectedBuilder &lck) {
    if (lck.packet->getSize() > 0 && millis() - packet_start_time > timeout)
        sendNow(lck);
}

template <class Derived>
void PollingBLEMIDISender<Derived>::sendNow(ProtectedBuilder &lck) {
    BLEDataView data {lck.packet->getBuffer(), lck.packet->getSize()};
    if (data.length > 0) {
        CRTP(Derived).sendData(data);
        lck.packet->reset();
        lck.packet->setCapacity(min_mtu - 3);
    }
}

template <class Derived>
void PollingBLEMIDISender<Derived>::updateMTU(uint16_t mtu) {
    if (force_min_mtu == 0)
        min_mtu = mtu;
    else
        min_mtu = std::min(force_min_mtu, mtu);
    DEBUGFN(NAMEDVALUE(min_mtu));
    auto lck = acquirePacket();
    if (lck.packet->getSize() == 0)
        lck.packet->setCapacity(min_mtu - 3);
}

template <class Derived>
void PollingBLEMIDISender<Derived>::forceMinMTU(uint16_t mtu) {
    force_min_mtu = mtu;
    updateMTU(min_mtu);
}

template <class Derived>
void PollingBLEMIDISender<Derived>::setTimeout(std::chrono::milliseconds timeout) {
    this->timeout = timeout.count();
}

END_CS_NAMESPACE
