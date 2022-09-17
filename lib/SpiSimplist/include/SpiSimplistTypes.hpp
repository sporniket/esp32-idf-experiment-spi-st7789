#ifndef SPI_SIMPLIST_TYPES_HPP
#define SPI_SIMPLIST_TYPES_HPP

// standard includes
#include <cstddef>
#include <cstdint>
#include <map>

// esp32 includes
#include "esp_log.h"

// project includes

/**
 * @brief Identifier are used to uniquely tag a Spi entity for further
 * retrieval.
 */
using SpiIdentifier = uint16_t;

/**
 * @brief Combined identifier made from an host port identifier and a device identifier.
 */
using SpiDeviceOfHostIdentifier = uint32_t;

/**
 * @brief Macros to switch between `SpiDeviceOfHostIdentifier` and `SpiIdentifier`.
 * 
 */
class SpiIdentifierHelper {
    public:
    /**
     * @brief Convert a pair of host and device identifiers into a combined identifier.
     * 
     * @param idHost id of the host port.
     * @param idDevice id of the device.
     * @return SpiDeviceOfHostIdentifier 
     */
    static SpiDeviceOfHostIdentifier deviceOfHostIdFromIdHostIdDevice(SpiIdentifier idHost, SpiIdentifier idDevice) {
        return (((SpiDeviceOfHostIdentifier)idHost) << 16) + idDevice;
    }

    /**
     * @brief Extract the id of the host port from a combined identifier.
     * 
     * @param id the combined identifier containing the id to extract.
     * @return SpiIdentifier the extracted id of the host port.
     */
    static SpiIdentifier idHostFromDeviceOfHostId(SpiDeviceOfHostIdentifier id) { return id >> 16; }

    /**
     * @brief Extract the id of the device from a combined identifier.
     * 
     * @param id the combined identifier containing the id to extract.
     * @return SpiIdentifier the extracted id of the device.
     */
    static SpiIdentifier idDeviceFromDeviceOfHostId(SpiDeviceOfHostIdentifier id) { return id & 0xff; }
};

/**
 * @brief Representations of the different SPI clock modes (CPOL = Clock POLarity ; CPHA = Clock PHAse).
 * 
 */
enum SpiClockMode { CPOL_0_CPHA_0 = 0, CPOL_0_CPHA_1 = 1, CPOL_1_CPHA_0 = 2, CPOL_1_CPHA_1 = 3 };

#endif