#include <unity.h>
#include <memory>

#include "CanvasRgb444c.hpp"

/**
 * @brief Before test
 */
void setUp(void) {}

/**
 * @brief After test.
 */
void tearDown(void) {}

void canvas_plot__should_modify_one_pixel() {
    uint8_t buffer[24 * 16]; // for 16x16 pixels
    MemoryArea bufferArea(buffer, sizeof(buffer));
    CanvasBufferRgb444c cbuf(bufferArea, 16, 16);
    std::unique_ptr<Palette> p{PaletteHelper::getPalette(PaletteEnum::MONOCHROME_GREEN)};
    CanvasRgb444c c(cbuf, *p);
    for (uint16_t i = 0 ; i < 16 ; i++) {
        c.plot(i,i) ;
    }
    uint8_t expected[24 * 16] {
        //line 0
        0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 1
        0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 2
        0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 3
        0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 4
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 5
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 6
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 7
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 8
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 9
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 10
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 11
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 12
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,
        //line 13
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,
        //line 14
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,
        //line 15
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe
    } ;
    TEST_ASSERT_EQUAL_UINT8_ARRAY((expected), (buffer), (sizeof(expected)));

}

void canvas_line__should_draw_rasterized_line() {
    uint8_t buffer[24 * 16]; // for 16x16 pixels
    MemoryArea bufferArea(buffer, sizeof(buffer));
    CanvasBufferRgb444c cbuf(bufferArea, 16, 16);
    std::unique_ptr<Palette> p{PaletteHelper::getPalette(PaletteEnum::MONOCHROME_GREEN)};
    CanvasRgb444c c(cbuf, *p);
    // horizontal
    c.line(9,0,14,0);
    // horizontal-ish
    c.line(5,0,10,3) ;
    c.line(10,3,5,6) ;
    c.line(10,3,15,8) ; // diagonal
    // vertical-ish
    c.line(0,5,3,10) ;
    c.line(3,10,6,5) ;
    // vertical
    c.line(0,9,0,14) ;
    uint8_t expected[24 * 16] {
        //line 0 / index 0
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0xbf,0xeb,0xfe,/**/0xbf,0xeb,0xfe,/**/0xbf,0xe0,0x00,
        //line 1 / index 24
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xeb,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 2 / index 48
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xeb,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 3 / index 72
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 4 / index 96
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xeb,0xfe,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 5 / index 120
        0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xeb,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,
        //line 6 / index 144
        0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,
        //line 7 / index 168
        0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0xbf,0xe0,0x00,
        //line 8 / index 192
        0x00,0x00,0x00,/**/0xbf,0xe0,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x0b,0xfe,
        //line 9 / index 216
        0xbf,0xe0,0x00,/**/0xbf,0xe0,0x00,/**/0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 10 / index 240
        0xbf,0xe0,0x00,/**/0x00,0x0b,0xfe,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 11 / index 264
        0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 12 / index 288
        0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 13 / index 312
        0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 14 / index 336
        0xbf,0xe0,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,
        //line 15 / index 360
        0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00,/**/0x00,0x00,0x00
    } ;
    TEST_ASSERT_EQUAL_UINT8_ARRAY((expected), (buffer), (sizeof(expected)));

}

void canvas_setColor__should_change_active_color() {
    uint8_t buffer[24 * 16]; // for 16x16 pixels
    MemoryArea bufferArea(buffer, sizeof(buffer));
    CanvasBufferRgb444c cbuf(bufferArea, 16, 16);
    std::unique_ptr<Palette> p{PaletteHelper::getPalette(PaletteEnum::MONOCHROME_GREEN)};
    CanvasRgb444c c(cbuf, *p);

    c.setShapeColorIndex(1) ;
    c.plot(0,0);
    c.setShapeColorIndex(2) ; 
    c.plot(1,0);
    uint8_t expected[3] {0x39,0x64,0xd9} ;
    TEST_ASSERT_EQUAL_UINT8_ARRAY((expected), (buffer), (sizeof(expected)));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(canvas_plot__should_modify_one_pixel);
    RUN_TEST(canvas_line__should_draw_rasterized_line);
    RUN_TEST(canvas_setColor__should_change_active_color);
    UNITY_END();
}
