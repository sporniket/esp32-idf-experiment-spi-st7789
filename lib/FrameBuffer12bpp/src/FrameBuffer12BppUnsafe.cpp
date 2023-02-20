
// header include
#include "FrameBuffer12BppUnsafe.hpp"

FrameBuffer12BppUnsafe::~FrameBuffer12BppUnsafe() {}
// write code here...

void FrameBuffer12BppUnsafe::useColor(uint16_t red, uint16_t green, uint16_t blue) {
    // use the 4 most significants bits
    uint16_t red4bits = (red >> 12) & 0xff;
    uint16_t green4bits = (green >> 12) & 0xff;
    uint16_t blue4bits = (blue >> 12) & 0xff;
    // update active color scaled values
    activeColor.red = red4bits << 12;
    activeColor.green = red4bits << 12;
    activeColor.blue = red4bits << 12;
    // update doublePix
    doublePix[0] = red4bits << 4 | green4bits;
    doublePix[1] = blue4bits << 4 | red4bits;
    doublePix[2] = green4bits << 4 | blue4bits;
}

uint32_t FrameBuffer12BppUnsafe::appendSinglePixel(uint32_t firstSubpixel) {
    uint32_t pixData = firstSubpixel >> 1;
    if (firstSubpixel & 0x1) {
        // odd pixel
        buffer[pixData] = (buffer[pixData] & 0xf0) | (doublePix[1] & 0xf);
        ++pixData;
        buffer[pixData++] = doublePix[2];
    } else {
        // even pixel
        buffer[pixData++] = doublePix[0];
        buffer[pixData] = (doublePix[1] & 0x0f) | (buffer[pixData] & 0xf);
    }
    return firstSubpixel + 3;
}

uint32_t FrameBuffer12BppUnsafe::appendHlineOfPixels(uint32_t firstSubpixel, uint32_t lastSubpixel) {
    uint32_t currentSubPixel = firstSubpixel;

    if (firstSubpixel & 0x1) {
        // the first pixel is single
        currentSubPixel = appendSinglePixel(currentSubPixel);
    }
    // now we are at even position
    while (currentSubPixel < lastSubpixel) {
        // append a double pixels as long as we did not reach the last pixel
        uint32_t pixData = currentSubPixel >> 1;
        buffer[pixData++] = doublePix[0];
        buffer[pixData++] = doublePix[1];
        buffer[pixData++] = doublePix[2];
        currentSubPixel += 6;
    }
    if (!(lastSubpixel & 0x1)) {
        // The last pixel is not part of a double pixel.
        currentSubPixel = appendSinglePixel(currentSubPixel);
    }
    return currentSubPixel;
}

void FrameBuffer12BppUnsafe::point(uint32_t x1, uint32_t y1) {
    appendSinglePixel(subPixelOffsetFromCoordinates(x1, y1));
}

void FrameBuffer12BppUnsafe::hline(uint32_t x1, uint32_t y1, uint32_t x2) {
    uint32_t firstSubPixelOffset = subPixelOffsetFromCoordinates(x1, y1);
    uint32_t lastSubPixelOffset = subPixelOffsetFromCoordinates(x2, y1);

    appendHlineOfPixels(firstSubPixelOffset, lastSubPixelOffset);
}

void FrameBuffer12BppUnsafe::vline(uint32_t x1, uint32_t y1, uint32_t y2) {

    uint32_t lastSubPixelOffset = subPixelOffsetFromCoordinates(x1, y2);

    for (uint32_t currentSubPixelOffset = subPixelOffsetFromCoordinates(x1, y1);
         currentSubPixelOffset <= lastSubPixelOffset;
         currentSubPixelOffset = appendSinglePixel(currentSubPixelOffset)) {
    }
}

void FrameBuffer12BppUnsafe::area(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) {
    uint32_t subPixelOffset = subPixelOffsetFromCoordinates(x1, y1);

    uint32_t lastSubPixelOfLineOffset = subPixelOffsetFromCoordinates(x2, y1);

    uint32_t lastSubPixelOffset = subPixelOffsetFromCoordinates(x2, y2);

    while (subPixelOffset < lastSubPixelOffset) {
        appendHlineOfPixels(subPixelOffset, lastSubPixelOfLineOffset);

        subPixelOffset += widthInSubPixels;
        lastSubPixelOfLineOffset += widthInSubPixels;
    }
}
bool FrameBuffer12BppUnsafe::copy(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferDestination) {
    return false; // TODO
}
bool FrameBuffer12BppUnsafe::paste(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferSource) {
    return false; // TODO
}
