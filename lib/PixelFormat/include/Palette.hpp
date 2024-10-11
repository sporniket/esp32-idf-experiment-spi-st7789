#ifndef PALETTE_HPP
#define PALETTE_HPP

// standard includes
#include <cstdint>
#include <vector>

// esp32 includes

// project includes
#include "Color.hpp"

using Palette = std::vector<Color>;

/**
 * @brief Palette to simulate a vintage CRT screen with green phosphore (P1).
 */
class MonochromeGreen {
    public:
    static const Palette PALETTE;
};

/**
 * @brief Palette to simulate a vintage CRT screen with ambre phosphore (P3).
 */
class MonochromeAmber {
    public:
    static const Palette PALETTE;
};

#endif