
// header include
#include "St7789Esp32.hpp"

//========================<[ St7789Esp32 ]>========================

St7789Esp32::St7789Esp32(int16_t dataCommandPin, int16_t readWritePin, spi_device_handle_t spiDeviceHandle,
                         uint16_t width, uint16_t height, St7789Orientation orientation, bool mirror,
                         St7789PixelFormat pixelFormat)
    : dataCommandPin(dataCommandPin), readWritePin(readWritePin), spiDeviceHandle(spiDeviceHandle), width(width),
      height(height), orientation(orientation), mirror(mirror), pixelFormat(pixelFormat) {
    // prepare and send the init commands.
    ESP_LOGV(TAG_ST7789_ESP32, "with DC pin %d", dataCommandPin);
    ESP_LOGV(TAG_ST7789_ESP32, "with RW pin %d", readWritePin);
    ESP_LOGV(TAG_ST7789_ESP32, "with spi device handle %p", spiDeviceHandle);
    ESP_LOGV(TAG_ST7789_ESP32, "with dimensions %d x %d", width, height);
    ESP_LOGV(TAG_ST7789_ESP32, "with orientation %d, mirroring is %d", orientation, mirror);
    ESP_LOGV(TAG_ST7789_ESP32, "with pixel format %x", pixelFormat);

    await(colmod(pixelFormat));
    await(madctl(0));
    await(porctrl(0x0c, 0x0c, false, 0x03, 0x03, 0x03, 0x03));
    await(gctrl(0x04, 0x05));
    await(vcom(0x2b));
    await(lcmctrl(0x2c));
    await(vdvvrhen(true));
    await(vrhs(0x11));
    await(vdvs(0x20));
    await(frctrl2(0x00, 0x0f));
    await(pwctrl1(2, 2, 1));
    await(pvgamctrl(0x00, 0x00, 0x05, 0x0e, 0x15, 0x0d, 0x37, 0x03, 0x04, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19, 0x0d,
                    0x00, 0x00));
    await(nvgamctrl(0x00, 0x00, 0x05, 0x0d, 0x0c, 0x06, 0x2d, 0x04, 0x04, 0x40, 0x0e, 0x1c, 0x18, 0x16, 0x19, 0x0d,
                    0x00, 0x00));
    await(slpout());
    await(dispon());
    ESP_LOGV(TAG_ST7789_ESP32, "DONE setting colmod");
}

St7789Esp32::~St7789Esp32() {}
// write code here...

St7789Esp32Builder *St7789Esp32::define() { return new St7789Esp32Builder(); }

void St7789Esp32::awaitWhileBusIsAcquired(St7789Command *command) {
    esp_err_t ret;
    ESP_LOGV(TAG_ST7789_ESP32, "preparing jobs for command %x", command->opcode);
    St7789Job<spi_transaction_t> *job = prepareJob(command);

    ESP_LOGV(TAG_ST7789_ESP32, "Performing command transaction");
    ret = spi_device_polling_transmit(spiDeviceHandle, job->getTransactionOfCommand());
    ESP_ERROR_CHECK(ret);
    if (command->dataLength > 0) {
        ESP_LOGV(TAG_ST7789_ESP32, "Performing data transaction");
        ret = spi_device_polling_transmit(spiDeviceHandle, job->getTransactionOfData());
        ESP_ERROR_CHECK(ret);
    }

    delete job ; // WARNING with read command, callee MUST provide a persistant buffer where the spi driver will write data read from device.
    ESP_LOGV(TAG_ST7789_ESP32, "DONE command");
}

void St7789Esp32::schedule(St7789Command *command) {
    // FIXME implement proper scheduling
    await(command);
}

void St7789Esp32::schedule(std::vector<St7789Command *> *sequence) {
    // FIXME implement proper scheduling
    await(sequence);
}

void St7789Esp32::await(St7789Command *command) {
    esp_err_t ret;

    ret = spi_device_acquire_bus(spiDeviceHandle, portMAX_DELAY);
    ESP_ERROR_CHECK(ret);

    awaitWhileBusIsAcquired(command);

    spi_device_release_bus(spiDeviceHandle);
}

void St7789Esp32::await(std::vector<St7789Command *> *sequence) {
    esp_err_t ret;
    ret = spi_device_acquire_bus(spiDeviceHandle, portMAX_DELAY);
    ESP_ERROR_CHECK(ret);

    for (std::vector<St7789Command *>::iterator it = sequence->begin(); it != sequence->end(); ++it) {
        awaitWhileBusIsAcquired(*it);
    }

    spi_device_release_bus(spiDeviceHandle);
}

//========================<[ St7789Esp32Builder ]>========================

St7789Esp32Builder::~St7789Esp32Builder() {}
