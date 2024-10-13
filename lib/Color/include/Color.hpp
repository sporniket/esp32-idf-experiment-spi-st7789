#ifndef COLOR_HPP
#define COLOR_HPP

// standard includes
#include <cstdint>
#include <iostream>

// esp32 includes

// project includes
#include "PixelFormat.hpp"

// write code here

class Color {
    public:
    // Constructeur à partir de valeurs red, green, blue
    Color(uint32_t red, uint32_t green, uint32_t blue)
        : red(red), green(green), blue(blue),
          rgb332(((red >> 24) & 0b11100000) | ((green >> 27) & 0b00011100) | ((blue >> 30) & 0b00000011)),
          rgb444(((red >> 20) & 0b111100000000) | ((green >> 24) & 0b000011110000) | ((blue >> 28) & 0b000000001111)),
          rgb555(((red >> 17) & 0b111110000000000) | ((green >> 22) & 0b000001111100000) |
                 ((blue >> 27) & 0b000000000011111)),
          rgb565(((red >> 16) & 0b1111100000000000) | ((green >> 21) & 0b0000011111100000) |
                 ((blue >> 27) & 0b0000000000011111)),
          rgb666(((red >> 14) & 0b111111000000000000) | ((green >> 20) & 0b000000111111000000) |
                 ((blue >> 26) & 0b000000000000111111)),
          rgb888(((red >> 8) & 0xFF0000) | ((green >> 16) & 0x00FF00) | ((blue >> 24) & 0x0000FF)) {}

    // Constructeur à partir d'un format encodé
    Color(const PixelFormatEnum format, uint32_t encoded);

    bool operator==(const Color &rhs) const { return red == rhs.red && green == rhs.green && blue == rhs.blue; }
    bool operator!=(const Color &rhs) const { return !operator==(rhs); }

    // Accesseurs pour les composantes rouge, verte et bleue
    const uint32_t getRed() const { return red; }
    const uint32_t getGreen() const { return green; }
    const uint32_t getBlue() const { return blue; }

    // Conversions vers différents formats de couleur
    const uint8_t asRgb332() const { return rgb332; }

    const uint16_t asRgb444() const { return rgb444; }

    const uint16_t asRgb555() const { return rgb555; }

    const uint16_t asRgb565() const { return rgb565; }

    const uint32_t asRgb666() const { return rgb666; }

    const uint32_t asRgb888() const { return rgb888; }

    private:
    uint32_t red;
    uint32_t green;
    uint32_t blue;

    uint8_t rgb332;
    uint16_t rgb444;
    uint16_t rgb555;
    uint16_t rgb565;
    uint32_t rgb666;
    uint32_t rgb888;
};

/**
 * @brief Set of the primary colors and orange, and their darker or lighter derivatives.
 */
enum class BaseColors {

    // Black to white
    BLACK = 0x000000,
    GREY_44 = 0x444444,
    DARKER_GREY = 0x555555,
    GREY_88 = 0x888888,
    GREY = 0xaaaaaa,
    GREY_BB = 0xbbbbbb,
    WHITE = 0xffffff,

    // "Soot" colors, 0x55 per active channel else 0
    SOOT_BLUE = 0x000055,
    SOOT_GREEN = 0x005500,
    SOOT_CYAN = 0x005555,
    SOOT_RED = 0x550000,
    SOOT_MAGENTA = 0x550055,
    SOOT_YELLOW = 0x555500,

    // "Darker" colors, 0xaa per active channel else 0
    DARKER_BLUE = 0x0000aa,
    DARKER_GREEN = 0x00aa00,
    DARKER_CYAN = 0x00aaaa,
    DARKER_RED = 0xaa0000,
    DARKER_MAGENTA = 0xaa00aa,
    DARKER_YELLOW = 0xaaaa00,

    // Normal colors, 0xff per active channel else 0
    BLUE = 0x0000ff,
    GREEN = 0x00ff00,
    CYAN = 0x00ffff,
    RED = 0xff0000,
    MAGENTA = 0xff00ff,
    YELLOW = 0xffff00,

    // "Lighter" colors, 0xff per active channel else 0x55
    LIGHTER_BLUE = 0x5555ff,
    LIGHTER_GREEN = 0x55ff55,
    LIGHTER_CYAN = 0x55ffff,
    LIGHTER_RED = 0xff5555,
    LIGHTER_MAGENTA = 0xff55ff,
    LIGHTER_YELLOW = 0xffff55,

    // "Pastel" colors, 0xff per active channel else 0xaa
    PASTEL_BLUE = 0xaaaaff,
    PASTEL_GREEN = 0xaaffaa,
    PASTEL_CYAN = 0xaaffff,
    PASTEL_RED = 0xffaaaa,
    PASTEL_MAGENTA = 0xffaaff,
    PASTEL_YELLOW = 0xffffaa,

    // Orange colors : R > G > B
    DARKER_ORANGE = 0xaa5500,
    ORANGE = 0xffaa00,
};

/**
 * @brief Set of colors to simulate a vintage CRT screen with green phosphore (P1).
 */
enum class MonochromeGreenColors {
    GREEN1 = 0x399b6a,
    GREEN2 = 0x42d297,
    GREEN3 = 0x73fdbf,
    GREEN4 = 0xbaffe0,
};

/**
 * @brief Set of colors to simulate a vintage CRT screen with ambre phosphore (P3).
 */
enum class MonochromeAmberColors {
    AMBER1 = 0xa17414,
    AMBER2 = 0xd3a51a,
    AMBER3 = 0xe5bf13,
    AMBER4 = 0xedde8c,
};

#endif