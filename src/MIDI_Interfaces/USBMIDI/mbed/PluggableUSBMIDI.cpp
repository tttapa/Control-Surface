#if defined(ARDUINO_ARCH_MBED)

#include "PluggableUSBMIDI.hpp"
#include <AH/Debug/Debug.hpp>

#include <cstring> // memcpy

BEGIN_CS_NAMESPACE

// ------------------------- CONSTRUCTOR/DESTRUCTOR ------------------------- //

PluggableUSBMIDI::PluggableUSBMIDI() : PluggableUSBModule(2) {
    PluggableUSBD().plug(this);
}

PluggableUSBMIDI::~PluggableUSBMIDI() { PluggableUSBD().deinit(); }

// ----------------------------- INITIALIZATION ----------------------------- //

void PluggableUSBMIDI::init(EndpointResolver &resolver) {
    bulk_in_ep = resolver.endpoint_in(USB_EP_TYPE_BULK, get_packet_size());
    bulk_out_ep = resolver.endpoint_out(USB_EP_TYPE_BULK, get_packet_size());
    assert(resolver.valid());
}

bool PluggableUSBMIDI::connected() const {
    return usb_connected.load(std::memory_order_relaxed);
}

// --------------------------------- USB API -------------------------------- //

void PluggableUSBMIDI::callback_state_change(DeviceState new_state) {
    assert_locked();
    usb_connected.store(new_state == USBDevice::Configured,
                        std::memory_order_relaxed);
}

uint32_t PluggableUSBMIDI::callback_request(const setup_packet_t *setup,
                                            USBDevice::RequestResult *result,
                                            uint8_t **data) {
    assert_locked();
    *result = USBDevice::PassThrough;
    *data = nullptr;
    return 0;
}

bool PluggableUSBMIDI::callback_request_xfer_done(const setup_packet_t *setup,
                                                  bool aborted) {
    assert_locked();
    return false;
}

bool PluggableUSBMIDI::callback_set_configuration(uint8_t configuration) {
    assert_locked();

    PluggableUSBD().endpoint_add(
        bulk_in_ep, get_packet_size(), USB_EP_TYPE_BULK,
        mbed::callback(this, &PluggableUSBMIDI::in_callback));
    PluggableUSBD().endpoint_add(
        bulk_out_ep, get_packet_size(), USB_EP_TYPE_BULK,
        mbed::callback(this, &PluggableUSBMIDI::out_callback));

    timeout.detach();
    BulkTX::reset();
    BulkRX::reset();

    return true;
}

void PluggableUSBMIDI::callback_set_interface(uint16_t interface,
                                              uint8_t alternate) {
    assert_locked();
}

END_CS_NAMESPACE

#endif
