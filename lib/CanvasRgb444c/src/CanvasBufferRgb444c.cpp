
// header include
#include "CanvasBufferRgb444c.hpp"

CanvasBufferRgb444c::CanvasBufferRgb444c(const MemoryArea &buffer, uint16_t width, uint16_t height)
    : buffer(buffer), width(width < 0x400 ? width & (0x3fe) : 0x400), height(height < 0x400 ? height & (0x3fe) : 0x3ff),
      lineOffset(width + (width >> 1)), invalidDimensions((width == 0) || (height == 0)) {
    setupColorBrushes(0, color_even, color_odd, color_both);
    setupColorBrushes(0, backgroundColor_even, backgroundColor_odd, backgroundColor_both);
    // clear buffer
    for (uint8_t *current = buffer.getStart(); current < buffer.getEnd(); current++) {
        *current = 0;
    }
}

CanvasReturnCode CanvasBufferRgb444c::hline(uint16_t x, uint16_t y, uint16_t w) {
    if (invalidDimensions)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__CANVAS_INVALID_DIMENSIONS;
    if (x >= width)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__X;
    if (y >= height)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y;
    if (w == 0)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__W_ZERO;
    if (w >= width)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__W;
    uint16_t x2 = x + w - 1;
    if (x2 >= width)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__X2;

    return unsafe_hline(x, y, w);
}

CanvasReturnCode CanvasBufferRgb444c::unsafe_hline(uint16_t x, uint16_t y, uint16_t w) {
    // move to start of working area
    uint16_t x_evenStart = x & 0x3fe; // work by pairs of pixels
    uint16_t x_pair = x_evenStart >> 1;
    uint8_t *current = buffer.getStart() + lineOffset * y + x_evenStart + x_pair;

    bool x_odd = (1 == (x & 1));
    if (w == 1) {
        changeSinglePixel(current, x_odd, color_odd, color_even);
    } else {
        if (x_odd) {
            changeSingleOddPixel(current, color_odd);
            w--;
        } else {
            changePairOfPixel(current, color_both);
            w -= 2;
        }
        current += 3;
        while (w > 1) {
            changePairOfPixel(current, color_both);
            w -= 2;
            current += 3;
        }
        if (w == 1) {
            changeSingleEvenPixel(current, color_even);
        }
    }
    return CanvasReturnCode::OK;
}

CanvasReturnCode CanvasBufferRgb444c::vline(uint16_t x, uint16_t y, uint16_t h) {
    if (invalidDimensions)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__CANVAS_INVALID_DIMENSIONS;
    if (x >= width)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__X;
    if (y >= height)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y;
    if (h == 0)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__H_ZERO;
    if (h >= height)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__H;
    uint16_t y2 = y + h - 1;
    if (y2 >= height)
        return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y2;

    return unsafe_vline(x, y, h);
}

CanvasReturnCode CanvasBufferRgb444c::unsafe_vline(uint16_t x, uint16_t y, uint16_t h) {
    // move to start of working area
    uint16_t x_evenStart = x & 0x3fe; // work by pairs of pixels
    uint16_t x_pair = x_evenStart >> 1;
    uint8_t *current = buffer.getStart() + lineOffset * y + x_evenStart + x_pair;

    bool x_odd = (1 == (x & 1));
    while (h > 0) {
        changeSinglePixel(current, x_odd, color_odd, color_even);
        h--;
        current += lineOffset;
    }
    return CanvasReturnCode::OK;
}

void CanvasBufferRgb444c::setupColorBrushes(uint16_t c, uint8_t *even, uint8_t *odd, uint8_t *both) {
    even[1] = (c << 4) & 0xf0; // B0
    odd[1] = c & 0xff;         // GB

    c = c >> 4;         // c = 00RG
    even[0] = c & 0xff; // RG

    c = c >> 4;       // c = 000R
    odd[0] = c & 0xf; // 0R

    both[0] = even[0];          // RG
    both[1] = even[1] | odd[0]; // BR
    both[2] = odd[1];           // GB
}

CanvasReturnCode CanvasBufferRgb444c::unsafe_HBrensenHamLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    // called only when :
    // * x2 > x1
    // * y2 != y1
    // * (x2 - x1) > abs(y2 - y1)

    uint16_t dx = x2 - x1;
    uint16_t dx2 = dx << 1; // = dx * 2;
    uint16_t dy2 ;
    int16_t dydraw;

    if (y2 > y1) {
        dy2 = (y2 - y1) << 1;
        dydraw = 1;
    } else {
        dy2 = (y1 - y2) << 1;
        dydraw = -1;
    }

    uint16_t xmark = x1;
    uint16_t ymark = y1;
    uint16_t len = 1;
    int16_t sigma = 0;

    for (uint16_t i = 1; i <= dx; i++) {
        sigma += dy2;
        if (sigma > dx) {
            hline(xmark, ymark, len);
            ymark += dydraw;
            xmark += len;
            len = 0;
        } else {
            len++;
        }
    }

    hline(xmark, ymark, len);
    return CanvasReturnCode::OK;
}
