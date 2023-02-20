
// header include
#include "FrameBuffer12Bpp.hpp"

FrameBuffer12Bpp::~FrameBuffer12Bpp() {
    delete internal;
    internal = nullptr;
}
// write code here...

void FrameBuffer12Bpp::point(uint32_t x1, uint32_t y1) {
    if (isValidX(x1) && isValidY(y1)) {
        internal->point(x1, y1);
    }
}

void FrameBuffer12Bpp::hline(uint32_t x1, uint32_t y1, uint32_t x2) {
    if (!isValidY(y1)) {
        // do nothing
        return;
    }
    if (x2 == x1 && isValidX(x1)) {
        return internal->point(x1, y1);
    }
    if (x2 < x1) {
        if (isValidX(x2)) {
            return internal->hline(x2, y1, isValidX(x1) ? x1 : xMax);
        }
    } else {
        if (isValidX(x1)) {
            return internal->hline(x1, y1, isValidX(x2) ? x2 : xMax);
        }
    }
}

void FrameBuffer12Bpp::vline(uint32_t x1, uint32_t y1, uint32_t y2) {
    if (!isValidX(x1)) {
        // do nothing
        return;
    }
    if (y2 == y1 && isValidY(y1)) {
        return internal->point(x1, y1);
    }
    if (y2 < y1) {
        if (isValidY(y2)) {
            return internal->vline(x1, y2, isValidY(y1) ? y1 : yMax);
        }
    } else {
        if (isValidY(y1)) {
            return internal->vline(x1, y1, isValidY(y1) ? y1 : yMax);
        }
    }
}

void FrameBuffer12Bpp::area(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) {
    if (x2 == x1) {
        return vline(x1, y1, y2);
    }
    if (y2 == y1) {
        return hline(x1, y1, x2);
    }
    if (y2 < y1) {
        if (isValidY(y2)) {
            uint32_t yBottom = isValidY(y1) ? y1 : yMax;
            if (x2 < x1) {
                if (isValidX(x2)) {
                    return internal->area(x2, y2, isValidX(x1) ? x1 : xMax, yBottom);
                }
            } else {
                if (isValidX(x1)) {
                    return internal->area(x1, y2, isValidX(x2) ? x2 : xMax, yBottom);
                }
            }
        }
    } else {
        if (isValidY(y1)) {
            uint32_t yBottom = isValidY(y2) ? y2 : yMax;
            if (x2 < x1) {
                if (isValidX(x2)) {
                    return internal->area(x2, y1, isValidX(x1) ? x1 : xMax, yBottom);
                }
            } else {
                if (isValidX(x1)) {
                    return internal->area(x1, y1, isValidX(x2) ? x2 : xMax, yBottom);
                }
            }
        }
    }
}
bool FrameBuffer12Bpp::copy(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferDestination) {
    //TODO
    return false;
}
bool FrameBuffer12Bpp::paste(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t *bufferSource) {
    //TODO
    return false;
}
