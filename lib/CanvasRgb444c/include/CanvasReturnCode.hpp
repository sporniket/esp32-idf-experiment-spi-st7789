#ifndef CANVAS_RETURN_CODE_HPP
#define CANVAS_RETURN_CODE_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes

enum class CanvasReturnCode {
    OK = 0,
    // == out of bounds ==
    KO__OUT_OF_BOUNDS,                            // unspecified source
    KO__OUT_OF_BOUNDS__CANVAS_INVALID_DIMENSIONS, // canvas width and height MUST be > 0
    KO__OUT_OF_BOUNDS__X,
    KO__OUT_OF_BOUNDS__Y,
    KO__OUT_OF_BOUNDS__W_ZERO, // w MUST be > 0
    KO__OUT_OF_BOUNDS__W,
    KO__OUT_OF_BOUNDS__X2,
    KO__OUT_OF_BOUNDS__Y2,
    KO__OUT_OF_BOUNDS__H_ZERO, // h MUST be > 0
    KO__OUT_OF_BOUNDS__H

};

#endif