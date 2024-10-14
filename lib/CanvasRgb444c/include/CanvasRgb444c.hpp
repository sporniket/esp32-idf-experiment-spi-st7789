#ifndef CANVAS_RGB444C_HPP
#define CANVAS_RGB444C_HPP

// standard includes
#include <cstdint>

// esp32 includes

// external dependencies
#include "Palette.hpp"

// project includes
#include "CanvasBufferRgb444c.hpp"

class CanvasRgb444c {
    public:
    CanvasRgb444c(CanvasBufferRgb444c &buffer, Palette &palette) : buffer(buffer), palette(palette) {
        if (this->palette.size() > 0) {
            this->buffer.setColor(palette.back());
        }
    }

    CanvasReturnCode plot(uint16_t x, uint16_t y) { return buffer.hline(x, y, 1); }

    private:
    CanvasBufferRgb444c &buffer;
    Palette &palette;
};

#endif