#ifndef PIXEL_FORMAT_HPP
#define PIXEL_FORMAT_HPP

// standard includes
#include <cstdint>
#include <variant>

// esp32 includes

// project includes

// write code here

/**
 * @brief Type of pixel model.
 * ```
 *
 * Either `INDEXED` : the pixel is encoded as an indexed color between 0 included and 1<<`n` excluded, `n` being the
 * number of bits required to store the value ;
 *
 * Or `RGB` : the pixel is encoded as it's color expressed as its levels of RED, GREEN and BLUE color (additive color
 * synthesis).
 *
 */
enum class PixelType { INDEXED, RGB };

/**
 * @brief How the pixels are organized in memory.
 * ```
 *
 * Either `INTERLEAVED` : a group of pixels are coded together by destructuring their bits and gathering them togethers,
 * starting from all the least significant bits, up to the most significant ones ; for a group of bits, the leftest one
 * is for the leftest pixel, and so on.
 *
 * Or `INTERLEAVED_BE` : like `INTERLEAVED`, but the restructuring starts from the most significant bits down to the
 * least significant ones;
 *
 * Or `PROGRESSIVE` : a single pixel uses an integer number of bytes to encode its value, then comes the next pixel
 * using another cluster of bytes, and so on.
 *
 * Or `PROGRESSIVE_PACKED` : a single pixel uses an partial number of bytes to encode its value, then comes the next
 * pixel starting by finishing the last byte before continuing to the next byte, and so on. For exemple, with 4 bits per
 * pixel, two pixels can occupy the same byte.
 *
 */
enum class PixelLayout { INTERLEAVED, INTERLEAVED_BE, PROGRESSIVE, PROGRESSIVE_PACKED };

/**
 * @brief Indexed format : width of the index.
 * ```
 *
 * Immutable description of the index as its required width in bits.
 *
 */
class IndexedFormat {
    public:
    /**
     * @brief Full constructor.
     *
     * @param indexWidth the required width of the index.
     */
    IndexedFormat(uint8_t indexWidth) : indexWidth(indexWidth) {}

    /**
     * @brief Get the width of the index.
     *
     * @return uint8_t the width of the index.
     */
    uint8_t getIndexWidth() const { return indexWidth; }

    private:
    /**
     * @brief Storage of the width of the index.
     */
    const uint8_t indexWidth;
};

// Classe pour les formats RGB (avec largeur des canaux rouge, vert et bleu) - Immuable
/**
 * @brief RGB format : width of each channel value.
 * ```
 *
 * Immutable description of the RGB pixel as its required width in bits for each channel.
 *
 */
class RGBFormat {
    public:
    /**
     * @brief Full constructor.
     *
     * @param redWidth required width to store a RED level.
     * @param greenWidth required width to store a GREEN level.
     * @param blueWidth required width to store a BLUE level.
     */
    RGBFormat(uint8_t redWidth, uint8_t greenWidth, uint8_t blueWidth)
        : redWidth(redWidth), greenWidth(greenWidth), blueWidth(blueWidth) {}

    /**
     * @brief Get the width of the RED level.
     *
     * @return uint8_t the width of RED level.
     */
    uint8_t getRedWidth() const { return redWidth; }
    /**
     * @brief Get the width of the GREEN level.
     *
     * @return uint8_t the width of the GREEN level.
     */
    uint8_t getGreenWidth() const { return greenWidth; }
    /**
     * @brief Get the width of the BLUE level.
     *
     * @return uint8_t the width of the BLUE level.
     */
    uint8_t getBlueWidth() const { return blueWidth; }

    private:
    /**
     * @brief Storage of the width of the RED level.
     */
    const uint8_t redWidth;
    /**
     * @brief Storage of the width of the GREEN level.
     */
    const uint8_t greenWidth;
    /**
     * @brief Storage of the width of the BLUE level.
     */
    const uint8_t blueWidth;
};

/**
 * @brief Describe how a multiple-byte value is serialized.
 * ```
 *
 * Either `BIG_ENDIAN` : the most significant byte is stored first, the least significant one comes last.
 *
 * Or `LITTLE_ENDIAN` : the least significant byte is stored first, the most significant comes last.
 *
 */
