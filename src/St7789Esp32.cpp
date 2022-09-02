
// header include
#include "St7789Esp32.hpp"

St7789Esp32::St7789Esp32(int16_t dataCommandPin, int16_t readWritePin, spi_device_handle_t *spiDeviceHandle,
                         uint16_t width, uint16_t height, St7789Orientation orientation, bool mirror,
                         St7789PixelFormat pixelFormat)
    : dataCommandPin(dataCommandPin), readWritePin(readWritePin), spiDeviceHandle(spiDeviceHandle), width(width),
      height(height), orientation(orientation), mirror(mirror), pixelFormat(pixelFormat) {
    // prepare and send the init commands.
}

St7789Esp32::~St7789Esp32() {}
// write code here...

void St7789Esp32::schedule(St7789Command *command) {}

void St7789Esp32::schedule(std::vector<St7789Command *> *sequence) {}

void St7789Esp32::await(St7789Command *command) {}

void St7789Esp32::await(std::vector<St7789Command *> *sequence) {}
