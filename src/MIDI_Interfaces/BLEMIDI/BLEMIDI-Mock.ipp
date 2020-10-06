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

class BLEMIDI {
  public:
    MOCK_METHOD(void, setServerCallbacks, (BLEServerCallbacks *));
    MOCK_METHOD(void, setCharacteristicsCallbacks,
                (BLECharacteristicCallbacks *));
    MOCK_METHOD(void, begin,
                (BLEServerCallbacks *, BLECharacteristicCallbacks *));
    MOCK_METHOD(void, notifyValue, (std::vector<uint8_t>));
    MOCK_METHOD(std::string, getValue, ());
    void notifyValue(const uint8_t * data, size_t len) {
        notifyValue(std::vector<uint8_t>(data, data + len));
    }
    uint16_t get_min_mtu() const { return 23; }
};

END_CS_NAMESPACE

// LCOV_EXCL_STOP