enum class StorageEndianness { BIG_ENDIAN, LITTLE_ENDIAN };

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
    /**
     * @brief Full constructor.
     *
     * @param width the width of the unit of storage.
     * @param endianness endianness for multi-byte values.
     */
    StorageUnitFormat(uint8_t width, StorageEndianness endianness) : width(width), endianness(endianness) {}

    /**
     * @brief Get the width of the unit storage.
     *
     * @return uint8_t the width of the unit storage.
     */
    uint8_t getWidth() const { return width; }

    /**
     * @brief Get the endianness of the unit storage.
     *
     * @return StorageEndianness the endianness of the unit storage.
     */
    StorageEndianness getEndianness() const { return endianness; }

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

// Builtin storage units

/**
 * @brief Usual byte array.
 */
static const StorageUnitFormat SINGLE_BYTE(8, StorageEndianness::BIG_ENDIAN);

/**
 * @brief 16-bits value, Big endian.
 *
 * For e.g. the Atari ST, RGB555, or RGB565.
 */
static const StorageUnitFormat DOUBLE_BYTES_BE(16, StorageEndianness::BIG_ENDIAN);

/**
 * @brief 24-bits value, Big endian.
 *
 * For e.g. RGB666, RGB888.
 */
static const StorageUnitFormat TRIPLE_BYTES_BE(24, StorageEndianness::BIG_ENDIAN);

/**
 * @brief 32-bits value, Big endian.
 *
 * For e.g. 24-bits value and still keep each pixel location even-byte aligned.
 */
static const StorageUnitFormat QUADRUPLE_BYTES_BE(32, StorageEndianness::BIG_ENDIAN);

/**
 * @brief Description of the format of pixels.
 * ```
 *
 * A format of pixels is decided by choosing between indexed color value or RGB value, the number of bits required to
 * store the values, and whether the bits of the pixels are encoded together (progressive) or restructured together by
 * groups of pixels (interleaved).
 *
 * ### How to get the format
 *
 *
 *     if (rgbFormat.getType() == PixelType::RGB) {
 *         const RGBFormat& rgb = std::get<RGBFormat>(rgbFormat.getFormat());
 *         std::cout << "Accessing RGB format: Red Width = " << static_cast<int>(rgb.getRedWidth()) << "\n";
 *     }
 *
 *
 */
class PixelFormat {
    public:
    /**
     * @brief Constructor for pixels described as values designated an indexed color.
     *
     * @param indexWidth required width of the index.
     * @param layout the layout (interleaved or progressive).
     * @param storage format of the storage in memory.
     */
    PixelFormat(uint8_t indexWidth, PixelLayout layout, StorageUnitFormat storage)
        : type(PixelType::INDEXED), layout(layout), format(IndexedFormat(indexWidth)), storage(storage) {}

    // Constructeur pour le format RGB (le layout est toujours "progressive" pour RGB)
    /**
     * @brief Construct for pixels described as a mix of RED, GREEN and BLUE levels.
     *
     * @param redWidth required width of the RED level.
     * @param greenWidth required width of the GREEN level.
     * @param blueWidth required width of the BLUE level.
     * @param storage format of the storage in memory.
     */
    PixelFormat(uint8_t redWidth, uint8_t greenWidth, uint8_t blueWidth, StorageUnitFormat storage, bool packed)
        : type(PixelType::RGB), layout(packed ? PixelLayout::PROGRESSIVE_PACKED : PixelLayout::PROGRESSIVE),
          format(RGBFormat(redWidth, greenWidth, blueWidth)), storage(storage) {}

    /**
     * @brief Get the type of pixel format.
     *
     * @return PixelType the type of pixel format.
     */
    PixelType getType() const { return type; }

    /**
     * @brief Get the layout format of the pixels.
     *
     * @return PixelLayout the layout format of the pixels.
     */
    PixelLayout getLayout() const { return layout; }

