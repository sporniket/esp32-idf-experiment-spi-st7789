#include "Palette.hpp"
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
    const Color a(PixelFormat::RGB888_24BE, 0x000000);
    const Color b(PixelFormat::RGB888_24BE, 0xcccccc);
    const Color c(PixelFormat::RGB888_24BE, 0xffffff);
    const Palette p {a,b,c};

    TEST_ASSERT_TRUE((p.size() == 3));
    TEST_ASSERT_TRUE((p[0] == Color(PixelFormat::RGB888_24BE, 0x000000)));
    TEST_ASSERT_TRUE((p[1] == Color(PixelFormat::RGB888_24BE, 0xcccccc)));
    TEST_ASSERT_TRUE((p[2] == Color(PixelFormat::RGB888_24BE, 0xffffff)));
}
void MonochromeGreen_PALETTE__hasExpectedColors() {
    TEST_ASSERT_TRUE((MonochromeGreen::PALETTE.size() == 5));
    TEST_ASSERT_TRUE((BaseColors::BLACK == MonochromeGreen::PALETTE[0]));
    TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN1 == MonochromeGreen::PALETTE[1]));
    TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN2 == MonochromeGreen::PALETTE[2]));
    TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN3 == MonochromeGreen::PALETTE[3]));
    TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN4 == MonochromeGreen::PALETTE[4]));
}

void PaletteBuiltins__MONOCHROME_AMBER() {
/*    TEST_ASSERT_TRUE((PaletteBuiltins::MONOCHROME_GREEN[0] == Color(PixelFormat::RGB888_24BE, 0x000000)));
    TEST_ASSERT_TRUE((PaletteBuiltins::MONOCHROME_GREEN[1] == Color(PixelFormat::RGB888_24BE, 0xa17414)));
    TEST_ASSERT_TRUE((PaletteBuiltins::MONOCHROME_GREEN[2] == Color(PixelFormat::RGB888_24BE, 0xd3a51a)));
    TEST_ASSERT_TRUE((PaletteBuiltins::MONOCHROME_GREEN[3] == Color(PixelFormat::RGB888_24BE, 0xe5bf13)));
    TEST_ASSERT_TRUE((PaletteBuiltins::MONOCHROME_GREEN[4] == Color(PixelFormat::RGB888_24BE, 0xedde8c)));*/
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(Palette__isAVectorOfColors);
    RUN_TEST(MonochromeGreen_PALETTE__hasExpectedColors);
    UNITY_END();
}
