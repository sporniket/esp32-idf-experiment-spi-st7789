
// header include
#include "SpiSimplistEsp32.hpp"

//========================<[ SpiSimplistEsp32 ]>========================

SpiSimplistEsp32::~SpiSimplistEsp32() {}
// write code here...

SpiSimplistEsp32Builder *SpiSimplistEsp32::define() { return new SpiSimplistEsp32Builder(); }

//========================<[ HostToDevicePreTransactionListenerMapping ]>========================
HostToDevicePreTransactionListenerMapping::HostToDevicePreTransactionListenerMapping(SpiIdentifier idHost,
                                                                                     SpiIdentifier idDevice,
                                                                                     transaction_cb_t listener)
    : idHost(idHost), idDevice(idDevice), listener(listener) {}

//========================<[ SpiSimplistEsp32Builder ]>========================

SpiSimplistEsp32Builder::~SpiSimplistEsp32Builder() {}

SpiSimplistEsp32 *SpiSimplistEsp32Builder::build() {
    SpiSimplistEsp32 *result = new SpiSimplistEsp32();
    // SPI2 aka legacy HSPI
    if (hostSpecs.count(SPI2_HOST) > 0) {
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32, "will build SPI2_HOST...");
        buildHost(result, SPI2_HOST);
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32, "DONE build SPI2_HOST...");
    }
    // SPI3 aka legacy VSPI
    if (hostSpecs.count(SPI3_HOST) > 0) {
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32, "will build SPI3_HOST...");
        buildHost(result, SPI3_HOST);
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32, "DONE build SPI3_HOST...");
    }

    return result;
}

void SpiSimplistEsp32Builder::buildHost(SpiSimplistEsp32 *spi, SpiIdentifier idHost) {
    esp_err_t ret;
    SpiHostSpecs *host = hostSpecs[idHost];
    spi_bus_config_t buscfg = {.mosi_io_num = host->getSerialPins()->getDataOut(),
                               .miso_io_num = host->getSerialPins()->getDataIn(),
                               .sclk_io_num = host->getSerialPins()->getClock(),
                               .quadwp_io_num = -1,
                               .quadhd_io_num = -1,
                               .data4_io_num = -1,
                               .data5_io_num = -1,
                               .data6_io_num = -1,
                               .data7_io_num = -1,
                               .max_transfer_sz = 0, // 4092
                               .flags = SPICOMMON_BUSFLAG_MASTER,
                               .intr_flags = 0};
    ESP_LOGV(TAG_SPI_SIMPLIST_ESP32, "will spi_bus_initialize...");
    ret = spi_bus_initialize((spi_host_device_t)idHost, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    ESP_LOGV(TAG_SPI_SIMPLIST_ESP32, "will browse device...");
    for (std::map<SpiIdentifier, SpiDeviceForHostSpecs *>::iterator it = host->getDevices()->begin();
         it != host->getDevices()->end(); ++it) {
        ESP_LOGV(TAG_SPI_SIMPLIST_ESP32, "Preparing device #%d...", it->first);
        SpiDeviceForHostSpecs *device = it->second;
        spi_device_interface_config_t *devcfg = new spi_device_interface_config_t;
        devcfg->command_bits = 0;
        devcfg->address_bits = 0;
        devcfg->dummy_bits = 0;
        devcfg->mode = 0;           // FIXME give a way to setup
        devcfg->duty_cycle_pos = 0; // default to 50/50
        devcfg->cs_ena_pretrans = 0;
        devcfg->cs_ena_posttrans = 0;
        devcfg->clock_speed_hz = (int)device->getClockFrequency();
        devcfg->input_delay_ns = 0;
        devcfg->spics_io_num = device->getSelectPin();
        devcfg->flags =
                SPI_DEVICE_HALFDUPLEX |
                SPI_DEVICE_NO_DUMMY, // FIXME give a way to setup // SPI_DEVICE_3WIRE for ST7789 with serial interface 1
                devcfg->queue_size = 7; // FIXME give a way to setup
        SpiDeviceOfHostIdentifier id =
                SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(host->getId(), device->getId());
        if (hostToDevicePreTransactionListeners.count(id) > 0) {
            devcfg->pre_cb = hostToDevicePreTransactionListeners[id];
        } else {
            devcfg->pre_cb = nullptr;
        }
        if (hostToDevicePostTransactionListeners.count(id) > 0) {
            devcfg->post_cb = hostToDevicePostTransactionListeners[id];
        } else {
            devcfg->post_cb = nullptr;
        }
        spi_device_handle_t deviceHandle;
        ret = spi_bus_add_device((spi_host_device_t)idHost, devcfg, &deviceHandle);
        spi->registerDevice(idHost, device->getId(), deviceHandle);
    }
}