    /**
     * @brief Get the logical storage description format.
     *
     * @return const std::variant<IndexedFormat, RGBFormat>& either the format of the index, or the format of the RGB
     * color.
     */
    const std::variant<IndexedFormat, RGBFormat> &getFormat() const { return format; }

    /**
     * @brief Get the format of the raw data in memory.
     *
     * @return StorageUnitFormat
     */
    StorageUnitFormat getStorage() const { return storage; }

    private:
    /**
     * @brief Storage of the type of pixel.
     */
    const PixelType type;
    /**
     * @brief Storage of the type of layout.
     */
    const PixelLayout layout;
    /**
     * @brief Logical storage description format.
     */
    const std::variant<IndexedFormat, RGBFormat> format;

    /**
     * @brief Format of the storage in memory.
     */
    const StorageUnitFormat storage;
};

// Indexed builtins formats
/**
 * @brief Monochrome pixel (1bpp) format, for e.g. OLED monochrome displays.
 */
static const PixelFormat PixelFormat_i1i_8be(1, PixelLayout::INTERLEAVED, SINGLE_BYTE);

/**
 * @brief Monochrome pixel (1bpp) format with 16bits interleaving, like so called "high resolution" on the Atari ST..
 */
static const PixelFormat PixelFormat_i1i(1, PixelLayout::INTERLEAVED, DOUBLE_BYTES_BE);

/**
 * @brief 4 colours pixel (2bpp) format with 16bits interleaving, like so called "medium resolution" on the Atari ST.
 */
static const PixelFormat PixelFormat_i2i(2, PixelLayout::INTERLEAVED, DOUBLE_BYTES_BE);

/**
 * @brief 16 colours pixel (4bpp) format with 16bits interleaving, like so called "low resolution" on the Atari ST.
 */
static const PixelFormat PixelFormat_i4i(4, PixelLayout::INTERLEAVED, DOUBLE_BYTES_BE);

/**
 * @brief 256 colours pixel (8bpp) format with 16bits interleaving, like on the Atari Falcon 030.
 */
static const PixelFormat PixelFormat_i8i(8, PixelLayout::INTERLEAVED, DOUBLE_BYTES_BE);

/**
 * @brief 256 colours pixel (8bpp) format, with progressive format like VGA resolution on PC.
 */
static const PixelFormat PixelFormat_i8p(8, PixelLayout::PROGRESSIVE, SINGLE_BYTE);

/**
 * @brief RGB pixel stored on a single byte : 8 levels of RED and GREEN, 4 levels of BLUE.
 */
static const PixelFormat PixelFormat_rgb332(3, 3, 2, SINGLE_BYTE, false);

/**
 * @brief RGB pixel stored on a 12 bits (1 byte and an half) : 16 levels of RED, GREEN and BLUE.
 *
 * That format can be used on ST7789 displays.
 */
static const PixelFormat PixelFormat_rgb332(4, 4, 4, SINGLE_BYTE, true);

/**
 * @brief RGB pixel stored on two bytes, big endian : 32 levels of RED, GREEN and BLUE.
 */
static const PixelFormat PixelFormat_rgb555(5, 5, 5, DOUBLE_BYTES_BE, false);

/**
 * @brief RGB pixel stored on two bytes, big endian : 32 levels of RED and BLUE, 64 levels of GREEN.
 *
 * Like the so called "True Color" mode of the Atari Falcon 030.
 */
static const PixelFormat PixelFormat_rgb565(5, 6, 5, DOUBLE_BYTES_BE, false);

/**
 * @brief RGB pixel stored on three bytes, big endian : 64 levels of RED, GREEN, and BLUE.
 *
 * That format can be used on ST7789 displays.
 */
static const PixelFormat PixelFormat_rgb666(6, 6, 6, TRIPLE_BYTES_BE, false);

/**
 * @brief RGB pixel stored on three bytes, big endian : 256 levels of RED, GREEN, and BLUE.
 *
 * The usual 24bpp pixel.
 */
static const PixelFormat PixelFormat_rgb888(8, 8, 8, TRIPLE_BYTES_BE, false);

#endif