#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes
#include "ColorRgb48Bpp.hpp"

/** @brief Low level interface of a memory area containing raw data representing a surface of pixels.
 *
 * A framebuffer maintains an _active color_ in order to draw some _primitives_ ; it also provide
 * a mean to extract a rectangular area (`copy`) to a memory buffer, and the reverse operation (`paste`).
 */
class FrameBuffer {
    private:
    public:
    virtual ~FrameBuffer();
    /**
     * @brief Get the width of the area maintained by this frame buffer, x coordinates range from 0 to `width - 1`.
     *
     * @return uint32_t the width.
     */
    virtual uint32_t getWidth() = 0;
    /**
     * @brief Get the height of the area maintained by this frame buffer, y coordinates range from 0 to `height - 1`.
     *
     * @return uint32_t the height.
     */
    virtual uint32_t getHeight() = 0;
    /**
     * @brief Set the Active Color to be used by the frame buffer.
     *
     * @param color the color definition
     */
    virtual void setActiveColor(ColorRgb48Bpp *color) = 0;
    /**
     * @brief Get the Active Color used by the frame buffer.
     *
     * @return ColorRgb48Bpp*
     */
    virtual ColorRgb48Bpp *getActiveColor() = 0;
    /**
     * @brief Draws a single point, that should be inside the framebuffer.
     *
     * @param x1 x coordinate of the point.
     * @param y1 y coordinate of the point.
     */
    virtual void point(uint32_t x1, uint32_t y1) = 0;
    /**
     * @brief Draws an horizontal line at y1, from x1 to x2 included. It is usually better when `x1 < x2` and all
     * coordinates falls inside the framebuffer.
     *
     * @param x1 x coordinate of the start.
     * @param y1 y coordinate of the line.
     * @param x2 x coordinate of the end.
     */
    virtual void hline(uint32_t x1, uint32_t y1, uint32_t x2) = 0;
    /**
     * @brief Draws a vertical line at x1, from y1 to y2 included. It is usually better when `y1 < y2` and all
     * coordinates falls inside the framebuffer.
     *
     * @param x1 x coordinate of the line.
     * @param y1 y coordinate of the start.
     * @param y2 y coordinate of the end.
     */
    virtual void vline(uint32_t x1, uint32_t y1, uint32_t y2) = 0;
    /**
     * @brief Fills an area that spans from x1 to x2 included along the x axis, and from y1 to y2 along the y axis. It
     * is usually better when `x1 < x2`, `y1 < y2` and all coordinates falls inside the framebuffer.
     *
     * @param x1 first x coordinate of the border along the x axis.
     * @param y1 first y coordinate of the border along the y axis.
     * @param x2 second x coordinate of the border along the x axis.
     * @param y2 second y coordinate of the border along the y axis.
     */
    virtual void area(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) = 0;
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
    virtual bool copy(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferDestination) = 0;
    /**
     * @brief Copy the data from a memory buffer into a rectangular area that spans from x1 to x2 included along the x
     * axis, and from y1 to y2 along the y axis. It is usually better when `x1 < x2`, `y1 < y2`. All coordinates MUST fall inside the framebuffer or the operation will NOT be performed.
     *
     * @param x1 first x coordinate of the border along the x axis.
     * @param y1 first y coordinate of the border along the y axis.
     * @param x2 second x coordinate of the border along the x axis.
     * @param y2 second y coordinate of the border along the y axis.
     * @param bufferSource the memory area from where the data will be copied.
     */
    virtual bool paste(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferSource) = 0;
};

#endif