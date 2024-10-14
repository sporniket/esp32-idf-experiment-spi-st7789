#include <unity.h>
#include <memory>

#include "MemoryArea.hpp"
#include "Palette.hpp"
#include "PixelFormat.hpp"


enum class CanvasReturnCode {
    OK = 0,
    // == out of bounds ==
    KO__OUT_OF_BOUNDS, // unspecified source
    KO__OUT_OF_BOUNDS__CANVAS_INVALID_DIMENSIONS, // canvas width and height MUST be > 0
    KO__OUT_OF_BOUNDS__X,
    KO__OUT_OF_BOUNDS__Y,
    KO__OUT_OF_BOUNDS__W_ZERO, // w MUST be > 0
    KO__OUT_OF_BOUNDS__W,
    KO__OUT_OF_BOUNDS__X2,
    KO__OUT_OF_BOUNDS__Y2,
    KO__OUT_OF_BOUNDS__H_ZERO, // h MUST be > 0
    KO__OUT_OF_BOUNDS__H

};

class CanvasBufferRgb444c {
    public:
    CanvasBufferRgb444c(const MemoryArea &buffer, uint16_t width, uint16_t height)
        : buffer(buffer), width(width < 0x400 ? width & (0x3fe) : 0x400),
          height(height < 0x400 ? height & (0x3fe) : 0x3ff), lineOffset(width + (width >>1)), invalidDimensions((width == 0) || (height == 0)) {
            setupColorBrushes(0, color_even, color_odd, color_both) ;
            setupColorBrushes(0, backgroundColor_even, backgroundColor_odd, backgroundColor_both) ;
            // clear buffer
            for (uint8_t* current = buffer.getStart() ; current < buffer.getEnd() ; current++) {
                *current = 0;
            }
          }
    
    CanvasReturnCode hline(uint16_t x, uint16_t y, uint16_t w) {
        if (invalidDimensions) return CanvasReturnCode::KO__OUT_OF_BOUNDS__CANVAS_INVALID_DIMENSIONS;
        if (x >= width) return CanvasReturnCode::KO__OUT_OF_BOUNDS__X;
        if (y >= height) return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y;
        if (w == 0) return CanvasReturnCode::KO__OUT_OF_BOUNDS__W_ZERO;
        if (w >= width) return CanvasReturnCode::KO__OUT_OF_BOUNDS__W;
        uint16_t x2 = x + w - 1;
        if (x2 >= width) return CanvasReturnCode::KO__OUT_OF_BOUNDS__X2;

        return unsafe_hline(x, y, w);
    }

    CanvasReturnCode unsafe_hline(uint16_t x, uint16_t y, uint16_t w) {
        // move to start of working area
        uint16_t x_evenStart = x & 0x3fe ; // work by pairs of pixels
        uint16_t x_pair = x_evenStart >> 1 ;
        uint8_t* current = buffer.getStart() + lineOffset * y + x_evenStart + x_pair ;

        bool x_odd = (1 == (x & 1)) ;
        if (w == 1) {
            changeSinglePixel(current, x_odd, color_odd, color_even);
        } else {
            if (x_odd) {
                changeSingleOddPixel(current, color_odd);
                w--;
            } else {
                changePairOfPixel(current, color_both) ;
                w -=2 ;
            }
            current += 3;
            while (w > 1)
            {
                changePairOfPixel(current, color_both) ;
                w -=2 ;
                current += 3;
            }
            if (w == 1) {
                changeSingleEvenPixel(current, color_even);
            }
        }
        return CanvasReturnCode::OK;
    }

    CanvasReturnCode vline(uint16_t x, uint16_t y, uint16_t h) {
        if (invalidDimensions) return CanvasReturnCode::KO__OUT_OF_BOUNDS__CANVAS_INVALID_DIMENSIONS;
        if (x >= width) return CanvasReturnCode::KO__OUT_OF_BOUNDS__X;
        if (y >= height) return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y;
        if (h == 0) return CanvasReturnCode::KO__OUT_OF_BOUNDS__H_ZERO;
        if (h >= height) return CanvasReturnCode::KO__OUT_OF_BOUNDS__H;
        uint16_t y2 = y + h - 1;
        if (y2 >= height) return CanvasReturnCode::KO__OUT_OF_BOUNDS__Y2;

        return unsafe_vline(x, y, h);
    }

