#ifndef ST7789_HPP
#define ST7789_HPP

// standard includes
#include <cstdint>
#include <vector>

// esp32 includes
#include "esp_log.h"

// project includes
#include "St7789Job.hpp"
#include "St7789Types.hpp"

static const char *TAG_ST7789 = "St7789";

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
        if (MUST_USE_EXTERNAL_BUFFER(size)) {
            command->keepExternalBufferOnDelete = true; // the developper MUST manage the buffer
            command->externalBuffer = buffer;
        } else {
            uint8_t *message = command->internalBuffer;
            for (int i = 0; i < size; i++) {
                message[i] = buffer[i];
            }
        }
    }

    St7789Command *newCommand(St7789Opcode opcode, uint32_t dataLength, St7789CommandDirection dataDirection,
                              bool keepExternalBufferOnDelete) {
        St7789Command *command = new St7789Command;
        command->opcode = opcode;
        command->dataLength = dataLength;
        command->dataDirection = dataDirection;
        command->keepExternalBufferOnDelete = keepExternalBufferOnDelete;
        return command;
    }

    public:
    virtual ~St7789(){};
    /**
     * @brief Perform the operation as non blocking SPI transactions, returns when it is finished.
     *
     * @param command
     */
    virtual void schedule(St7789Command *command) = 0;
    virtual void schedule(std::vector<St7789Command *> *sequence) = 0;
    virtual void await(St7789Command *command) = 0;
    virtual void await(std::vector<St7789Command *> *sequence) = 0;
    St7789Command *nop() {
        St7789Command *command = newCommand(NOP, 0, NONE_7789, false);
        return command;
    }
    St7789Command *swreset() {
        St7789Command *command = newCommand(SWRESET, 0, NONE_7789, false);
        return command;
    }
    St7789Command *caset(uint16_t xs, uint16_t xe) {
        St7789Command *command = newCommand(CASET, 4, WRITE_7789, false);
        uint8_t *message;
        if (MUST_USE_EXTERNAL_BUFFER(4)) {
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
        St7789Command *command = newCommand(RASET, 4, WRITE_7789, false);
        uint8_t *message;
        if (MUST_USE_EXTERNAL_BUFFER(4)) {
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
        St7789Command *command = newCommand(RAMWR, size, WRITE_7789, false);
        setupMessageFromUnmanagedMemory(command, size, buffer);
        return command;
    }
    St7789Command *colmod(St7789PixelFormat f) {
        St7789Command *command = newCommand(COLMOD, 1, WRITE_7789, false);
        uint8_t *message = command->internalBuffer;
        message[0] = f & 0xff;
        message[1] = 0;
        message[2] = 0;
        message[3] = 0;
        return command;
    }
    St7789Command *ramwrc(uint32_t size, uint8_t *buffer) {
        St7789Command *command = newCommand(RAMWRC, size, WRITE_7789, false);
        setupMessageFromUnmanagedMemory(command, size, buffer);
        return command;
    }
};

#endif