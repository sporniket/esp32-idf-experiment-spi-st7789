#ifndef FRAME_BUFFER12BPP_UNSAFE_HPP
#define FRAME_BUFFER12BPP_UNSAFE_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes
#include "ColorRgb48Bpp.hpp"
#include "FrameBuffer.hpp"

/** @brief Private implementation of FrameBuffer with a pixel model having 12 bits per pixels (rgb-444). All checks has
 * been performed outside this class, thus `x1 < x2`, `y1 < y2` and all coordinates falls inside the framebuffer.
 */
class FrameBuffer12BppUnsafe {
    private:
    /**
     * @brief Horizontal dimension in pixels.
     *
     * Valid `x` coordinates range from `0` to `width - 1`.
     */
    uint32_t width;
    /**
     * @brief Vertical dimension in pixels.
     *
     * Valid `y` coordinates range from `0` to `width - 1`.
     */
    uint32_t height;
    /**
     * @brief Number of subpixels (red, green, blue) in a lines.
     *
     * `widthInSubPixels = 3 * width`
     */
    uint32_t widthInSubPixels;
    /**
     * @brief Active color.
     */
    ColorRgb48Bpp activeColor{0, 0, 0};
    /**
     * @brief The buffer story the frame data
     */
    uint8_t *buffer;
    /**
     * @brief 12bpp precomputed data for a pixel.
     *
     * Element 0 for byte at even position starting from the buffer (to copy, pixel at even y coordinate) ;
     * element 1 for byte at odd posision starting from the buffer (either use the most significant
     * nimble -pixel at even y coordinate- or the lowest significant nimble -pixel at odd y coordinate-) ;
     * element 2 for byte at even position starting from the buffer (to copy, pixel at odd y coordinate).
     */
    uint8_t doublePix[3]{0, 0, 0};
    /**
     * @brief Pre-computed flag signaling that the width is odd, meaning there is a half-byte required to store a full
     * line.
     */
    bool widthIsOdd;

    void useColor(uint16_t red, uint16_t green, uint16_t blue);
    /**
     * @brief Compute the number of subpixels starting from the buffer, to the designated pixels.
     *
     * ## Automated test
     *
     * > WHEN I KNOW HOW TO INTEGRATE A TESTING PHASE IN THE BUILD :
     *
     * __When__ the frame buffer is for a width of 20 pixels (60 subpixels) and a height of 15 pixels
     *
     * __Then__ `bufferOffsetFromCoordinates(0, 0) == 0`
     *
     * __Then__ `bufferOffsetFromCoordinates(9, 9) == 567` (3 × 9 + 60 × 9)
     *
     * __Then__ `bufferOffsetFromCoordinates(9, 10) == 627` (3 × 9 + 60 × 10)
     *
     * __Then__ `bufferOffsetFromCoordinates(10, 9) == 570` (3 × 10 + 60 × 9)
     *
     * __Then__ `bufferOffsetFromCoordinates(10, 10) == 630` (3 × 10 + 60 × 10)
     *
     * __When__ the frame buffer is for a width of 21 pixels (63 subpixels) and a height of 15 pixels
     *
     * __Then__ `bufferOffsetFromCoordinates(0, 0) == 0`
     *
     * __Then__ `bufferOffsetFromCoordinates(9, 9) == 594` (3 × 9 + 63 × 9)
     *
     * __Then__ `bufferOffsetFromCoordinates(9, 10) == 657` (3 × 9 + 63 × 10)
     *
     * __Then__ `bufferOffsetFromCoordinates(10, 9) == 597` (3 × 10 + 63 × 9)
     *
     * __Then__ `bufferOffsetFromCoordinates(10, 10) == 660` (3 × 10 + 63 × 10)
     *
     * @param x
     * @param y
     * @return uint32_t
     */
    uint32_t subPixelOffsetFromCoordinates(uint32_t x, uint32_t y) { return x * 3 + y * widthInSubPixels; };
    uint32_t appendSinglePixel(uint32_t firstSubpixel);
    uint32_t appendHlineOfPixels(uint32_t firstSubpixel, uint32_t lastSubpixel);

