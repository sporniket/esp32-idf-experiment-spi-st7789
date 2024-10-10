#ifndef COLOR_HPP
#define COLOR_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes
#include "PixelFormat.hpp"

//write code here

class Color {
    uint32_t red, green, blue;

    public:
    // Constructeur à partir de valeurs red, green, blue
    Color(uint32_t red, uint32_t green, uint32_t blue) : red(red), green(green), blue(blue) {}

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
    }

    // Accesseurs pour les composantes rouge, verte et bleue
    const uint32_t getRed() const { return red; }
    const uint32_t getGreen() const { return green; }
    const uint32_t getBlue() const { return blue; }

    // Conversions vers différents formats de couleur
    const uint8_t asRgb332() const {
        return ((red >> 24) & 0b11100000) | ((green >> 27) & 0b00011100) | ((blue >> 30) & 0b00000011);
    }

    const uint16_t asRgb444() const {
        return ((red >> 20) & 0b111100000000) | ((green >> 24) & 0b000011110000) | ((blue >> 28) & 0b000000001111);
    }

    const uint16_t asRgb555() const {
        return ((red >> 17) & 0b111110000000000) | ((green >> 22) & 0b000001111100000) |
               ((blue >> 27) & 0b000000000011111);
    }

    const uint16_t asRgb565() const {
        return ((red >> 16) & 0b1111100000000000) | ((green >> 21) & 0b0000011111100000) |
               ((blue >> 27) & 0b0000000000011111);
    }

    const uint32_t asRgb666() const {
        return ((red >> 14) & 0b111111000000000000) | ((green >> 20) & 0b000000111111000000) |
               ((blue >> 26) & 0b000000000000111111);
    }

    const uint32_t asRgb888() const {
        return ((red >> 8) & 0xFF0000) | ((green >> 16) & 0x00FF00) | ((blue >> 24) & 0x0000FF);
    }
};

#endif