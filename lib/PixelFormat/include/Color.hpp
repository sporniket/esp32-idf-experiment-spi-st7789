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
    Color(const PixelFormat &format, uint32_t encoded) {
        if (format == PixelFormat::RGB332_8BE) {
            red = ((((encoded & 0b11100000) >> 5) * 0b1001001) >> 1) * 0x01010101;
            green = ((((encoded & 0b00011100) >> 2) * 0b1001001) >> 1) * 0x01010101;
            blue = ((encoded & 0b00000011) * 0b01010101) * 0x01010101;
        } else if (format == PixelFormat::RGB444C_12BE) {
            red = (((encoded & 0b111100000000) >> 8) * 0x11) * 0x01010101;
            green = (((encoded & 0b000011110000) >> 4) * 0x11) * 0x01010101;
            blue = ((encoded & 0b000000001111) * 0x11) * 0x01010101;
        } else if (format == PixelFormat::RGB555_16BE) {
            red = ((((encoded & 0b111110000000000) >> 10) * 0b100001) >> 2) * 0x01010101;
            green = ((((encoded & 0b000001111100000) >> 5) * 0b100001) >> 2) * 0x01010101;
            blue = (((encoded & 0b000000000011111) * 0b100001) >> 2) * 0x01010101;
        } else if (format == PixelFormat::RGB565_16BE) {
            red = ((((encoded & 0b1111100000000000) >> 11) * 0b100001) >> 2) * 0x01010101;
            green = ((((encoded & 0b0000011111100000) >> 5) * 0b1000001) >> 4) * 0x01010101;
            blue = (((encoded & 0b0000000000011111) * 0b100001) >> 2) * 0x01010101;
        } else if (format == PixelFormat::RGB666_24BE) {
            red = ((((encoded & 0b111111000000000000) >> 12) * 0b1000001) >> 4) * 0x01010101;
            green = ((((encoded & 0b000000111111000000) >> 6) * 0b1000001) >> 4) * 0x01010101;
            blue = (((encoded & 0b000000000000111111) * 0b1000001) >> 4) * 0x01010101;
        } else if (format == PixelFormat::RGB888_24BE) {
            red = ((encoded & 0xFF0000) >> 16) * 0x01010101;
            green = ((encoded & 0x00FF00) >> 8) * 0x01010101;
            blue = (encoded & 0x0000FF) * 0x01010101;
        }
        rgb332 = (((red >> 24) & 0b11100000) | ((green >> 27) & 0b00011100) | ((blue >> 30) & 0b00000011));
        rgb444 = (((red >> 20) & 0b111100000000) | ((green >> 24) & 0b000011110000) | ((blue >> 28) & 0b000000001111));
        rgb555 = (((red >> 17) & 0b111110000000000) | ((green >> 22) & 0b000001111100000) |
                  ((blue >> 27) & 0b000000000011111));
        rgb565 = (((red >> 16) & 0b1111100000000000) | ((green >> 21) & 0b0000011111100000) |
                  ((blue >> 27) & 0b0000000000011111));
        rgb666 = (((red >> 14) & 0b111111000000000000) | ((green >> 20) & 0b000000111111000000) |
                  ((blue >> 26) & 0b000000000000111111));
        rgb888 = (((red >> 8) & 0xFF0000) | ((green >> 16) & 0x00FF00) | ((blue >> 24) & 0x0000FF));
    }

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
class BaseColors {
    public:
    // Black to white
    static const Color BLACK;
    static const Color DARKER_GREY;
    static const Color GREY;
    static const Color WHITE;

    // "Soot" colors, 0x55 per active channel else 0
    static const Color SOOT_BLUE;
    static const Color SOOT_GREEN;
    static const Color SOOT_CYAN;
    static const Color SOOT_RED;
    static const Color SOOT_MAGENTA;
    static const Color SOOT_YELLOW;

    // "Darker" colors, 0xaa per active channel else 0
    static const Color DARKER_BLUE;
    static const Color DARKER_GREEN;
    static const Color DARKER_CYAN;
    static const Color DARKER_RED;
    static const Color DARKER_MAGENTA;
    static const Color DARKER_YELLOW;

    // Normal colors, 0xff per active channel else 0
    static const Color BLUE;
    static const Color GREEN;
    static const Color CYAN;
    static const Color RED;
    static const Color MAGENTA;
    static const Color YELLOW;

    // "Lighter" colors, 0xff per active channel else 0x55
    static const Color LIGHTER_BLUE;
    static const Color LIGHTER_GREEN;
    static const Color LIGHTER_CYAN;
    static const Color LIGHTER_RED;
    static const Color LIGHTER_MAGENTA;
    static const Color LIGHTER_YELLOW;

    // "Pastel" colors, 0xff per active channel else 0xaa
    static const Color PASTEL_BLUE;
    static const Color PASTEL_GREEN;
    static const Color PASTEL_CYAN;
    static const Color PASTEL_RED;
    static const Color PASTEL_MAGENTA;
    static const Color PASTEL_YELLOW;

    // Orange colors : R > G > B
    static const Color DARKER_ORANGE;
    static const Color ORANGE;
};

/**
 * @brief Set of colors to simulate a vintage CRT screen with green phosphore (P1).
 */
class MonochromeGreenColors {
    public:
    static const Color GREEN1;
    static const Color GREEN2;
    static const Color GREEN3;
    static const Color GREEN4;
};

/**
 * @brief Set of colors to simulate a vintage CRT screen with ambre phosphore (P3).
 */
class MonochromeAmberColors {
    public:
    static const Color AMBER1;
    static const Color AMBER2;
    static const Color AMBER3;
    static const Color AMBER4;
};

#endif