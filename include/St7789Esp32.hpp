#ifndef ST7789ESP32_HPP
#define ST7789ESP32_HPP

// standard includes
#include <algorithm>
#include <cstdint>
#include <vector>

// esp32 includes

//
#include "SpiSimplistEsp32.hpp"

#define SIZE_OF_POINTER_TO_BYTE_ARRAY (sizeof(uint8_t *))

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
    virtual ~St7789Job(St7789Command *com, SpiTransaction *tcom, SpiTransaction *tdat) virtual ~st7789job() {
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
    virtual void schedule(St7789Command *command) = 0;
    virtual void schedule(std::vector<St7789Command *> *sequence) = 0;
    virtual void await(St7789Command *command) = 0;
    virtual void await(std::vector<St7789Command *> *sequence) = 0;
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
     * @brief Tells the pre-transaction listener how to setup the pins.
     *
     */
    St7789TransactionNature nature;
};

/**
 * @brief human friendly setup of the screen orientation, will be translated into MADCTL flags.
 *
 */
enum St7789Orientation { NORTH, EAST, WEST, SOUTH };

// project includes

// forward declarations
class St7789Esp32Builder;

/**
 * @brief A driver for the ST7789 on ESP32/IDF platform, to be instanciated for each connected screen.
 *
 */
class St7789Esp32 : public St7789 {
    private:
    // Transaction extras to be linked from spi transactions
    St7789TransactionExtra extraCommand = {this, COMMAND};
    St7789TransactionExtra extraDataRead = {this, DATA_READ};
    St7789TransactionExtra extraDataWrite = {this, DATA_WRITE};

    // settings
    int16_t dataCommandPin;
    int16_t readWritePin;
    spi_device_handle_t *spiDeviceHandle;
    uint16_t width;
    uint16_t height;
    St7789Orientation orientation;
    bool mirror;
    St7789PixelFormat pixelFormat;

    spi_transaction_t *commandTransactionFromCommand(St7789Command *cmd) {
        spi_transaction_t *t = new spi_transaction_t;
        std::fill(t, t + sizeof(spi_transaction_t), 0);
        t->length = 8; // always 8 bits
        t->tx_buffer = &(cmd->opcode);
        t->user = &extraCommand;
        return t;
    }

    spi_transaction_t *dataTransactionFromCommand(St7789Command *cmd) {
        spi_transaction_t *t = new spi_transaction_t;
        std::fill(t, t + sizeof(spi_transaction_t), 0);
        uint8_t *buffer = (cmd->dataLength > SIZE_OF_POINTER_TO_BYTE_ARRAY) ? cmd->externalBuffer : cmd->internalBuffer;
        if (cmd->dataDirection == WRITE) {
            t->length = 8 * cmd->dataLength;
            t->tx_buffer = buffer;
            t->user = &extraDataWrite;
        } else {
            // This is a data READ
            t->rxlength = 8 * cmd->dataLength;
            t->rx_buffer = buffer;
            t->user = &extraDataRead;
        }
        return t;
    }
    St7789Job<spi_transaction_t> *prepareJob(St7789Command *cmd) {
        return new St7789Job<spi_transaction_t>(cmd, commandTransactionFromCommand(cmd),
                                                dataTransactionFromCommand(cmd));
    }

    public:
    /**
     * @brief Instanciate a controller for a lcd screen driven by a ST7789, and send various initialization commands.
     *
     * @param dataCommandPin
     * @param readWritePin
     * @param spiDeviceHandle
     * @param width
     * @param height
     * @param orientation
     * @param mirror
     * @param pixelFormat
     */
    St7789Esp32(int16_t dataCommandPin, int16_t readWritePin, spi_device_handle_t *spiDeviceHandle, uint16_t width,
                uint16_t height, St7789Orientation orientation, bool mirror, St7789PixelFormat pixelFormat);
    virtual ~St7789Esp32();

    virtual void schedule(St7789Command *command);
    virtual void schedule(std::vector<St7789Command *> *sequence);
    virtual void await(St7789Command *command);
    virtual void await(std::vector<St7789Command *> *sequence);
};

/** @brief Allow to define an instance of St7789Esp32 with a fluent syntax.
 */
class St7789Esp32Builder {
    private:
    int16_t dataCommandPin = -1;
    int16_t readWritePin = -1;
    spi_device_handle_t *spiDeviceHandle;
    uint16_t width = 240;
    uint16_t height = 320;
    St7789Orientation orientation = NORTH;
    bool mirror = false;
    St7789PixelFormat pixelFormat = BPP18;

    public:
    virtual ~St7789Esp32Builder();
    /**
     * @brief Designate the GPIO pin number used as the data/command pin.
     *
     * @param p the GPIO pin number.
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withDataCommandPin(int16_t p) {
        dataCommandPin = p;
        return this;
    }
    /**
     * @brief Designate the GPIO pin number used as the read/write pin (tied to the /OE1 and OE2 pins of a 74'241 chip,
     * to manage the ST7789 configured with a bidirectionnal SDA -a.k.a serial interface 1 in the ST7789 datasheet-)
     *
     * @param p the GPIO pin number.
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withReadWritePin(int16_t p) {
        readWritePin = p;
        return this;
    }

    /**
     * @brief Set the esp32 spi device handler, to which actual SPI transaction will be sent.
     * 
     * @param h the handler.
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withSpiDeviceHandler(spi_device_handle_t *h) {
        spiDeviceHandle = h;
        return this;
    }

    /**
     * @brief Set the physical width of the screen.
     * 
     * @param w the width of the screen.
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withWidth(uint16_t w) {
        width = w;
        return this;
    }

    /**
     * @brief Set the physical height of the screen
     * 
     * @param h 
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withHeight(uint16_t h) {
        height = h;
        return this;
    }

    /**
     * @brief Set the physical orientation of the screen to use.
     * 
     * @param o 
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withOrientation(St7789Orientation o) {
        orientation = o;
        return this;
    }
    /**
     * @brief Set the screen so that the display is mirrored or not.
     * 
     * @param m true to activate the mirroring.
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withMirror(bool m) {
        mirror = m;
        return this;
    }
    /**
     * @brief Set the pixel format.
     * 
     * @param f the pixel format.
     * @return St7789Esp32Builder* the builder for the next step.
     */
    St7789Esp32Builder *withPixelFormat(St7789PixelFormat f) {
        pixelFormat = f;
        return this;
    }

    /**
     * @brief Create the driver instance using all the settings.
     * 
     * @return St7789Esp32* the driver instance.
     */
    St7789Esp32 *build() {
        St7789Esp32 *instance = new St7789Esp32(dataCommandPin, readWritePin, spiDeviceHandle, width, height,
                                                orientation, mirror, pixelFormat);
        return instance;
    }
};

#endif