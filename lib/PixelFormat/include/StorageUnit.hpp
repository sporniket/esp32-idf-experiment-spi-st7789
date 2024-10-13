#ifndef STORAGE_UNIT_HPP
#define STORAGE_UNIT_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes

// write code here

/**
 * @brief Describe how a multiple-byte value is serialized.
 * ```
 *
 * Either `big_endian` : the most significant byte is stored first, the least significant one comes last.
 *
 * Or `little_endian` : the least significant byte is stored first, the most significant comes last.
 *
 * Names are lower cased because unity framework define a BIG_ENDIAN macro that would collide with uppercased names.
 *
 */
enum class StorageEndianness { big_endian, little_endian };

enum class StorageUnitFormatEnum {
    /**
     * @brief Usual byte array.
     */
    SINGLE_BYTE,

    /**
     * @brief Unusual array of 12bits values. Values would be compacted to store 2 values in 3 bytes.
     *
     * This is for describing the RGB444 format of the ST7789
     */
    ONE_AND_AN_HALF_BYTE_BE,

    /**
     * @brief 16-bits value, Big endian.
     *
     * For e.g. the Atari ST, RGB555, or RGB565.
     */
    DOUBLE_BYTES_BE,

    /**
     * @brief 24-bits value, Big endian.
     *
     * For e.g. RGB666, RGB888.
     */
    TRIPLE_BYTES_BE,

    /**
     * @brief 32-bits value, Big endian.
     *
     * For e.g. 24-bits value and still keep each pixel location even-byte aligned.
     */
    QUADRUPLE_BYTES_BE
};

/**
 * @brief Description of the unit of storage in memory.
 * ```
 *
 * The unit of storage is first described by its width in bits. It is usually a multiple of 8, in other words a number
 * of bytes.
 *
 * Then, for a width requiring multiple bytes, endianness is specified.
 *
 */
class StorageUnitFormat {
    public:
    static StorageUnitFormat* getStorageUnitFormat(StorageUnitFormatEnum id);

    // ======== Code ========
    /**
     * @brief Full constructor.
     *
     * @param width the width of the unit of storage.
     * @param endianness endianness for multi-byte values.
     */
    StorageUnitFormat(uint8_t width, StorageEndianness endianness) : width(width), endianness(endianness) {}

    bool operator==(const StorageUnitFormat &rhs) const {
        return (width == rhs.width) && (endianness == rhs.endianness);
    }
    bool operator!=(const StorageUnitFormat &rhs) const { return !operator==(rhs); }

    /**
     * @brief Get the width of the unit storage.
     *
     * @return uint8_t the width of the unit storage.
     */
    const uint8_t getWidth() const { return width; }

    /**
     * @brief Get the endianness of the unit storage.
     *
     * @return StorageEndianness the endianness of the unit storage.
     */
    const StorageEndianness getEndianness() const { return endianness; }

    private:
    /**
     * @brief Storage of the width of the storage required.
     */
    const uint8_t width;

    /**
     * @brief Endianness of the storage.
     */
    const StorageEndianness endianness;
};

#endif