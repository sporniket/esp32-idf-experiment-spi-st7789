
// header include
#include "St7789Esp32.hpp"

//========================<[ St7789Esp32 ]>========================

St7789Esp32::St7789Esp32(int16_t dataCommandPin, int16_t readWritePin, spi_device_handle_t spiDeviceHandle,
                         uint16_t width, uint16_t height, St7789Orientation orientation, bool mirror,
                         St7789PixelFormat pixelFormat)
    : dataCommandPin(dataCommandPin), readWritePin(readWritePin), spiDeviceHandle(spiDeviceHandle), width(width),
      height(height), orientation(orientation), mirror(mirror), pixelFormat(pixelFormat) {
    // prepare and send the init commands.
    ESP_LOGI(TAG_ST7789_ESP32, "with DC pin %d", dataCommandPin);
    ESP_LOGI(TAG_ST7789_ESP32, "with RW pin %d", readWritePin);
    ESP_LOGI(TAG_ST7789_ESP32, "with spi device handle %p", spiDeviceHandle);
    ESP_LOGI(TAG_ST7789_ESP32, "with dimensions %d x %d", width, height);
    ESP_LOGI(TAG_ST7789_ESP32, "with orientation %d, mirroring is %d", orientation, mirror);
    ESP_LOGI(TAG_ST7789_ESP32, "with pixel format %x", pixelFormat);

    await(colmod(pixelFormat));
    ESP_LOGI(TAG_ST7789_ESP32, "DONE setting colmod");
}

St7789Esp32::~St7789Esp32() {}
// write code here...

St7789Esp32Builder *St7789Esp32::define() { return new St7789Esp32Builder(); }

void St7789Esp32::awaitWhileBusIsAcquired(St7789Command *command) {
    esp_err_t ret;
    ESP_LOGI(TAG_ST7789_ESP32, "preparing jobs for command %x", command->opcode);
    St7789Job<spi_transaction_t> *job = prepareJob(command);

    ESP_LOGI(TAG_ST7789_ESP32, "Performing command transaction");
    ret = spi_device_polling_transmit(spiDeviceHandle, job->getTransactionOfCommand());
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG_ST7789_ESP32, "Performing data transaction");
    ret = spi_device_polling_transmit(spiDeviceHandle, job->getTransactionOfData());
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG_ST7789_ESP32, "DONE command");
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
