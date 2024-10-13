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

void from_red_green_blue__should_have_expected_properties() {
    Color test(0xffffffff, 0x88888888, 0x44444444);
    TEST_ASSERT_TRUE((test.getRed() == 0xffffffff));
    TEST_ASSERT_TRUE((test.getGreen() == 0x88888888));
    TEST_ASSERT_TRUE((test.getBlue() == 0x44444444));
}

void asRgb332__should_convert_correctly() {
    Color test(0xffffffff, 0x88888888, 0x44444444);
    TEST_ASSERT_TRUE((test.asRgb332() == 0b11110001)); // 111.100.01
}

void from_rgb332__should_have_expected_properties() {
    Color test(PixelFormat::RGB332_8BE, 0b11110001);
    TEST_ASSERT_TRUE((test.getRed() == 0xffffffff));
    TEST_ASSERT_TRUE((test.getGreen() == 0x92929292));
    TEST_ASSERT_TRUE((test.getBlue() == 0x55555555));
}

void asRgb444__should_convert_correctly() {
    Color test(0xffffffff, 0x88888888, 0x44444444);
    TEST_ASSERT_TRUE((test.asRgb444() == 0b111110000100)); // 1111.1000.0100
}

void from_rgb444__should_have_expected_properties() {
    Color test(PixelFormat::RGB444C_12BE, 0b111110000100);
    TEST_ASSERT_TRUE((test.getRed() == 0xffffffff));
    TEST_ASSERT_TRUE((test.getGreen() == 0x88888888));
    TEST_ASSERT_TRUE((test.getBlue() == 0x44444444));
}

void asRgb555__should_convert_correctly() {
    Color test(0xffffffff, 0x88888888, 0x44444444);
    TEST_ASSERT_TRUE((test.asRgb555() == 0b111111000101000)); // 11111.10001.01000
}

void from_rgb555__should_have_expected_properties() {
    Color test(PixelFormat::RGB555_16BE, 0b111111000101000);
    TEST_ASSERT_TRUE((test.getRed() == 0xffffffff));
    TEST_ASSERT_TRUE((test.getGreen() == 0x8c8c8c8c));
    TEST_ASSERT_TRUE((test.getBlue() == 0x42424242));
}

void asRgb565__should_convert_correctly() {
    Color test(0xffffffff, 0x88888888, 0x44444444);
    TEST_ASSERT_TRUE((test.asRgb565() == 0b1111110001001000)); // 11111.100010.01000
}

void from_rgb565__should_have_expected_properties() {
    Color test(PixelFormat::RGB565_16BE, 0b1111110001001000);
    TEST_ASSERT_TRUE((test.getRed() == 0xffffffff));
    TEST_ASSERT_TRUE((test.getGreen() == 0x8a8a8a8a));
    TEST_ASSERT_TRUE((test.getBlue() == 0x42424242));
}

void asRgb666__should_convert_correctly() {
    Color test(0xffffffff, 0x88888888, 0x44444444);
    TEST_ASSERT_TRUE((test.asRgb666() == 0b111111100010010001)); // 111111.100010.010001
}

void from_rgb666__should_have_expected_properties() {
    Color test(PixelFormat::RGB666_24BE, 0b111111100010010001);
    TEST_ASSERT_TRUE((test.getRed() == 0xffffffff));
    TEST_ASSERT_TRUE((test.getGreen() == 0x8a8a8a8a));
    TEST_ASSERT_TRUE((test.getBlue() == 0x45454545));
}

void asRgb888__should_convert_correctly() {
    Color test(0xffffffff, 0x88888888, 0x44444444);
    TEST_ASSERT_TRUE((test.asRgb888() == 0xff8844));
}

void from_rgb888__should_have_expected_properties() {
    Color test(PixelFormat::RGB888_24BE, 0xff8844);
    TEST_ASSERT_TRUE((test.getRed() == 0xffffffff));
    TEST_ASSERT_TRUE((test.getGreen() == 0x88888888));
    TEST_ASSERT_TRUE((test.getBlue() == 0x44444444));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(from_red_green_blue__should_have_expected_properties);
    RUN_TEST(asRgb332__should_convert_correctly);
    RUN_TEST(from_rgb332__should_have_expected_properties);
    RUN_TEST(asRgb444__should_convert_correctly);
    RUN_TEST(from_rgb444__should_have_expected_properties);
    RUN_TEST(asRgb555__should_convert_correctly);
    RUN_TEST(from_rgb555__should_have_expected_properties);
    RUN_TEST(asRgb565__should_convert_correctly);
    RUN_TEST(from_rgb565__should_have_expected_properties);
    RUN_TEST(asRgb666__should_convert_correctly);
    RUN_TEST(from_rgb666__should_have_expected_properties);
    RUN_TEST(asRgb888__should_convert_correctly);
    RUN_TEST(from_rgb888__should_have_expected_properties);
    UNITY_END();
}
