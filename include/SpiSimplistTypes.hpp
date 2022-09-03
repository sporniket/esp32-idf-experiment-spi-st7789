#ifndef SPI_SIMPLIST_TYPES_HPP
#define SPI_SIMPLIST_TYPES_HPP

// standard includes
#include <cstddef>
#include <cstdint>
#include <map>

// esp32 includes
#include "esp_log.h"

// project includes
static const char *TAG_SPI_SIMPLIST_TYPES = "SpiSimplistTypes";

/**
 * @brief Identifier are used to uniquely tag a Spi entity for further
 * retrieval.
 */
using SpiIdentifier = uint16_t;

using SpiDeviceOfHostIdentifier = uint32_t;

class SpiIdentifierHelper {
    public:
    static SpiDeviceOfHostIdentifier deviceOfHostIdFromIdHostIdDevice(SpiIdentifier idHost, SpiIdentifier idDevice) {
        return (((SpiDeviceOfHostIdentifier)idHost) << 16) + idDevice;
    }
    static SpiIdentifier idHostFromDeviceOfHostId(SpiDeviceOfHostIdentifier id) { return id >> 16; }
    static SpiIdentifier idDeviceFromDeviceOfHostId(SpiDeviceOfHostIdentifier id) { return id & 0xff; }
};

/**
 * @brief Specification of the GPIO pins to use for SPI serial transmission.
 *
 */
class SpiSerialPinsMapping {
    private:
    int16_t clock = -1;
    int16_t dataIn = -1;
    int16_t dataOut = -1;

    public:
    /**
     * @brief Specify the GPIO pin mapped to SCK.
     *
     * @param gpio the gpio pin number.
     * @return SpiSerialPinsMapping* this updated mapping.
     */
    SpiSerialPinsMapping *withClock(int16_t gpio) {
        clock = gpio;
        return this;
    }

    /**
     * @brief Specify the GPIO pin mapped to SDI.
     *
     * @param gpio the gpio pin number.
     * @return SpiSerialPinsMapping* this updated mapping.
     */
    SpiSerialPinsMapping *withDataIn(int16_t gpio) {
        dataIn = gpio;
        return this;
    }

    /**
     * @brief Specify the GPIO pin mapped to SDO.
     *
     * @param gpio the gpio pin number.
     * @return SpiSerialPinsMapping* this updated mapping.
     */
    SpiSerialPinsMapping *withDataOut(int16_t gpio) {
        dataOut = gpio;
        return this;
    }

    /**
     * @brief Get the SCK pin number.
     *
     * @return int16_t the SCK pin number.
     */
    int16_t getClock() { return clock; }

    /**
     * @brief Get the SDI pin number.
     *
     * @return int16_t the SDI pin number.
     */
    int16_t getDataIn() { return dataIn; }

    /**
     * @brief Get the SDO pin number.
     *
     * @return int16_t the SDO pin number.
     */
    int16_t getDataOut() { return dataOut; }
};

/**
 * @brief Specification of a device linked to the host.
 */
class SpiDeviceForHostSpecs {
    private:
    SpiIdentifier id;
    int16_t selectPin = -1;
    uint32_t clockFrequency = 100000; // 100 kHz

    public:
    SpiDeviceForHostSpecs(SpiIdentifier id);
    SpiDeviceForHostSpecs *withSelectPin(int16_t gpio) {
        selectPin = gpio;
        return this;
    }
    SpiDeviceForHostSpecs *withClockFrequency(uint32_t frequency) {
        clockFrequency = frequency;
        return this;
    }
    SpiIdentifier getId() { return id; }
    int16_t getSelectPin() { return selectPin; }
    uint32_t getClockFrequency() { return clockFrequency; }
};

/**
 * @brief Specification of the Spi Host.
 */
class SpiHostSpecs {

    private:
    /**
     * @brief Identify the Host
     *
     */
    SpiIdentifier id;
    SpiSerialPinsMapping *serialPins;
    std::map<SpiIdentifier, SpiDeviceForHostSpecs *> devices;

    public:
    SpiHostSpecs(SpiIdentifier id);
    SpiHostSpecs *withSerialPins(SpiSerialPinsMapping *mapping) {
        serialPins = mapping;
        return this;
    }

    SpiHostSpecs *withDevice(SpiDeviceForHostSpecs *device) {
        devices[device->getId()] = device;
        return this;
    }

    SpiIdentifier getId() { return id; }

    SpiSerialPinsMapping *getSerialPins() { return serialPins; }

    bool isDefinedDevice(SpiIdentifier) { return devices.count(id) > 0; }

    SpiDeviceForHostSpecs *getDevice(SpiIdentifier id) { return isDefinedDevice(id) ? devices[id] : nullptr; }

    std::map<SpiIdentifier, SpiDeviceForHostSpecs *> *getDevices() { return &devices; }
};

class SpiSpecs {
    private:
    std::map<SpiIdentifier, SpiHostSpecs *> hosts;

    public:
    SpiSpecs *withHost(SpiHostSpecs *host) {
        hosts[host->getId()] = host;
        return this;
    }
    bool isDefinedHost(SpiIdentifier id) { return hosts.count(id) > 0; }
    SpiHostSpecs *getHost(SpiIdentifier id) { return isDefinedHost(id) ? hosts[id] : nullptr; }
};

#endif