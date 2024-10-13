#include "Color.hpp"
#include <unity.h>


/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void MonochromeGreenColors__has_expected_colors() {
    {
        const Color a(PixelFormat::RGB888_24BE, 0x399b6a) ;

        //TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN1 == a));
        //TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN1.asRgb888() == 0x399b6a));
    }
    /*TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN2 == Color(PixelFormat::RGB888_24BE, 0x42d297)));
    TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN3 == Color(PixelFormat::RGB888_24BE, 0x73fdbf)));
    TEST_ASSERT_TRUE((MonochromeGreenColors::GREEN4 == Color(PixelFormat::RGB888_24BE, 0xbaffe0)));*/
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(MonochromeGreenColors__has_expected_colors);
    UNITY_END();
}
