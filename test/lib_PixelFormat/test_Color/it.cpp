#include "PixelFormat.hpp"
#include <cstdint>
#include <unity.h>

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
