#ifndef PALETTE_HPP
#define PALETTE_HPP

// standard includes
#include <cstdint>
#include <vector>

// esp32 includes

// project includes
#include "Color.hpp"

using Palette = std::vector<Color>;

enum class PaletteEnum {
    /**
     * @brief As the name state it, the basic 16 colors of DOS.
     */
    CLASSIC_DOS,

    // Monochrome palettes : a 5 colors gradient from black to the lightest color.
    /**
     * @brief Green monochrome palette.
     */
    MONOCHROME_GREEN,

    /**
     * @brief Amber monochrome palette.
     */
    MONOCHROME_AMBER,

    /**
     * @brief White monochrome palette.
     */
    MONOCHROME_WHITE,

    // Duochrome palettes : only two colors (one background color and one drawing color).
    /**
     * @brief Green on black palette.
     */
    DUOCHROME_GREEN_ON_BLACK,

    /**
     * @brief Amber on black palette.
     */
    DUOCHROME_AMBER_ON_BLACK,

    /**
     * @brief White on black palette.
     */
    DUOCHROME_WHITE_ON_BLACK,

    /**
     * @brief Black on white palette.
     */
    DUOCHROME_BLACK_ON_WHITE
};

class PaletteHelper {
    public:
    static Palette *getPalette(const PaletteEnum id);
};

#endif