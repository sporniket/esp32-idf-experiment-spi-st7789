#ifndef ST7789ESP32_HPP
#define ST7789ESP32_HPP

// standard includes
#include <cstdint>
#include <vector>

// esp32 includes

//
#include "SpiSimplistEsp32.hpp"

#define SIZE_OF_POINTER_TO_BYTE_ARRAY (sizeof(uint8_t *))

// forward declarations
class St7789Esp32;
class St7789Esp32Builder;

enum St7789Opcode {
    NOP = 0x00,
    SWRESET = 0x01,
    CASET = 0x2a,
    RASET = 0x2b,
    RAMWR = 0x2c,
    COLMOD = 0x3a,
    RAMWRC = 0x3c,
};

enum St7789PixelFormat { BPP12 = 0x53, BPP16 = 0x54, BPP18 = 0x55 };

/**
 * @brief attached to a native transaction to tell the pre-transaction listener what the transaction is.
 */
enum St7789TransactionNature {
    COMMAND,
    DATA_READ,
    DATA_WRITE,
};

/**
 * @brief Tells whether a command will have data to be read from or written to the controller ; when there is no data,
 * there is no direction.
 *
 */
enum St7789CommandDirection { READ, WRITE, NONE };

/**
 * @brief Describe a command : opcode and data buffer to send or buffer to receive.
 *
 */
struct St7789Command {
    /**
     * @brief the opcode to send as a COMMAND transaction.
     *
     */
    St7789Opcode opcode;

    /**
     * @brief Length of the DATA transaction.
     *
     */
    uint32_t dataLength;

    /**
     * @brief How data will move during the DATA transaction.
     *
     */
    St7789CommandDirection dataDirection;

    /**
     * @brief if true, do not delete external buffer (allocated by the developper), meaningless when using internal
     * buffer.
     *
     */
    bool keepExternalBufferOnDelete;

    /**
     * @brief Depending on the length of data.
     *
     */
    union {
        /**
         * @brief When the length of data is shorter than the size of a pointer, store the data in the structure.
         */
        uint8_t internalBuffer[SIZE_OF_POINTER_TO_BYTE_ARRAY];
        /**
         * @brief When the length of data is longer than the size of a pointer, point to an external buffer to be
         * managed.
         */
        uint8_t *externalBuffer;
    };
};

/**
 * @brief Essentially a collection of pointers that will have to be freed later.
 *
 * @tparam SpiTransaction the native type of transaction
 */
template <typename SpiTransaction> class St7789Job {
    private:
    St7789Command *command;
    SpiTransaction *transactionOfCommand;
    SpiTransaction *transactionOfData;

    public:
    St7789Job(St7789Command *cmd, SpiTransaction *tcmd, SpiTransaction *tdat)
        : command(cmd), transactionOfCommand(tcmd), transactionOfData(tdat) {}
    virtual ~St7789Job(st7789command *com, SpiTransaction *tcom, SpiTransaction *tdat) virtual ~st7789job() {
        if (command) {
            if (command->dataLength > SIZE_OF_POINTER_TO_BYTE_ARRAY && !(command->keepExternalBufferOnDelete)) {
                delete command->externalBuffer;
            }
            delete command;
        }
        if (transactionOfCommand) {
            delete transactionOfCommand;
        }
        if (transactionOfData) {
            delete transactionOfData;
        }
    }
};

class St7789 {
    private:
    // FIXME a way to manage a buffer of small data chunk, for messages longer than the length of a pointer.

    /**
     * @brief When preparing variable length command, the external buffer is unmanaged ; and if the message happens to
     * be short, it is copied into the internal buffer instead.
     *
     * @param command the command being setup.
     * @param size the size of the message.
     * @param buffer the location of the message (unmanaged memory allocation).
     */
    void setupMessageFromUnmanagedMemory(St7789Command *command, uint32_t size, uint8_t *buffer) {
        if (SIZE_OF_POINTER_TO_BYTE_ARRAY < size) {
            command->keepExternalBufferOnDelete = true; // the developper MUST manage the buffer
            command->externalBuffer = buffer;
        } else {
            uint8_t *message = command->internalBuffer;
            for (int i = 0; i < size; i++) {
                message[i] = buffer[i];
            }
        }
    }

    public:
    virtual ~St7789(){};
    St7789Command *nop() {
        St7789Command *command = new St7789Command{NOP, 0, NONE};
        return command;
    }
    St7789Command *swreset() {
        St7789Command *command = new St7789Command{SWRESET, 0, NONE};
        return command;
    }
    St7789Command *caset(uint16_t xs, uint16_t xe) {
        St7789Command *command = new St7789Command{CASET, 4, WRITE};
        uint8_t *message;
        if (SIZE_OF_POINTER_TO_BYTE_ARRAY < 4) {
            // create and fill external buffer
            command->keepExternalBufferOnDelete = false; // managed buffer
            command->externalBuffer = new (uint8_t[4]);
            message = command->externalBuffer;
        } else {
            message = command->internalBuffer;
        }
        message[0] = xs >> 8 & 0xff;
        message[1] = xs & 0xff;
        message[2] = xe >> 8 & 0xff;
        message[3] = xe & 0xff;
        return command;
    }
    St7789Command *raset(uint16_t ys, uint16_t ye) {
        St7789Command *command = new St7789Command{RASET, 4, WRITE};
        uint8_t *message;
        if (SIZE_OF_POINTER_TO_BYTE_ARRAY < 4) {
            // create and fill external buffer
            command->keepExternalBufferOnDelete = false; // managed buffer
            command->externalBuffer = new (uint8_t[4]);
            message = command->externalBuffer;
        } else {
            message = command->internalBuffer;
        }
        message[0] = ys >> 8 & 0xff;
        message[1] = ys & 0xff;
        message[2] = ye >> 8 & 0xff;
        message[3] = ye & 0xff;
        return command;
    }
    St7789Command *ramwr(uint32_t size, uint8_t *buffer) {
        St7789Command *command = new St7789Command{RAMWR, size, WRITE};
        setupMessageFromUnmanagedMemory(command, size, buffer);
        return command;
    }
    St7789Command *colmod(St7789PixelFormat f) {
        St7789Command *command = new St7789Command{COLMOD, 1, WRITE};
        uint8_t *message = command->internalBuffer;
        message[0] = f & 0xff;
        message[1] = 0;
        message[2] = 0;
        message[3] = 0;
        return command;
    }
    St7789Command *ramwrc(uint32_t size, uint8_t *buffer) {
        St7789Command *command = new St7789Command{RAMWRC, size, WRITE};
        setupMessageFromUnmanagedMemory(command, size, buffer);
        return command;
    }
};

// HERE
class St7789BatchEsp32 {
    private:
    bool immediate = true; // true = polling mode.
    vector<spi_transaction_t *> batch;

    public:
    virtual ~St7789Batch() {}
    static St7789Batch *define(){return new St7789Batch()} St7789Batch *void sendImmediately() {
        // send to the given spi in no polling mode
        //--acquire bus
        //--send
        //--free bus
    }
    void send() {
        // send and let the spi driver do its things
    }
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
    St7789Esp32 *target;

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
};

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