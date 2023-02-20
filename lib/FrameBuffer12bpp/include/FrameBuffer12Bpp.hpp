#ifndef FRAME_BUFFER12BPP_HPP
#define FRAME_BUFFER12BPP_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes
#include "ColorRgb48Bpp.hpp"
#include "FrameBuffer.hpp"
#include "FrameBuffer12BppUnsafe.hpp"

/** @brief Public implementation of FrameBuffer with a pixel model having 12 bits per pixels (rgb-444). Due to the
 * memory organisation, it is better when copy/paste operation are done on x coordinate starting with the smallest
 * coordinate on an even value, and the biggest coordinate on an odd value.
 *
 * This public implementation performs various checks with
 */
class FrameBuffer12Bpp : public FrameBuffer {
    private:
    FrameBuffer12BppUnsafe *internal;
    uint32_t xMax;
    uint32_t yMax;
    bool isValidX(uint32_t x) { return x < getWidth(); };
    bool isValidY(uint32_t y) { return y < getHeight(); };

    public:
    virtual ~FrameBuffer12Bpp();
    FrameBuffer12Bpp(uint32_t w, uint32_t h, uint8_t *start) : xMax{w - 1}, yMax{h - 1} {
        internal = new FrameBuffer12BppUnsafe(w, h, start);
    };
    /**
     * @brief Get the width of the area maintained by this frame buffer, x coordinates range from 0 to `width - 1`.
     *
     * @return uint32_t the width.
     */
    virtual uint32_t getWidth() { return internal->getWidth(); };
    /**
     * @brief Get the height of the area maintained by this frame buffer, y coordinates range from 0 to `height - 1`.
     *
     * @return uint32_t the height.
     */
    virtual uint32_t getHeight() { return internal->getWidth(); };
    /**
     * @brief Set the Active Color to be used by the frame buffer.
     *
     * @param color the color definition
     */
    virtual void setActiveColor(ColorRgb48Bpp *color) { internal->setActiveColor(color); };
    /**
     * @brief Get the Active Color used by the frame buffer.
     *
     * @return ColorRgb48Bpp*
     */
    virtual ColorRgb48Bpp *getActiveColor() { return internal->getActiveColor(); };
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