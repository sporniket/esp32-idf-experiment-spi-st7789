
// header include
#include "Palette.hpp"

// write code here...

const Palette MonochromeGreen::PALETTE {
    Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
                                       Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN1),
                                       Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN2),
                                       Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN3),
                                       Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeGreenColors::GREEN4)};

const Palette MonochromeAmber::PALETTE {

        Color(PixelFormatEnum::RGB888_24BE, (uint32_t)BaseColors::BLACK),
        Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER1),
        Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER2),
        Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER3),
        Color(PixelFormatEnum::RGB888_24BE, (uint32_t)MonochromeAmberColors::AMBER4)};
