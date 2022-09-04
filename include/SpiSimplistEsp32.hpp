#ifndef SPI_SIMPLIST_ESP32_HPP
#define SPI_SIMPLIST_ESP32_HPP

// standard includes
#include <cstdint>
#include <map>

// esp32 includes
#include "driver/spi_master.h"

// project includes
#include "SpiSimplist.hpp"

static const char *TAG_SPI_SIMPLIST_ESP32 = "SpiSimplistEsp32";

// typedef void HostToDevicePreTransactionListener(spi_transaction_t *t);

class HostToDevicePreTransactionListenerMapping {
    private:
    SpiIdentifier idHost;
    SpiIdentifier idDevice;
    transaction_cb_t listener;

    public:
    HostToDevicePreTransactionListenerMapping(SpiIdentifier idHost, SpiIdentifier idDevice, transaction_cb_t listener);
    SpiIdentifier getIdHost() { return idHost; }
    SpiIdentifier getIdDevice() { return idDevice; }
    transaction_cb_t getListener() { return listener; }
};

class SpiSimplistEsp32Builder;

/** @brief What the class is for.
 */
class SpiSimplistEsp32 {
    private:
    std::map<SpiDeviceOfHostIdentifier, spi_device_handle_t> deviceHandlesOfSpi2;

    public:
    virtual ~SpiSimplistEsp32();
    static SpiSimplistEsp32Builder *define();
    void registerDevice(SpiIdentifier idHost, SpiIdentifier idDevice, spi_device_handle_t device) {
        deviceHandlesOfSpi2[SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(idHost, idDevice)] = device;
    }
    spi_device_handle_t getDevice(SpiIdentifier idHost, SpiIdentifier idDevice) {
        SpiDeviceOfHostIdentifier id = SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(idHost, idDevice);
        return (deviceHandlesOfSpi2.count(id) > 0) ? deviceHandlesOfSpi2[id] : nullptr;
    }
};

class SpiSimplistEsp32Builder {
    private:
    std::map<SpiIdentifier, SpiHostSpecs *> hostSpecs;
    std::map<SpiDeviceOfHostIdentifier, transaction_cb_t> hostToDevicePreTransactionListeners;
    std::map<SpiDeviceOfHostIdentifier, transaction_cb_t> hostToDevicePostTransactionListeners;
    void buildHost(SpiSimplistEsp32 *spi, SpiIdentifier idHost);

    public:
    virtual ~SpiSimplistEsp32Builder();
    SpiSimplistEsp32Builder *withHostSpecs(SpiIdentifier id, SpiSerialPinsMapping *serialPins) {
        
        hostSpecs[id] = (new SpiHostSpecs(id)) //
                                ->withSerialPins(serialPins);
        return this;
    }
    SpiSimplistEsp32Builder *withDeviceForHostSpecs(SpiIdentifier id, SpiDeviceForHostSpecs *specs) {
        if (hostSpecs.count(id) == 0) {
            // FIXME: log error
            // silently do nothing
            return this;
        }
        hostSpecs[id]->withDevice(specs);
        return this;
    }
    SpiSimplistEsp32Builder *withPreTransactionListener(SpiIdentifier idHost, SpiIdentifier idDevice,
                                                        transaction_cb_t listener) {
        hostToDevicePreTransactionListeners[SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(idHost, idDevice)] =
                listener;
        return this;
    }
    SpiSimplistEsp32Builder *withPostTransactionListener(SpiIdentifier idHost, SpiIdentifier idDevice,
                                                        transaction_cb_t listener) {
        hostToDevicePostTransactionListeners[SpiIdentifierHelper::deviceOfHostIdFromIdHostIdDevice(idHost, idDevice)] =
                listener;
        return this;
    }
    SpiSimplistEsp32 *build();
};

#endif