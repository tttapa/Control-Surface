#include "TeensyHostMIDI.hpp"

BEGIN_CS_NAMESPACE

// ------------------------------- INITIALIZATION ------------------------------

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::init() {
    contribute_Pipes(mypipes, len(mypipes));
    contribute_Transfers(mytransfers, len(mytransfers));
    contribute_String_Buffers(mystring_bufs, len(mystring_bufs));
    rxpipe = nullptr;
    txpipe = nullptr;
    driver_ready_for_device(this);
}

template <uint16_t MaxPacketSize>
bool TeensyHostMIDI<MaxPacketSize>::claim(Device_t *dev, int type,
                                          const uint8_t *descriptors,
                                          uint32_t len) {
    // only claim at interface level
    if (type != 1)
        return false;

    if (!claim_if_midi(dev, type, descriptors, len)) {
        USBHost::println_("This interface is not MIDI");
        return false;
    }
    USBHost::println_("claimed");

    // if an IN endpoint was found, create its pipe
    if (rx_ep && rx_size <= MaxPacketSize) {
        rxpipe = new_Pipe(dev, rx_ep_type, rx_ep, 1, rx_size);
        if (rxpipe) {
            rxpipe->callback_function = rx_callback;
            BulkRX<TeensyHostMIDI, uint32_t, MaxPacketSize>::reset(rx_size);
        }
    } else {
        rxpipe = nullptr;
    }

    // if an OUT endpoint was found, create its pipe
    if (tx_ep && tx_size <= MaxPacketSize) {
        txpipe = new_Pipe(dev, tx_ep_type, tx_ep, 0, tx_size);
        if (txpipe) {
            txpipe->callback_function = tx_callback;
            write_timeout.stop();
            BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::reset(rx_size);
        }
    } else {
        txpipe = nullptr;
    }

    // claim if either pipe created
    return rxpipe || txpipe;
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::disconnect() {
    rxpipe = nullptr;
    txpipe = nullptr;
}

// ------------------------------ USB CALLBACKS ----------------------------- //

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::rx_callback(const Transfer_t *transfer) {
    if (transfer->driver)
        reinterpret_cast<TeensyHostMIDI *>(transfer->driver)
            ->out_callback(transfer);
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::tx_callback(const Transfer_t *transfer) {
    if (transfer->driver)
        reinterpret_cast<TeensyHostMIDI *>(transfer->driver)
            ->in_callback(transfer);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::timer_event(USBDriverTimer *whichtimer) {
    if (whichtimer == &write_timeout)
        timeout_callback();
}

// ----------------------------- USB READ/WRITE ----------------------------- //

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::write_start(uint8_t *buffer,
                                                uint32_t size) {
    // digitalWrite(3, HIGH);
    if (txpipe)
        queue_Data_Transfer(txpipe, buffer, size, this);
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::write_start_isr(uint8_t *buffer,
                                                    uint32_t size) {
    // digitalWrite(3, HIGH);
    if (txpipe)
        queue_Data_Transfer(txpipe, buffer, size, this);
}
template <uint16_t MaxPacketSize>
uint32_t
TeensyHostMIDI<MaxPacketSize>::write_finish(const Transfer_t *transfer) {
    // digitalWrite(3, LOW);
    // delayMicroseconds(200);
    return transfer->length - ((transfer->qtd.token >> 16) & 0x7FFF);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::read_start(uint8_t *buffer, uint32_t size) {
    if (rxpipe)
        queue_Data_Transfer(rxpipe, buffer, size, this);
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::read_start_isr(uint8_t *buffer,
                                                   uint32_t size) {
    if (rxpipe)
        queue_Data_Transfer(rxpipe, buffer, size, this);
}
template <uint16_t MaxPacketSize>
uint32_t
TeensyHostMIDI<MaxPacketSize>::read_finish(const Transfer_t *transfer) {
    return transfer->length - ((transfer->qtd.token >> 16) & 0x7FFF);
}

// ---------------------------------- WRITING ----------------------------------

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::start_timeout() {
    write_timeout.start(write_timeout_duration.count());
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::cancel_timeout() {
    write_timeout.stop();
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::tx_start(const void *data, uint32_t size) {
    write_start(reinterpret_cast<uint8_t *>(const_cast<void *>(data)), size);
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::tx_start_isr(const void *data,
                                                 uint32_t size) {
    write_start_isr(reinterpret_cast<uint8_t *>(const_cast<void *>(data)),
                    size);
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::tx_start_timeout(const void *data,
                                                     uint32_t size) {
    tx_start_isr(data, size);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::in_callback(const Transfer_t *) {
    BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::tx_callback();
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::timeout_callback() {
    BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::timeout_callback();
}

// ---------------------------------- READING ----------------------------------

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::rx_start(void *data, uint32_t size) {
    read_start(reinterpret_cast<uint8_t *>(data), size);
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::rx_start_isr(void *data, uint32_t size) {
    read_start_isr(reinterpret_cast<uint8_t *>(data), size);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::out_callback(const Transfer_t *transfer) {
    uint32_t bytes_read = read_finish(transfer);
    BulkRX<TeensyHostMIDI, uint32_t, MaxPacketSize>::rx_callback(bytes_read);
}

END_CS_NAMESPACE

#include "TeensyHostMIDI-claim.ipp"
