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
    CanvasReturnCode line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
        if (x1 >= buffer.getWidth())
            return CanvasReturnCode::KO__OUT_OF_BOUNDS__X;
        if (y1 >= buffer.getHeight())
            return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y;
        if (x2 >= buffer.getWidth())
            return CanvasReturnCode::KO__OUT_OF_BOUNDS__X2;
        if (y2 >= buffer.getHeight())
            return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y2;

        bool greaterX1 = (x1 > x2);
        bool greaterY1 = (y1 > y2);
        uint16_t dxAbs = (greaterX1) ? (x1 - x2) : (x2 - x1);
        uint16_t dyAbs = (greaterY1) ? (y1 - y2) : (y2 - y1);
        if (dxAbs > dyAbs) {
            //horizontal-ish
            if (greaterX1) {
                return buffer.unsafe_brensenhamLine(x2, y2, x1, y1, false);
            } else {
                return buffer.unsafe_brensenhamLine(x1, y1, x2, y2, false);
            }
        } else if (dyAbs > dxAbs) {
            //vertical-ish
            if (greaterY1) {
                return buffer.unsafe_brensenhamLine(y2, x2, y1, x1, true);
            } else {
                return buffer.unsafe_brensenhamLine(y1, x1, y2, x2, true);
            }
        } else {
            return CanvasReturnCode::KO__ERROR;
        }
    }

    private:
    CanvasBufferRgb444c &buffer;
    Palette &palette;
};

#endif