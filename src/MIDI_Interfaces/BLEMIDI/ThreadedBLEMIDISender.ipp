#include "ThreadedBLEMIDISender.hpp"

#include <AH/Containers/CRTP.hpp>

BEGIN_CS_NAMESPACE

template <class Derived>
ThreadedBLEMIDISender<Derived>::~ThreadedBLEMIDISender() {
    lock_t lck(shared.mtx);
    // Tell the sender that this is the last packet
    shared.stop = true;
    // Tell the sender to not to wait for the timeout
    shared.flush = true;
    lck.unlock();
    cv.notify_one();
    // Wait for it to be sent, and join the thread when done
    if (send_thread.joinable())
        send_thread.join();
}

template <class Derived>
void ThreadedBLEMIDISender<Derived>::begin() {
    send_thread = std::thread([this] {
        // As long as you didn't get the stop signal, wait for data to send
        while (handleSendEvents())
            ; // loop
    });
}

template <class Derived>
auto ThreadedBLEMIDISender<Derived>::acquirePacket() -> ProtectedBuilder {
    return {&shared.packet, lock_t {shared.mtx}};
}

template <class Derived>
void ThreadedBLEMIDISender<Derived>::releasePacketAndNotify(
    ProtectedBuilder &lck) {
    lck.lck.unlock();
    cv.notify_one();
}

template <class Derived>
void ThreadedBLEMIDISender<Derived>::sendNow(ProtectedBuilder &lck) {
    assert(lck.lck.owns_lock());
    // No need to send empty packets
    if (shared.packet.empty())
        return;

    // Tell the background sender thread to send the packet now
    shared.flush = true;
    lck.lck.unlock();
    cv.notify_one();

    // Wait for flush to complete (when the sender clears the flush flag)
    lck.lck.lock();
    cv.wait(lck.lck, [this] { return !shared.flush; });
}

template <class Derived>
void ThreadedBLEMIDISender<Derived>::updateMTU(uint16_t mtu) {
    uint16_t force_min_mtu_c = force_min_mtu;
    if (force_min_mtu_c == 0)
        min_mtu = mtu;
    else
        min_mtu = std::min(force_min_mtu_c, mtu);
    DEBUGFN(NAMEDVALUE(min_mtu));
    auto lck = acquirePacket();
    if (lck.packet->getSize() == 0)
        lck.packet->setCapacity(min_mtu - 3);
}

template <class Derived>
void ThreadedBLEMIDISender<Derived>::forceMinMTU(uint16_t mtu) {
    force_min_mtu = mtu;
    updateMTU(min_mtu);
}

template <class Derived>
void ThreadedBLEMIDISender<Derived>::setTimeout(
    std::chrono::milliseconds timeout) {
    lock_t lck(shared.mtx);
    shared.timeout = timeout;
}

template <class Derived>
bool ThreadedBLEMIDISender<Derived>::handleSendEvents() {
    lock_t lck(shared.mtx);

    // Wait for a packet to be started (or for a stop signal)
    cv.wait(lck, [this] { return !shared.packet.empty() || shared.stop; });
    // Wait for flush signal or timeout.
    auto timeout = shared.timeout;
    cv.wait_for(lck, timeout, [this] { return shared.flush; });

    // Stop this thread
    if (shared.stop)
        return false;
    // Note: do not send anything in this case, because we might be in the base
    // class destructor, and the subclass implementing the sendData function
    // might already be destroyed.

    // Send the packet over BLE, empty the buffer, and update the buffer
    // size based on the MTU of the connected clients.
    BLEDataView data {shared.packet.getBuffer(), shared.packet.getSize()};
    if (data.length > 0)
        CRTP(Derived).sendData(data);
    shared.packet.reset();
    shared.packet.setCapacity(min_mtu - 3);
    // Note: the MTU may have been reduced asynchronously, in which case the
    // sending of the data may fail, or it may be truncated. However, since
    // updating the MTU while a transmission is already going on is rare, we
    // don't handle this case, as it would require parsing and re-encoding the
    // buffer into two or more packets.

    // Notify the main thread that the flush was done.
    if (shared.flush) {
        shared.flush = false;
        lck.unlock();
        cv.notify_one();
    }
    return true;
}

END_CS_NAMESPACE
