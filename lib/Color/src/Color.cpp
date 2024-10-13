
// header include
#include "Color.hpp"
#include <variant>

// ==========[ BaseColors ]==========

const Color BaseColors::BLACK(PixelFormat::RGB888_24BE, 0x000000);
const Color BaseColors::DARKER_GREY(PixelFormat::RGB888_24BE, 0x555555);
const Color BaseColors::GREY(PixelFormat::RGB888_24BE, 0xaaaaaa);
const Color BaseColors::WHITE(PixelFormat::RGB888_24BE, 0xffffff);

const Color BaseColors::SOOT_BLUE(PixelFormat::RGB888_24BE, 0x000055);
const Color BaseColors::SOOT_GREEN(PixelFormat::RGB888_24BE, 0x005500);
const Color BaseColors::SOOT_CYAN(PixelFormat::RGB888_24BE, 0x005555);
const Color BaseColors::SOOT_RED(PixelFormat::RGB888_24BE, 0x550000);
const Color BaseColors::SOOT_MAGENTA(PixelFormat::RGB888_24BE, 0x550055);
const Color BaseColors::SOOT_YELLOW(PixelFormat::RGB888_24BE, 0x555500);

const Color BaseColors::DARKER_BLUE(PixelFormat::RGB888_24BE, 0x0000aa);
const Color BaseColors::DARKER_GREEN(PixelFormat::RGB888_24BE, 0x00aa00);
const Color BaseColors::DARKER_CYAN(PixelFormat::RGB888_24BE, 0x00aaaa);
const Color BaseColors::DARKER_RED(PixelFormat::RGB888_24BE, 0xaa0000);
const Color BaseColors::DARKER_MAGENTA(PixelFormat::RGB888_24BE, 0xaa00aa);
const Color BaseColors::DARKER_YELLOW(PixelFormat::RGB888_24BE, 0xaaaa00);

const Color BaseColors::BLUE(PixelFormat::RGB888_24BE, 0x0000ff);
const Color BaseColors::GREEN(PixelFormat::RGB888_24BE, 0x00ff00);
const Color BaseColors::CYAN(PixelFormat::RGB888_24BE, 0x00ffff);
const Color BaseColors::RED(PixelFormat::RGB888_24BE, 0xff0000);
const Color BaseColors::MAGENTA(PixelFormat::RGB888_24BE, 0xff00ff);
const Color BaseColors::YELLOW(PixelFormat::RGB888_24BE, 0xffff00);

const Color BaseColors::LIGHTER_BLUE(PixelFormat::RGB888_24BE, 0x5555ff);
const Color BaseColors::LIGHTER_GREEN(PixelFormat::RGB888_24BE, 0x55ff55);
const Color BaseColors::LIGHTER_CYAN(PixelFormat::RGB888_24BE, 0x55ffff);
const Color BaseColors::LIGHTER_RED(PixelFormat::RGB888_24BE, 0xff5555);
const Color BaseColors::LIGHTER_MAGENTA(PixelFormat::RGB888_24BE, 0xff55ff);
const Color BaseColors::LIGHTER_YELLOW(PixelFormat::RGB888_24BE, 0xffff55);

const Color BaseColors::PASTEL_BLUE(PixelFormat::RGB888_24BE, 0xaaaaff);
const Color BaseColors::PASTEL_GREEN(PixelFormat::RGB888_24BE, 0xaaffaa);
const Color BaseColors::PASTEL_CYAN(PixelFormat::RGB888_24BE, 0xaaffff);
const Color BaseColors::PASTEL_RED(PixelFormat::RGB888_24BE, 0xffaaaa);
const Color BaseColors::PASTEL_MAGENTA(PixelFormat::RGB888_24BE, 0xffaaff);
const Color BaseColors::PASTEL_YELLOW(PixelFormat::RGB888_24BE, 0xffffaa);

const Color BaseColors::DARKER_ORANGE(PixelFormat::RGB888_24BE, 0xaa5500);
const Color BaseColors::ORANGE(PixelFormat::RGB888_24BE, 0xffaa00);

// ==========[ MonochromeGreenColors ]==========

const Color MonochromeGreenColors::GREEN1(PixelFormat::RGB888_24BE, 0x399b6a);
const Color MonochromeGreenColors::GREEN2(PixelFormat::RGB888_24BE, 0x42d297);
const Color MonochromeGreenColors::GREEN3(PixelFormat::RGB888_24BE, 0x73fdbf);
const Color MonochromeGreenColors::GREEN4(PixelFormat::RGB888_24BE, 0xbaffe0);

// ==========[ MonochromeAmberColors ]==========

const Color MonochromeAmberColors::AMBER1(PixelFormat::RGB888_24BE, 0xa17414);
const Color MonochromeAmberColors::AMBER2(PixelFormat::RGB888_24BE, 0xd3a51a);
const Color MonochromeAmberColors::AMBER3(PixelFormat::RGB888_24BE, 0xe5bf13);
const Color MonochromeAmberColors::AMBER4(PixelFormat::RGB888_24BE, 0xedde8c);

// ==========[ Color ]==========

Color::Color(const PixelFormat &format, uint32_t encoded) {
    if (format.getType() != PixelType::RGB) {
        const IndexedFormat &f = std::get<IndexedFormat>(format.getFormat());
        // const IndexedFormat &formatf = std::get<IndexedFormat>(format.format);
        if (
            (f.getIndexWidth() == 1) && //
            (StorageUnitFormat::DOUBLE_BYTES_BE == format.getStorage()) && //
            (PixelLayout::PROGRESSIVE == format.getLayout())//
            ) {
            red = 0;
        }
    } else {
        // type == PixelType::RGB
        const RGBFormat &f = std::get<RGBFormat>(format.getFormat());
        // const RGBFormat &formatf = std::get<RGBFormat>(format.getFormat());
        if (
            (f.getRedWidth() == 8) && 
            (f.getGreenWidth() == 8) && 
            (f.getBlueWidth() == 8) &&
            (StorageUnitFormat::DOUBLE_BYTES_BE == format.getStorage()) &&
            (PixelLayout::PROGRESSIVE == format.getLayout())
            ) {
            red = encoded;
        }
    }
    /** /
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
    /**/
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