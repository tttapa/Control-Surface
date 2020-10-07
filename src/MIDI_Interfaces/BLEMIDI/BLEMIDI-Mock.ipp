#include <Def/Def.hpp>
#include <gmock-wrapper.h>
#include <string>
#include <vector>

// LCOV_EXCL_START

class BLECharacteristic {
  public:
    MOCK_METHOD(std::string, getValue, ());
    MOCK_METHOD(void, setValue, (const uint8_t * data, size_t len));
};

class BLECharacteristicCallbacks {
  public:
    virtual ~BLECharacteristicCallbacks() = default;
    virtual void onRead(BLECharacteristic *pCharacteristic) {
        (void)pCharacteristic;
    }
    virtual void onWrite(BLECharacteristic *pCharacteristic) {
        (void)pCharacteristic;
    }
};

class BLEServer {};

class BLEServerCallbacks {
  public:
    virtual ~BLEServerCallbacks() = default;
    virtual void onConnect(BLEServer *pServer) { (void)pServer; }
    virtual void onDisconnect(BLEServer *pServer) { (void)pServer; }
};

BEGIN_CS_NAMESPACE

class BLEMIDI_Mock {
  public:
    virtual void setServerCallbacks(BLEServerCallbacks *) {}
    virtual void setCharacteristicsCallbacks(BLECharacteristicCallbacks *) {}
    virtual void begin(BLEServerCallbacks *, BLECharacteristicCallbacks *) {}

    MOCK_METHOD(void, notifyValue, (std::vector<uint8_t>));
    MOCK_METHOD(std::string, getValue, ());
    void notifyValue(const uint8_t * data, size_t len) {
        if (len >= 2)
            notifyValue(std::vector<uint8_t>(data, data + len));
    }
    uint16_t get_min_mtu() const { return 23; }
};

using BLEMIDI = testing::StrictMock<BLEMIDI_Mock>;

END_CS_NAMESPACE

// LCOV_EXCL_STOP