    public:
    FrameBuffer12BppUnsafe(uint32_t w, uint32_t h, uint8_t *start) : width{w}, height{h}, buffer{start} {
        widthIsOdd = (width & 1) == 1;
        widthInSubPixels = 3 * width;
    }
    virtual ~FrameBuffer12BppUnsafe();
    /**
     * @brief Get the width of the area maintained by this frame buffer, x coordinates range from 0 to `width - 1`.
     *
     * @return uint32_t the width.
     */
    virtual uint32_t getWidth() { return width; };
    /**
     * @brief Get the height of the area maintained by this frame buffer, y coordinates range from 0 to `height - 1`.
     *
     * @return uint32_t the height.
     */
    virtual uint32_t getHeight() { return height; };
    /**
     * @brief Set the Active Color to be used by the frame buffer.
     *
     * @param color the color definition
     */
    virtual void setActiveColor(ColorRgb48Bpp *color) {
        if (nullptr != color) {
            useColor(color->red, color->green, color->blue);
        } else {
            useColor(0, 0, 0);
        }
    };
    /**
     * @brief Get the Active Color used by the frame buffer.
     *
     * @return ColorRgb48Bpp*
     */
    virtual ColorRgb48Bpp *getActiveColor() { return &activeColor; };
    /**
     * @brief Draws a single point, that should be inside the framebuffer.
     *
     * @param x1 x coordinate of the point.
     * @param y1 y coordinate of the point.
     */
    virtual void point(uint32_t x1, uint32_t y1);
    /**
     * @brief Draws an horizontal line at y1, from x1 to x2 included. It is usually better when `x1 < x2` and all
     * coordinates falls inside the framebuffer.
     *
     * @param x1 x coordinate of the start.
     * @param y1 y coordinate of the line.
     * @param x2 x coordinate of the end.
     */
    virtual void hline(uint32_t x1, uint32_t y1, uint32_t x2);
    /**
     * @brief Draws a vertical line at x1, from y1 to y2 included. It is usually better when `y1 < y2` and all
     * coordinates falls inside the framebuffer.
     *
     * @param x1 x coordinate of the line.
     * @param y1 y coordinate of the start.
     * @param y2 y coordinate of the end.
     */
    virtual void vline(uint32_t x1, uint32_t y1, uint32_t y2);
    /**
     * @brief Fills an area that spans from x1 to x2 included along the x axis, and from y1 to y2 along the y axis. It
     * is usually better when `x1 < x2`, `y1 < y2` and all coordinates falls inside the framebuffer.
     *
     * @param x1 first x coordinate of the border along the x axis.
     * @param y1 first y coordinate of the border along the y axis.
     * @param x2 second x coordinate of the border along the x axis.
     * @param y2 second y coordinate of the border along the y axis.
     */
    virtual void area(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
    /**
     * @brief Extracts the data from a rectangular area of the framebuffer to a memory buffer. The rectangular area
     * spans from x1 to x2 included along the x axis, and from y1 to y2 along the y axis. It is usually better when `x1
     * < x2`, `y1 < y2`. All coordinates MUST fall inside the framebuffer or the operation will NOT be performed.
     *
     * @param x1 first x coordinate of the border along the x axis.
     * @param y1 first y coordinate of the border along the y axis.
     * @param x2 second x coordinate of the border along the x axis.
     * @param y2 second y coordinate of the border along the y axis.
     * @param bufferDestination the memory area to where the data will be copied.
     *
     * @return bool `true` when the operation was succesfully performed.
     */
    virtual bool copy(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferDestination);
    /**
     * @brief Copy the data from a memory buffer into a rectangular area that spans from x1 to x2 included along the x
     * axis, and from y1 to y2 along the y axis. It is usually better when `x1 < x2`, `y1 < y2`. All coordinates MUST
     * fall inside the framebuffer or the operation will NOT be performed.
     *
     * @param x1 first x coordinate of the border along the x axis.
     * @param y1 first y coordinate of the border along the y axis.
     * @param x2 second x coordinate of the border along the x axis.
     * @param y2 second y coordinate of the border along the y axis.
     * @param bufferSource the memory area from where the data will be copied.
     */
    virtual bool paste(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferSource);
};

#endif