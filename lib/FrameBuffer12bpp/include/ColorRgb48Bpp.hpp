#ifndef COLOR_RGB48BPP_HPP
#define COLOR_RGB48BPP_HPP

// standard includes
#include <cstdint>

// esp32 includes

// project includes

/** @brief Model of RGB, 16 bits per channel, color.
 */
class ColorRgb48Bpp {
    private:
    public:
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    ColorRgb48Bpp(uint16_t red, uint16_t green, uint16_t blue) : red{red}, green{green}, blue{blue} {};
    virtual ~ColorRgb48Bpp();
};

#endif