#ifdef FOOBAR

gap_advertisements_set_params

    static const uint8_t adv_sid = 0;
static le_advertising_set_t le_advertising_set;
static uint8_t adv_handle = 0;

static const le_extended_advertising_parameters_t extended_params = {
    .advertising_event_properties = 1,         // connectable
    .primary_advertising_interval_min = 0x4b0, // 750 ms
    .primary_advertising_interval_max = 0x4b0, // 750 ms
    .primary_advertising_channel_map = 7,
    .own_address_type = 0,
    .peer_address_type = 0,
    .peer_address = {0},
    .advertising_filter_policy = 0,
    .advertising_tx_power = 10,   // 10 dBm
    .primary_advertising_phy = 1, // LE 1M PHY
    .secondary_advertising_max_skip = 0,
    .secondary_advertising_phy = 1, // LE 1M PHY
    .advertising_sid = adv_sid,
    .scan_request_notification_enable = 0,
};

static const uint8_t extended_adv_data[] = {
    // 16 bit service data, ORG_BLUETOOTH_SERVICE_BROADCAST_AUDIO_SCAN_SERVICE,
    3, BLUETOOTH_DATA_TYPE_SERVICE_DATA_16_BIT_UUID,
    ORG_BLUETOOTH_SERVICE_BROADCAST_AUDIO_SCAN_SERVICE & 0xff,
    ORG_BLUETOOTH_SERVICE_BROADCAST_AUDIO_SCAN_SERVICE >> 8,
    // name
    5, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'S', 'i', 'n', 'k'};

static void setup_advertising() {
    gap_extended_advertising_setup(&le_advertising_set, &extended_params,
                                   &adv_handle);
    gap_extended_advertising_set_adv_data(adv_handle, sizeof(extended_adv_data),
                                          extended_adv_data);
    gap_extended_advertising_start(adv_handle, 0, 0);
}

uint8_t gap_extended_advertising_set_scan_response_data(
    uint8_t advertising_handle, uint16_t scan_response_data_length,
    const uint8_t *scan_response_data);

#endif
