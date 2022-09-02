
// header include
#include "St7789Esp32.hpp"

St7789Esp32::St7789Esp32(int16_t dataCommandPin, int16_t readWritePin, spi_device_handle_t spiDeviceHandle,
                         uint16_t width, uint16_t height, St7789Orientation orientation, bool mirror,
                         St7789PixelFormat pixelFormat)
    : dataCommandPin(dataCommandPin), readWritePin(readWritePin), spiDeviceHandle(spiDeviceHandle), width(width),
      height(height), orientation(orientation), mirror(mirror), pixelFormat(pixelFormat) {
    // prepare and send the init commands.
}

St7789Esp32::~St7789Esp32() {}
// write code here...

void St7789Esp32::awaitWhileBusIsAcquired(St7789Command *command) {
    esp_err_t ret;
    St7789Job<spi_transaction_t> * job = prepareJob(command) ;

    ret = spi_device_polling_transmit(spiDeviceHandle, job->getTransactionOfCommand()) ;
    ESP_ERROR_CHECK(ret);
    ret = spi_device_polling_transmit(spiDeviceHandle, job->getTransactionOfData()) ;
    ESP_ERROR_CHECK(ret);
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

    ret = spi_device_acquire_bus(spiDeviceHandle, portMAX_DELAY) ;
    ESP_ERROR_CHECK(ret);

    awaitWhileBusIsAcquired(command) ;

    spi_device_release_bus(spiDeviceHandle);
}

void St7789Esp32::await(std::vector<St7789Command *> *sequence) {
    esp_err_t ret;
    ret = spi_device_acquire_bus(spiDeviceHandle, portMAX_DELAY) ;
    ESP_ERROR_CHECK(ret);
    
    for(std::vector<St7789Command *>::iterator it = sequence->begin() ; it != sequence->end() ; ++it) {
        awaitWhileBusIsAcquired(*it) ;
    }

    spi_device_release_bus(spiDeviceHandle);
}
