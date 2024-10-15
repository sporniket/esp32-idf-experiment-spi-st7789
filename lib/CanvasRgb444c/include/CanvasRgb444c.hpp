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
        if (x1 >= buffer.getWidth()) return CanvasReturnCode::KO__OUT_OF_BOUNDS__X;
        if (y1 >= buffer.getHeight()) return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y;
        if (x2 >= buffer.getWidth()) return CanvasReturnCode::KO__OUT_OF_BOUNDS__X2;
        if (y2 >= buffer.getHeight()) return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y2;

        uint16_t _x1, _y1, _x2, _y2;
        if (x1 > x2) {
            _x1 = x2;
            _x2 = x1;
            _y1 = y2;
            _y2 = y1;
        } else {
            _x1 = x1;
            _x2 = x2;
            _y1 = y1;
            _y2 = y2;
        }
        int _dx = _x2 - _x1;
        int _dy = _y2 - _y2;
        int _dyAbs = (_dy < 0) ? -_dy : _dy;
        if (_dx > _dyAbs) {
            return buffer.unsafe_HBrensenHamLine(_x1, _y1, _x2, _y2);
        } else {
            return CanvasReturnCode::KO__OUT_OF_BOUNDS ;
        }
    }

    private:
    CanvasBufferRgb444c &buffer;
    Palette &palette;
};

#endif