    CanvasReturnCode unsafe_vline(uint16_t x, uint16_t y, uint16_t h) {
        // move to start of working area
        uint16_t x_evenStart = x & 0x3fe ; // work by pairs of pixels
        uint16_t x_pair = x_evenStart >> 1 ;
        uint8_t* current = buffer.getStart() + lineOffset * y + x_evenStart + x_pair ;

        bool x_odd = (1 == (x & 1)) ;
        while(h > 0) {
            changeSinglePixel(current, x_odd, color_odd, color_even);
            h--;
            current += lineOffset ;
        }
        return CanvasReturnCode::OK;
    }

    void setColor(Color &color) {
        setupColorBrushes(color.asRgb444(), color_even, color_odd, color_both) ;
    }

    void setBackgroundColor(Color &color) {
        setupColorBrushes(color.asRgb444(), backgroundColor_even, backgroundColor_odd, backgroundColor_both) ;
    }

    private:

    void changeSinglePixel(uint8_t* pairStart, bool odd, uint8_t* c_odd, uint8_t* c_even) {
        if (odd) {
            changeSingleOddPixel(pairStart, c_odd);
        } else {
            changeSingleEvenPixel(pairStart, c_even);
        }
    }
    void changeSingleOddPixel(uint8_t* pairStart, uint8_t* c) {
        pairStart[1] = c[0] | (pairStart[1] & 0xf0) ;
        pairStart[2] = c[1] ;
    }
    void changeSingleEvenPixel(uint8_t* pairStart, uint8_t* c) {
        pairStart[0] = c[0] ;
        pairStart[1] = c[1] | (pairStart[1] & 0xf) ;
    }
    void changePairOfPixel(uint8_t* pairStart, uint8_t* c) {
        pairStart[0] = c[0] ;
        pairStart[1] = c[1] ;
        pairStart[2] = c[2] ;
    }

    /**
     * @brief Setup a triplet of uint8_t buffers used as brushes for basic drawing operations.
     * 
     * @param c the encoded value, i.e `0RGB` (each position is a nibble).
     * @param even the brush to change a single pixel at even x coordinate (0, 2, 4,...)
     * @param odd the brush to change a single pixel at odd x coordinate (1, 3, 5,...)
     * @param both the brush to change a pair of pixels at even x coordinate (0, 2, 4,...)
     */
    void setupColorBrushes(uint16_t c, uint8_t* even, uint8_t* odd, uint8_t* both) {
        even[1] = (c << 4) & 0xf0; // B0
        odd[1] = c & 0xff; // GB

        c = c >> 4 ; // c = 00RG
        even[0] = c & 0xff ; // RG

        c = c >> 4 ; // c = 000R
        odd[0] = c & 0xf; // 0R

        both[0] = even[0]; // RG
        both[1] = even[1] | odd[0] ; // BR
        both[2] = odd[1]; // GB
    }


    const MemoryArea &buffer;
    const uint16_t width;
    const uint16_t height;
    const uint16_t lineOffset;
    const bool invalidDimensions;
    /**
     * @brief [shape color] Value to use to change single pixel at odd x coordinate (1, 3, 5,...).
     */
    uint8_t color_odd[2];
    /**
     * @brief [shape color] Value to use to change single pixel at even x coordinate (0, 2, 4,...).
     */
    uint8_t color_even[2];
    /**
     * @brief [shape color] Value to use to change a pair of pixels at even x coordinate (0, 2, 4,...).
     */
    uint8_t color_both[3];

    /**
     * @brief [background color] Value to use to change single pixel at odd x coordinate (1, 3, 5,...).
     */
    uint8_t backgroundColor_odd[2];
    /**
     * @brief [background color] Value to use to change single pixel at even x coordinate (0, 2, 4,...).
     */
    uint8_t backgroundColor_even[2];
    /**
     * @brief [background color] Value to use to change a pair of pixels at even x coordinate (0, 2, 4,...).
     */
    uint8_t backgroundColor_both[3];
};

class CanvasRgb444c {
    public:
    CanvasRgb444c(CanvasBufferRgb444c &buffer, Palette &palette) : buffer(buffer), palette(palette) {
        if (this->palette.size() >0) {
            this->buffer.setColor(palette.back()) ;
        }
    }

    void plot(uint16_t x, uint16_t y) {
        buffer.hline(x,y,1) ;
    }

    private:
    CanvasBufferRgb444c &buffer;
    Palette &palette;
};

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

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(canvas_plot__should_modify_one_pixel);
    UNITY_END();
}
