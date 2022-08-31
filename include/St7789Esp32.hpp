#ifndef ST7789ESP32_HPP
#define ST7789ESP32_HPP

// standard includes
#include <cstdint>

// esp32 includes

//forward declarations
class St7789Esp32;
class St7789Esp32Builder;

enum St7789Opcode{
    NOP=0x00,
    SWRESET=0x01,
    CASET=0x2a,
    RASET=0x2b,
    RAMWR=0x2c,
    COLMOD=0x3a,
    RAMWRC=0x3c,
} ;

enum St7789PixelFormat { BPP12 = 0x53, BPP16 = 0x54, BPP18 = 0x55 };

template <typename SpiTransaction>
class St7789 {
    public:
    virtual ~St7789(){};
    virtual SpiTransaction* nop()=0;
    virtual SpiTransaction* swreset()=0;
    virtual SpiTransaction* caset(uint16_t xs, uint16_t xe)=0;
    virtual SpiTransaction* raset(uint16_t ys, uint16_t ye)=0;
    virtual SpiTransaction* ramwr(uint32_t size, uint8_t *buffer)=0;
    virtual SpiTransaction* colmod(St7789PixelFormat f)=0;
    virtual SpiTransaction* ramwrc(uint32_t size, uint8_t *buffer)=0;
};

/**
 * @brief Extra data to be referenced from a spi transaction, to be processed by a pre-transaction listener.
 * 
 * An instance of St7789Esp32 should pre-instanciate this structure for each combination of flags.
 * 
 * The goal is to be able to update the DC pin and the 74x241 that will proxy the SDA pins into SDO and SDI.
 */
struct St7789TransactionExtra {
    /**
     * @brief the instance of St7789, that is able to manage its specific command pins.
     */
    St7789Esp32* target;

    /**
     * @brief flag this transaction as Data (true) or Command (false)
     * 
     */
    bool isData;

    /**
     * @brief flag this transaction as Read (true) or Write (false).
     * 
     * The Host SDO pin should go to an input pin activated by the /OE1 pin of the 74'241.
     * The screen SDA pin should go to the corresponding output.
     * The Host SDI pin should go to an output pin activated by the OE2 of the 74'241.
     * The screen SDA pin should go to the corresponding input.
     */
    bool isRead;
} ;

/**
 * @brief human friendly setup of the screen orientation, will be translated into MADCTL flags.
 *
 */
enum St7789Orientation { NORTH, EAST, WEST, SOUTH };


// project includes
/**
 * @brief
 *
 */
class St7789Esp32 {
    private:
    public:
    virtual ~St7789Esp32();
    static St7789Esp32Builder *define() { return new St7789Esp32Builder(); }
};

/** @brief What the class is for.
 */
class St7789Esp32Builder {
    private:
    int16_t dataCommandPin = -1;
    spi_device_handle_t *spiDeviceHandle;
    uint16_t width = 240;
    uint16_t height = 320;
    St7789Orientation orientation = NORTH;
    bool mirror = false;
    St7789PixelFormat pixelFormat = BPP18;

    public:
    virtual ~St7789Esp32Builder();
    St7789Esp32Builder *withDataCommandPin(int16_t p) {
        dataCommandPin = p;
        return this;
    }
    St7789Esp32Builder *withSpiDeviceHandler(spi_device_handle_t *h) {
        spiDeviceHandle = h;
        return this;
    }
    St7789Esp32Builder *withWidth(uint16_t w) {
        width = w;
        return this;
    }
    St7789Esp32Builder *withHeight(uint16_t h) {
        height = h;
        return this;
    }
    St7789Esp32Builder *withOrientation(St7789Orientation o) {
        orientation = o;
        return this;
    }
    St7789Esp32Builder *withMirror(bool m) {
        mirror = m;
        return this;
    }
    St7789Esp32Builder *withPixelFormat(St7789PixelFormat f) {
        pixelFormat = p;
        return this;
    }
    St7789Esp32 *build() {
        St7789Esp32 *result = new St7789Esp32();
        return result;
    }
};

#endif