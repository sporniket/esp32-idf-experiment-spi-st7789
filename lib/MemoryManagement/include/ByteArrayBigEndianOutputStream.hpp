#ifndef BYTE_ARRAY_BIG_ENDIAN_OUTPUT_STREAM_HPP
#define BYTE_ARRAY_BIG_ENDIAN_OUTPUT_STREAM_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes
#include "MemoryArea.hpp"

//write code here
enum class ByteArrayStreamStatus {
    E_OK,
    E_KO__OUT_OF_BOUND
};

class ByteArrayBigEndianOutputStream {
public:
    ByteArrayBigEndianOutputStream(MemoryArea &area) 
        : area(area), position(area.getStart()), status(ByteArrayStreamStatus::E_OK), length(0) {}

    ByteArrayStreamStatus getStatus() const {
        return status;
    }

    uint32_t getLength() const {
        return length;
    }

    void reset() {
        position = area.getStart();
        status = ByteArrayStreamStatus::E_OK;
        length = 0;
    }

    template <typename T>
    ByteArrayBigEndianOutputStream& operator<<(const T& value) {
        uint8_t numBytes = sizeof(T);

        if (position + numBytes > area.getEnd()) {
            status = ByteArrayStreamStatus::E_KO__OUT_OF_BOUND;
            return *this;
        }

        // Copier les octets en big-endian (octet le plus significatif en premier)
        for (int i = numBytes - 1; i >= 0; --i) {
            *position++ = (value >> (i * 8)) & 0xFF;
        }

        length += numBytes;
        return *this;
    }

private:
    MemoryArea &area;
    uint8_t *position;
    ByteArrayStreamStatus status;
    uint32_t length;
};
#endif