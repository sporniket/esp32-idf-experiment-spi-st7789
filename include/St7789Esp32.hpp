#ifndef ST7789ESP32_HPP
#define ST7789ESP32_HPP

// standard includes
#include <cstdint>
#include <cstring>
#include <vector>

// esp32 includes
#include "driver/spi_master.h"

// project includes
#include "St7789.hpp"

// forward declarations
class St7789Esp32;
class St7789Esp32Builder;

static const char *TAG_ST7789_ESP32 = "St7789Esp32";

/**
 * @brief Extra data to be referenced from a spi transaction, to be processed by a pre-transaction and post-transaction
 * listener.
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

    /**
     * @brief Tells the post-transaction listener to wait a little.
     *
     */
    bool requiresDelay;
};

/**
 * @brief A driver for the ST7789 on ESP32/IDF platform, to be instanciated for each connected screen.
 *
 */
class St7789Esp32 : public St7789 {
    private:
    // Transaction extras to be linked from spi transactions
    St7789TransactionExtra extraCommand = {this, COMMAND, false};
    St7789TransactionExtra extraCommandWithDelay = {this, COMMAND, true};
    St7789TransactionExtra extraDataRead = {this, DATA_READ, false};
    St7789TransactionExtra extraDataWrite = {this, DATA_WRITE, false};
    St7789TransactionExtra extraDataWriteWithDelay = {this, DATA_WRITE, true};

    // settings
    int16_t dataCommandPin;
    int16_t readWritePin;
    spi_device_handle_t spiDeviceHandle;
    uint16_t width;
    uint16_t height;
    St7789Orientation orientation;
    bool mirror;
    St7789PixelFormat pixelFormat;

    bool isCommandWithDelay(St7789Opcode opcode) {
        switch (opcode) {
        case SWRESET:
        case SLPOUT:
        case DISPON:
            return true;

        default:
            return false;
        }
    }

    spi_transaction_t *commandTransactionFromCommand(St7789Command *cmd) {
        spi_transaction_t *t = new spi_transaction_t;
        t->flags = 0; // FIXME
        t->cmd = 0;
        t->addr = 0;
        t->length = 8; // always 8 bits
        t->rxlength = 0;
        t->user = isCommandWithDelay(cmd->opcode) ? &extraCommandWithDelay : &extraCommand;
        t->tx_buffer = &(cmd->opcode);
        t->rx_buffer = nullptr;
        return t;
    }

    spi_transaction_t *dataTransactionFromCommand(St7789Command *cmd) {
        spi_transaction_t *t = new spi_transaction_t;
        t->flags = 0; // FIXME
        t->cmd = 0;
        t->addr = 0;
        uint8_t *buffer = MUST_USE_EXTERNAL_BUFFER(cmd->dataLength) ? cmd->externalBuffer : cmd->internalBuffer;
        if (cmd->dataDirection == St7789CommandDirection::WRITE_7789) {
            t->length = 8 * cmd->dataLength;
            t->rxlength = 0;
            t->user = &extraDataWrite;
            t->tx_buffer = buffer;
            t->rx_buffer = nullptr;
        } else {
            // This is a data READ
            t->length = 0;
            t->rxlength = 8 * cmd->dataLength;
            t->user = &extraDataRead;
            t->tx_buffer = nullptr;
            t->rx_buffer = buffer;
        }
        return t;
    }
    St7789Job<spi_transaction_t> *prepareJob(St7789Command *cmd) {
        return new St7789Job<spi_transaction_t>(cmd,                                //
                                                commandTransactionFromCommand(cmd), //
                                                cmd->dataLength > 0 ? dataTransactionFromCommand(cmd) : nullptr);
    }

    void awaitWhileBusIsAcquired(St7789Command *command);

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
    St7789Esp32(int16_t dataCommandPin, int16_t readWritePin, spi_device_handle_t spiDeviceHandle, uint16_t width,
                uint16_t height, St7789Orientation orientation, bool mirror, St7789PixelFormat pixelFormat);
    virtual ~St7789Esp32();

    virtual void schedule(St7789Command *command);
    virtual void schedule(std::vector<St7789Command *> *sequence);
    virtual void await(St7789Command *command);
    virtual void await(std::vector<St7789Command *> *sequence);

    int16_t getDataCommandPin() { return dataCommandPin; }
    int16_t getReadWritePin() { return readWritePin; }
    static St7789Esp32Builder *define();
};

/** @brief Allow to define an instance of St7789Esp32 with a fluent syntax.
 */
class St7789Esp32Builder {
    private:
    int16_t dataCommandPin = -1;
    int16_t readWritePin = -1;
    spi_device_handle_t spiDeviceHandle;
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
     * to manage the ST7789 configured with a bidirectionnal SDA -a.k.a serial interface 1 in the ST7789 datasheet-,
     * those pins are set to HIGH for reading and LOW for writing)
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
    St7789Esp32Builder *withSpiDeviceHandler(spi_device_handle_t h) {
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