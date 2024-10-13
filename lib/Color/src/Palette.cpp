
// header include
#include "Palette.hpp"

// write code here...

Palette *PaletteHelper::getPalette(const PaletteEnum id) {
    switch (id) {
    case PaletteEnum::CLASSIC_DOS:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::DARKER_BLUE),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::DARKER_GREEN),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::DARKER_CYAN),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::DARKER_RED),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::DARKER_MAGENTA),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::DARKER_ORANGE),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::GREY),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::DARKER_GREY),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLUE),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::GREEN),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::CYAN),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::RED),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::MAGENTA),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::YELLOW),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::WHITE)});
    case PaletteEnum::MONOCHROME_GREEN:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN1),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN2),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN3),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN4)});
    case PaletteEnum::MONOCHROME_AMBER:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER1),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER2),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER3),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER4)});
    case PaletteEnum::MONOCHROME_WHITE:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::GREY_44),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::GREY_88),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::GREY_BB),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::WHITE)});
    case PaletteEnum::DUOCHROME_GREEN_ON_BLACK:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN3)});
    case PaletteEnum::DUOCHROME_AMBER_ON_BLACK:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER3)});
    case PaletteEnum::DUOCHROME_WHITE_ON_BLACK:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::WHITE)});
    case PaletteEnum::DUOCHROME_BLACK_ON_WHITE:
        return new Palette({Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::WHITE),
                            Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK)});
    default:
        return new Palette({});
    }
}
