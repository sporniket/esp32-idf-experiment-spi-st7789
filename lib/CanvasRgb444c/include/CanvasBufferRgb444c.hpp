#ifndef CANVAS_BUFFER_RGB444C_HPP
#define CANVAS_BUFFER_RGB444C_HPP

// standard includes
#include <cstdint>

// esp32 includes

// external dependencies
#include "Color.hpp"
#include "MemoryArea.hpp"

// project includes
#include "CanvasReturnCode.hpp"

class CanvasBufferRgb444c {
    public:
    CanvasBufferRgb444c(const MemoryArea &buffer, uint16_t width, uint16_t height);

    CanvasReturnCode hline(uint16_t x, uint16_t y, uint16_t w);

    CanvasReturnCode unsafe_hline(uint16_t x, uint16_t y, uint16_t w);

    CanvasReturnCode vline(uint16_t x, uint16_t y, uint16_t h);

    CanvasReturnCode unsafe_vline(uint16_t x, uint16_t y, uint16_t h);

    CanvasReturnCode unsafe_brensenhamLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool transpose);

    void setColor(Color &color) { setupColorBrushes(color.asRgb444(), color_even, color_odd, color_both); }

    void setBackgroundColor(Color &color) {
        setupColorBrushes(color.asRgb444(), backgroundColor_even, backgroundColor_odd, backgroundColor_both);
    }

    const uint16_t getWidth() const {return width;}
    const uint16_t getHeight() const {return height;}

    private:
    void changeSinglePixel(uint8_t *pairStart, bool odd, uint8_t *c_odd, uint8_t *c_even) {
        if (odd) {
            changeSingleOddPixel(pairStart, c_odd);
        } else {
            changeSingleEvenPixel(pairStart, c_even);
        }
    }
    void changeSingleOddPixel(uint8_t *pairStart, uint8_t *c) {
        pairStart[1] = c[0] | (pairStart[1] & 0xf0);
        pairStart[2] = c[1];
    }
    void changeSingleEvenPixel(uint8_t *pairStart, uint8_t *c) {
        pairStart[0] = c[0];
        pairStart[1] = c[1] | (pairStart[1] & 0xf);
    }
    void changePairOfPixel(uint8_t *pairStart, uint8_t *c) {
        pairStart[0] = c[0];
        pairStart[1] = c[1];
        pairStart[2] = c[2];
    }

    /**
     * @brief Setup a triplet of uint8_t buffers used as brushes for basic drawing operations.
     *
     * @param c the encoded value, i.e `0RGB` (each position is a nibble).
     * @param even the brush to change a single pixel at even x coordinate (0, 2, 4,...)
     * @param odd the brush to change a single pixel at odd x coordinate (1, 3, 5,...)
     * @param both the brush to change a pair of pixels at even x coordinate (0, 2, 4,...)
     */
    void setupColorBrushes(uint16_t c, uint8_t *even, uint8_t *odd, uint8_t *both);

    const MemoryArea &buffer;
    const uint16_t width;
    const uint16_t height;
    const uint16_t lineOffset;
    const bool invalidDimensions;
    /**
     * @brief [shape color] Value to use to change single pixel at odd x coordinate (1, 3, 5,...).
     */
    uint8_t color_odd[2];
    /**
     * @brief [shape color] Value to use to change single pixel at even x coordinate (0, 2, 4,...).
     */
    uint8_t color_even[2];
    /**
     * @brief [shape color] Value to use to change a pair of pixels at even x coordinate (0, 2, 4,...).
     */
    uint8_t color_both[3];

    /**
     * @brief [background color] Value to use to change single pixel at odd x coordinate (1, 3, 5,...).
     */
    uint8_t backgroundColor_odd[2];
    /**
     * @brief [background color] Value to use to change single pixel at even x coordinate (0, 2, 4,...).
     */
    uint8_t backgroundColor_even[2];
    /**
     * @brief [background color] Value to use to change a pair of pixels at even x coordinate (0, 2, 4,...).
     */
    uint8_t backgroundColor_both[3];
};

#endif