#include <AH/Hardware/Arduino-Hardware-Types.hpp>

// TODO
class SPIClass {
} extern SPI;

enum SPIMode {
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3,
};

class SPISettings {
  public:
    SPISettings(uint32_t, BitOrder_t, SPIMode) {}
};