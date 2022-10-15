// Copyright 2022 David SPORN
// ---
// This file is part of 'esp32-idf-experiment-spi-st7789'.
// ---
// 'esp32-idf-experiment-spi-st7789' is free software: you can redistribute it and/or 
// modify it under the terms of the GNU General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.

// 'esp32-idf-experiment-spi-st7789' is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 
// Public License for more details.

// You should have received a copy of the GNU General Public License along 
// with 'esp32-idf-experiment-spi-st7789'. If not, see <https://www.gnu.org/licenses/>. 
#ifndef SPI_SIMPLIST_SPECS_HPP
#define SPI_SIMPLIST_SPECS_HPP

// standard includes
#include <cstddef>
#include <cstdint>
#include <map>

// esp32 includes
#include "esp_log.h"

// project includes
#include "SpiSimplistTypes.hpp"

/**
 * @brief Specification of the GPIO pins to use for SPI serial transmission.
 *
 */
class SpiSerialPinsMappingSpecs {
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
    SpiSerialPinsMappingSpecs *withClock(int16_t gpio) {
        clock = gpio;
        return this;
    }

    /**
     * @brief Specify the GPIO pin mapped to SDI.
     *
     * @param gpio the gpio pin number.
     * @return SpiSerialPinsMapping* this updated mapping.
     */
    SpiSerialPinsMappingSpecs *withDataIn(int16_t gpio) {
        dataIn = gpio;
        return this;
    }

    /**
     * @brief Specify the GPIO pin mapped to SDO.
     *
     * @param gpio the gpio pin number.
     * @return SpiSerialPinsMapping* this updated mapping.
     */
    SpiSerialPinsMappingSpecs *withDataOut(int16_t gpio) {
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
    SpiClockMode clockMode = SpiClockMode::CPOL_0_CPHA_0;
    void *extraSpecs;

    public:
    SpiDeviceForHostSpecs(SpiIdentifier id);
    /**
     * @brief Define the GPIO to select the device.
     * 
     * @param gpio 
     * @return SpiDeviceForHostSpecs* 
     */
    SpiDeviceForHostSpecs *withSelectPin(int16_t gpio) {
        selectPin = gpio;
        return this;
    }
    /**
     * @brief Defines the clock frequency, in Hertz.
     * 
     * @param frequency 
     * @return SpiDeviceForHostSpecs* 
     */
    SpiDeviceForHostSpecs *withClockFrequency(uint32_t frequency) {
        clockFrequency = frequency;
        return this;
    }
    /**
     * @brief Defines the clock polarity and phase.
     * 
     * @param mode 
     * @return SpiDeviceForHostSpecs* 
     */
    SpiDeviceForHostSpecs *withClockMode(SpiClockMode mode) {
        clockMode = mode;
        return this;
    }
    /**
     * @brief Set extra specifications, usually platform dependant.
     *
     * @param extra 
     * @return SpiDeviceForHostSpecs*
     */
    SpiDeviceForHostSpecs *withExtraSpecs(void *extra) {
        extraSpecs = extra;
        return this;
    }

    SpiIdentifier getId() { return id; }
    int16_t getSelectPin() { return selectPin; }
    uint32_t getClockFrequency() { return clockFrequency; }
    SpiClockMode getClockMode() { return clockMode; }
    void *getExtraSpecs() { return extraSpecs; }
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
    SpiSerialPinsMappingSpecs *serialPins;
    std::map<SpiIdentifier, SpiDeviceForHostSpecs *> devices;

    public:
    SpiHostSpecs(SpiIdentifier id);
    SpiHostSpecs *withSerialPins(SpiSerialPinsMappingSpecs *mapping) {
        serialPins = mapping;
        return this;
    }

    SpiHostSpecs *withDevice(SpiDeviceForHostSpecs *device) {
        devices[device->getId()] = device;
        return this;
    }

    SpiIdentifier getId() { return id; }

    SpiSerialPinsMappingSpecs *getSerialPins() { return serialPins; }

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