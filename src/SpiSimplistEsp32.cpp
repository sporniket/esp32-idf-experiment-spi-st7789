
// header include
#include "SpiSimplistEsp32.hpp"

SpiSimplistEsp32::~SpiSimplistEsp32() {}
// write code here...

HostToDevicePreTransactionListenerMapping::HostToDevicePreTransactionListenerMapping(SpiIdentifier idHost, SpiIdentifier idDevice, transaction_cb_t listener)
    : idHost(idHost), idDevice(idDevice), listener(listener) {}

SpiSimplistEsp32 *SpiSimplistEsp32Builder::build() {
    SpiSimplistEsp32 *result = new SpiSimplistEsp32();
    esp_err_t ret;
    // SPI2 aka legacy HSPI
    if (hostSpecs.count(SPI2_HOST) > 0) {
        buildHost(result, SPI2_HOST) ;
    }
    // SPI3 aka legacy VSPI
    if (hostSpecs.count(SPI3_HOST) > 0) {
        buildHost(result, SPI3_HOST) ;
    }

    return result;
}

SpiSimplistEsp32 *SpiSimplistEsp32Builder::buildHost(SpiSimplistEsp32 *spi, SpiIdentifier idHost) {
    esp_err_t ret;
    SpiHostSpecs *host = hostSpecs[idHost];
    spi_bus_config_t buscfg = {
            .mosi_io_num = host->getSerialPins()->getDataOut(),
            .miso_io_num = host->getSerialPins()->getDataIn(),
            .sclk_io_num = host->getSerialPins()->getClock(),
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 0 // 4092
    };
    ret = spi_bus_initialize((spi_host_device_t)idHost, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    for (std::map<SpiIdentifier, SpiDeviceForHostSpecs *>::iterator it = host->getDevices()->begin(); it != host->getDevices()->end(); ++it) {
        SpiDeviceForHostSpecs *device = it->second;
        spi_device_interface_config_t *devcfg = new spi_device_interface_config_t;
        devcfg->mode = 0; // FIXME give a way to setup
        devcfg->clock_speed_hz = (int)device->getClockFrequency();
        devcfg->spics_io_num = device->getSelectPin();
        devcfg->queue_size = 7; // FIXME give a way to setup
        SpiDeviceOfHostIdentifier id = SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(host->getId(), device->getId());
        if (hostToDevicePreTransactionListeners.count(id) > 0) {
            devcfg->pre_cb = hostToDevicePreTransactionListeners[id];
        }
        spi_device_handle_t *deviceHandle = new spi_device_handle_t;
        ret = spi_bus_add_device((spi_host_device_t)idHost, devcfg, deviceHandle);
        spi->registerDevice(idHost, device->getId(), deviceHandle);
    }

}
