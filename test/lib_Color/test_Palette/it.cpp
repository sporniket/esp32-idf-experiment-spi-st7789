#include "Palette.hpp"
#include <memory>
#include <unity.h>

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void Palette__isAVectorOfColors() {
    const Color a(PixelFormatEnum::RGB888_24BE, 0x000000);
    const Color b(PixelFormatEnum::RGB888_24BE, 0xcccccc);
    const Color c(PixelFormatEnum::RGB888_24BE, 0xffffff);
    const Palette p{a, b, c};

    TEST_ASSERT_TRUE((p.size() == 3));
    TEST_ASSERT_TRUE((p[0] == Color(PixelFormatEnum::RGB888_24BE, 0x000000)));
    TEST_ASSERT_TRUE((p[1] == Color(PixelFormatEnum::RGB888_24BE, 0xcccccc)));
    TEST_ASSERT_TRUE((p[2] == Color(PixelFormatEnum::RGB888_24BE, 0xffffff)));
}
void PaletteHelper_getPalette__MONOCHROME_GREEN() {
    std::unique_ptr<Palette> p{PaletteHelper::getPalette(PaletteEnum::MONOCHROME_GREEN)};
    TEST_ASSERT_TRUE((p->size() == 5));
    TEST_ASSERT_TRUE((p->at(0).asRgb888() == (uint32_t)BaseColors::BLACK));
    TEST_ASSERT_TRUE((p->at(1).asRgb888() == (uint32_t)MonochromeGreenColors::GREEN1));
    TEST_ASSERT_TRUE((p->at(2).asRgb888() == (uint32_t)MonochromeGreenColors::GREEN2));
    TEST_ASSERT_TRUE((p->at(3).asRgb888() == (uint32_t)MonochromeGreenColors::GREEN3));
    TEST_ASSERT_TRUE((p->at(4).asRgb888() == (uint32_t)MonochromeGreenColors::GREEN4));
}

void PaletteHelper_getPalette__MONOCHROME_AMBER() {
    std::unique_ptr<Palette> p{PaletteHelper::getPalette(PaletteEnum::MONOCHROME_AMBER)};
    TEST_ASSERT_TRUE((p->size() == 5));
    TEST_ASSERT_TRUE((p->at(0).asRgb888() == (uint32_t)BaseColors::BLACK));
    TEST_ASSERT_TRUE((p->at(1).asRgb888() == (uint32_t)MonochromeAmberColors::AMBER1));
    TEST_ASSERT_TRUE((p->at(2).asRgb888() == (uint32_t)MonochromeAmberColors::AMBER2));
    TEST_ASSERT_TRUE((p->at(3).asRgb888() == (uint32_t)MonochromeAmberColors::AMBER3));
    TEST_ASSERT_TRUE((p->at(4).asRgb888() == (uint32_t)MonochromeAmberColors::AMBER4));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(Palette__isAVectorOfColors);
    RUN_TEST(PaletteHelper_getPalette__MONOCHROME_GREEN);
    RUN_TEST(PaletteHelper_getPalette__MONOCHROME_AMBER);
    UNITY_END();
}
