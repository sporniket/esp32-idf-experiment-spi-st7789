// Copyright 2022 David SPORN
// ---
// This file is part of 'esp32-idf-experiment-spi-st7789'.
// ---
// 'esp32-idf-experiment-spi-st7789' is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// 'esp32-idf-experiment-spi-st7789' is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.

// You should have received a copy of the GNU General Public License along
// with 'esp32-idf-experiment-spi-st7789'. If not, see <https://www.gnu.org/licenses/>. 

// std
#include <cstdint>
#include <iostream>

// esp32
#include "driver/spi_master.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "sdkconfig.h"

//
#include "Task.h"

#include "DigitalInputOutputEsp32.hpp"
#include "FeedbackLed.hpp"
#include "GeneralPurposeInputOutput.hpp"

#include "SpiSimplistEsp32.hpp"
#include "St7789Esp32.hpp"

#include "MallocSimpleRegistry.hpp"

#include "ColorRgb48Bpp.hpp"
#include "FrameBuffer12Bpp.hpp"

#include "CanvasRgb444c.hpp"

static const char *TAG_MAIN = "main";

/**
 * Main program.
 */
extern "C" {
void app_main(void); // entry point to the main program
}

// Prepopulated buffer of rgb data to send to display
// -- using 12bpp coding
DRAM_ATTR static const uint8_t RGB_DATA[] = {
        // Horizontal line, 8 pixels, solid color 0x0ff
        0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff,
        // Vertical line, 8 pixels, solid color 0xf0f
        0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x0f,
        // Box, 8x8 pixels, solid color 0xfdb
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 1
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 2
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 3
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 4
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 5
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 6
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 7
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, // line 8
        // Box, 8x8 pixels, solid color 0x000
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 1
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 2
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 3
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 4
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 5
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 6
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 7
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // line 8
};

// EXP DMA capable memory allocation
// -- screen size = 320 pix * 240 pix
// -- with 12bpp pixels, 2 pixels = 3 bytes
// -- memory size = 320 * 240 * 3 / 2 = 115200
uint8_t *ptrScreen;
FrameBuffer12Bpp *frambuff;

// Const for computing canvas dimension
const uint32_t sizeOfScreenLine = (CONFIG_ST7789_WIDTH) + (CONFIG_ST7789_WIDTH >> 1);
const uint32_t sizeOfFrameBuffer = sizeOfScreenLine * (CONFIG_ST7789_HEIGHT);
const uint32_t startOfLine20 = 20 * sizeOfScreenLine;
const uint32_t startOfLine40 = startOfLine20 << 1;
const uint32_t startOfLine60 = startOfLine40 + startOfLine20;
const uint32_t startOfLine80 = startOfLine60 + startOfLine20;
const uint32_t startOfLine100 = startOfLine80 + startOfLine20;
const uint32_t startOfLine120 = startOfLine100 + startOfLine20;
const uint32_t startOfLine140 = startOfLine120 + startOfLine20;
const uint32_t startOfLine160 = startOfLine140 + startOfLine20;
const uint32_t startOfLine180 = startOfLine160 + startOfLine20;
const uint32_t startOfLine200 = startOfLine180 + startOfLine20;
const uint32_t startOfLine220 = startOfLine200 + startOfLine20;
const uint32_t startOfLine240 = startOfLine220 + startOfLine20;
// Canvas #1 : from line 20 to 80 (60 lines)

MemoryArea *canvasBufferArea;
CanvasBufferRgb444c *canvasBuffer;
std::unique_ptr<Palette> greenPalette{PaletteHelper::getPalette(PaletteEnum::MONOCHROME_GREEN)};
CanvasRgb444c *canvas;

FeedbackLed mainLed;
GeneralPurposeInputOutput *gpio;
SpiSimplistEsp32 *spi;
St7789Esp32 *lcd7789;
MallocSimpleRegistry mallocRegistry;

class LedUpdaterTask : public Task {
    public:
    void run(void *data) {
        const TickType_t SLEEP_TIME = 100 / portTICK_PERIOD_MS; // 10 Hz
        while (true) {
            gpio->getDigital()->write(CONFIG_PIN_STATUS_MAIN, mainLed.next());
            vTaskDelay(SLEEP_TIME);
        }
    }
};

LedUpdaterTask *updateTask;

void drawTestFigure(CanvasRgb444c &c) {
    // missing : getWidth(), getHeight() and clear()
    const uint16_t w = CONFIG_ST7789_WIDTH; // missing : c.getWidth()
    const uint16_t h = 60;                  // missing : c.getHeight()
    // missing : c.clear() ;
    // missing : c.setColor(0)
    for (uint16_t row = 0; row < h; row++) {
        c.line(0, row, w - 1, row);
    }
    // missing : c.setColor(2)
    c.line(5, 5, w - 6, h - 6);
    c.line(w - 6, 5, 5, h - 6);
    // missing : c.rectangle(5, 5, w - 6, h - 6)
    c.line(5, 5, w - 6, 5);
    c.line(w - 6, 5, w - 6, h - 6);
    c.line(w - 6, h - 6, 5, h - 6);
    c.line(5, h - 6, 5, 5);
}

void onBeforeTransactionForSt7789(spi_transaction_t *t) {
    ESP_LOGV(TAG_MAIN, "--> onBeforeTransactionForSt7789");
    St7789TransactionExtra *extra = (St7789TransactionExtra *)t->user;
    bool isData = extra->getNature() != COMMAND;
    // manage DC pin
    gpio->getDigital()->write(extra->getTarget()->getDataCommandPin(), isData);
    ESP_LOGV(TAG_MAIN, "set DC pin to %d", isData);
    // manage R/W pin
    if (extra->getTarget()->getReadWritePin() >= 0) {
        bool isRead = extra->getNature() == DATA_READ;
        gpio->getDigital()->write(extra->getTarget()->getReadWritePin(), isRead);
        ESP_LOGV(TAG_MAIN, "set RW pin to %d", isRead);
    }
    ESP_LOGV(TAG_MAIN, "<-- onBeforeTransactionForSt7789");
}

void onAfterTransactionForSt7789(spi_transaction_t *t) {
    ESP_LOGV(TAG_MAIN, "--> onAfterTransactionForSt7789");
    St7789TransactionExtra *extra = (St7789TransactionExtra *)t->user;
    uint8_t requiredDelay = extra->getDelay();
    if (requiredDelay > 0) {
        vTaskDelay(requiredDelay / portTICK_PERIOD_MS);
    }
    ESP_LOGV(TAG_MAIN, "<-- onAfterTransactionForSt7789");
}

void app_main() {
    // ====[ PREPARE ]====
    ESP_LOGI(TAG_MAIN, "Start of app_main()");
    ESP_LOGI(TAG_MAIN, "Start of app_main() -- warn");

    // memory setup
    mallocRegistry["MainFrameBuffer"] =
            new MallocSimpleDescriptor(115200, true, heap_caps_malloc(115200, MALLOC_CAP_DMA));
    if (!mallocRegistry.contains("MainFrameBuffer") || !mallocRegistry["MainFrameBuffer"]->isActuallyAllocated()) {
        ESP_LOGE(TAG_MAIN, "COULD NOT ALLOCATE MEMORY");
        for (;;) {
        } // do not go further
    } else {
        ESP_LOGI(TAG_MAIN, "COULD ALLOCATE MEMORY");
        // can initialise memory
        ptrScreen = (uint8_t *)(mallocRegistry["MainFrameBuffer"]->getStart());
        ESP_LOGI(TAG_MAIN, "Instanciate framebuffer using buffer at %p...", (void *)ptrScreen);
        frambuff = new FrameBuffer12Bpp(CONFIG_ST7789_WIDTH, CONFIG_ST7789_HEIGHT, ptrScreen);
        ESP_LOGI(TAG_MAIN, "Initialize main canvas...");
        canvasBufferArea = new MemoryArea((uint8_t *)(mallocRegistry["MainFrameBuffer"]->getStart() + startOfLine20),
                                          (startOfLine80 - startOfLine20));
        canvasBuffer = new CanvasBufferRgb444c(*canvasBufferArea, CONFIG_ST7789_WIDTH, 60);
        canvas = new CanvasRgb444c(*canvasBuffer, *greenPalette);
        ESP_LOGI(TAG_MAIN, "Initialize screen memory...");
        ColorRgb48Bpp color(0, 0, 0);
        for (uint32_t line = 0; line < frambuff->getHeight(); line++) {
            uint32_t x = 0;
            color.green = ((line >> 4) & 0xf) << 12;
            for (uint8_t band = 0; band < 5 && x < frambuff->getWidth(); band++) {
                color.red = ((band > 0) ? ((band * 4 - 1) & 0xf) : 0) << 12;
                for (uint8_t blue = 0; blue < 16 && x < frambuff->getWidth(); blue++) {
                    color.blue = blue << 12;
                    frambuff->setActiveColor(&color);
                    frambuff->point(x++, line);
                    frambuff->point(x++, line);
                    frambuff->point(x++, line);
                }
            }
        }
    }

    ESP_LOGI(TAG_MAIN, "Initialize gpios...");
    // setup gpio
    gpio = (new GeneralPurposeInputOutput())->withDigital(new DigitalInputOutputEsp32());
    // control panel
    ESP_LOGI(TAG_MAIN, "-- control panel pins");
    gpio->getDigital()->setup(CONFIG_PIN_STATUS_MAIN, PinDirection::WRITE);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_MENU, PinDirection::READ);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_BACK, PinDirection::READ);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_DOWN, PinDirection::READ);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_UP, PinDirection::READ);
    // SPI pins
    ESP_LOGI(TAG_MAIN, "-- SPI pins");
    gpio->getDigital()->setup(CONFIG_PIN_SPI_HOST_SCK, PinDirection::WRITE);
    if (CONFIG_PIN_SPI_HOST_SDO > -1) {
        gpio->getDigital()->setup(CONFIG_PIN_SPI_HOST_SDO, PinDirection::WRITE);
    }
    if (CONFIG_PIN_SPI_HOST_SDI > -1) {
        gpio->getDigital()->setup(CONFIG_PIN_SPI_HOST_SDI, PinDirection::READ);
    }
    if (CONFIG_PIN_SPI_HOST_CS0 > -1) {
        gpio->getDigital()->setup(CONFIG_PIN_SPI_HOST_CS0, PinDirection::WRITE);
    }
    if (CONFIG_PIN_SPI_HOST_CS1 > -1) {
        gpio->getDigital()->setup(CONFIG_PIN_SPI_HOST_CS1, PinDirection::WRITE);
    }
    if (CONFIG_PIN_SPI_HOST_CS2 > -1) {
        gpio->getDigital()->setup(CONFIG_PIN_SPI_HOST_CS2, PinDirection::WRITE);
    }

    // ST7789 pins
    ESP_LOGI(TAG_MAIN, "-- ST7789 pins");
    gpio->getDigital()->setup(CONFIG_PIN_ST7789_DATA_COMMAND, PinDirection::WRITE);
    gpio->getDigital()->setup(CONFIG_PIN_ST7789_RESET, PinDirection::WRITE);
    // -- hardware reset
    ESP_LOGI(TAG_MAIN, "ST7789 hardware reset");
    gpio->getDigital()->setToLow(CONFIG_PIN_ST7789_RESET);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio->getDigital()->setToHigh(CONFIG_PIN_ST7789_RESET);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    // -- FIXME read/write pin

    // setup main led
    // -- start update task
    updateTask = new LedUpdaterTask();
    updateTask->start();
    // -- setup led
    mainLed.setFeedbackSequenceOnce(FeedbackSequence::BLINK_ONCE);
    // setup SPI Port (speed, pins, etc...) as HOST (a set of Peripheral Select pins)
    ESP_LOGI(TAG_MAIN, "Initialize SPI2");
    spi = SpiSimplistEsp32::define() //
                  ->withHostSpecs(SPI2_HOST,
                                  (new SpiSerialPinsMappingSpecs())               //
                                          ->withClock(CONFIG_PIN_SPI_HOST_SCK)    //
                                          ->withDataIn(CONFIG_PIN_SPI_HOST_SDI)   //
                                          ->withDataOut(CONFIG_PIN_SPI_HOST_SDO), //
                                  (new SpiSimplistEsp32BusConfigAsHostSpecs())    //
                                  )
                  ->withDeviceForHostSpecs(
                          SPI2_HOST,
                          (new SpiDeviceForHostSpecs(0))                    //
                                  ->withClockFrequency(SPI_MASTER_FREQ_26M) // FIXME configurable
                                  ->withSelectPin(CONFIG_PIN_SPI_HOST_CS0)  //
                                  ->withExtraSpecs((new SpiSimplistEsp32DeviceInterfaceConfigSpecs())               //
                                                           ->withFlags(SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_NO_DUMMY) //
                                                           ->withQueueSize(7)))
                  ->withPreTransactionListener(SPI2_HOST, 0, onBeforeTransactionForSt7789) //
                  ->withPostTransactionListener(SPI2_HOST, 0, onAfterTransactionForSt7789) //
                  ->build();

    mainLed.setFeedbackSequenceAndLoop(FeedbackSequence::BLINK_TWICE);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Initialize the LCD
    ESP_LOGI(TAG_MAIN, "Initialize LCD...");
    // setup ST7789 (D/C pin, reset pin, backlight pin)
    lcd7789 = St7789Esp32::define()                                        //
                      ->withSpiDeviceHandler(spi->getDevice(SPI2_HOST, 0)) //
                      ->withDataCommandPin(CONFIG_PIN_ST7789_DATA_COMMAND) //
                      ->withPixelFormat(BPP12)                             //
                      ->build();

    // -- send COLMOD(0x53) // (262k RGB, 12bpps) --> done by the instanciation
    mainLed.setFeedbackSequenceAndLoop(FeedbackSequence::BLINK_THRICE);

    ESP_LOGI(TAG_MAIN, "Start drawing...");
    // ====[ EXECUTE ]====
    if (nullptr == ptrScreen) {
        // ~~~~[fill screen by filling 8x8 blocs of solid color 0xfdb]~~~~
        for (uint16_t i = 0; i < 240; i += 8) {     // line
            for (uint16_t j = 0; j < 240; j += 8) { // col
                lcd7789->await(lcd7789->caset(j, j + 7));
                lcd7789->await(lcd7789->raset(i, i + 7));
                lcd7789->await(lcd7789->ramwr(96, (uint8_t *)RGB_DATA + 24));
            }
        }
        // ~~~~[clear screen by filling 8x8 blocs of solid black (0x000)]~~~~
        for (uint16_t i = 0; i < 240; i += 8) {     // line
            for (uint16_t j = 0; j < 240; j += 8) { // col
                lcd7789->await(lcd7789->caset(j, j + 7));
                lcd7789->await(lcd7789->raset(i, i + 7));
                lcd7789->await(lcd7789->ramwr(96, (uint8_t *)RGB_DATA + 120));
            }
        }
    } else {
        ESP_LOGI(TAG_MAIN, "BEGIN Copy screen memory...");
        // copy ptrScreen line by line
        lcd7789->await(lcd7789->caset(0, frambuff->getWidth() - 1));
        uint8_t *currentData = ptrScreen;
        uint32_t wrSize = (frambuff->getWidth() + (frambuff->getWidth() >> 1)) * 4;
        for (uint32_t line = 0; line < frambuff->getHeight(); line += 4) {
            lcd7789->await(lcd7789->raset(line, line + 3));
            lcd7789->await(lcd7789->ramwr(wrSize, currentData));
            currentData += wrSize;
        }
        ESP_LOGI(TAG_MAIN, "DONE Copy screen memory.");
    }
    // ~~~~[plot at (8,8), color 0xfff]~~~~
    // ST7789 --> CASET(8,8)
    lcd7789->await(lcd7789->caset(8, 8));
    // ST7789 --> RASET(8,8)
    lcd7789->await(lcd7789->raset(8, 8));
    // ST7789 --> RAMWR(0xff,0xf0)
    lcd7789->await(lcd7789->ramwr(2, (uint8_t *)RGB_DATA));
    // ~~~~[plot hline at (16,8)-->(23,8), color 0x0ff]~~~~
    // ST7789 --> CASET(16,23)
    lcd7789->await(lcd7789->caset(16, 23));
    // ST7789 --> RASET(8,8) // could be optimised out
    lcd7789->await(lcd7789->raset(8, 8));
    // ST7789 --> RAMWR(0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff)
    lcd7789->await(lcd7789->ramwr(12, (uint8_t *)RGB_DATA));
    // ~~~~[plot vline at (8,16)-->(8,23), color 0xf0f]~~~~
    // ST7789 --> CASET(8,8)
    lcd7789->await(lcd7789->caset(8, 8));
    // ST7789 --> RASET(16,23)
    lcd7789->await(lcd7789->raset(16, 23));
    // ST7789 --> RAMWR(0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f)
    lcd7789->await(lcd7789->ramwr(12, (uint8_t *)RGB_DATA + 12));
    // ~~~~[plot filled box at (16,16)-->(23,23), color 0xfdb]~~~~
    // ST7789 --> CASET(116,123)
    lcd7789->await(lcd7789->caset(16, 23));
    // ST7789 --> RASET(116,123)
    lcd7789->await(lcd7789->raset(16, 23));
    // ST7789 --> RAMWR(
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb, // line 1
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb, // line 2
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb, // line 3
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb, // line 4
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb, // line 5
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb, // line 6
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb, // line 7
    //                0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb,0xfd,0xbf,0xdb  // line 8
    //            )
    lcd7789->await(lcd7789->ramwr(96, (uint8_t *)RGB_DATA + 24));

    // use the canvas
    drawTestFigure(*canvas);
    // FIXME : send the whole in one go seems too much, or is it my code ? (most probably my code)
    // Most likely : for long transfert, do not await, and regularly poll.
    // and for still awaiting, see how to encapsulate, but most likely not in the main function
    // ST7789 --> CASET(116,123)
    // lcd7789->await(lcd7789->caset(0, CONFIG_ST7789_WIDTH - 1));
    // ST7789 --> RASET(116,123)
    // lcd7789->await(lcd7789->raset(20, 79));
    // ST7789 --> RAMWR(canvasBufferArea)
    // lcd7789->await(lcd7789->ramwr(canvasBufferArea->getLength(), canvasBufferArea->getStart()));

    // Meanwhile, brut copy of the screen...
    ESP_LOGI(TAG_MAIN, "BEGIN Copy screen memory...");
    // copy ptrScreen line by line
    lcd7789->await(lcd7789->caset(0, frambuff->getWidth() - 1));
    uint8_t *currentData = ptrScreen;
    uint32_t wrSize = (frambuff->getWidth() + (frambuff->getWidth() >> 1)) * 4;
    for (uint32_t line = 0; line < frambuff->getHeight(); line += 4) {
        lcd7789->await(lcd7789->raset(line, line + 3));
        lcd7789->await(lcd7789->ramwr(wrSize, currentData));
        currentData += wrSize;
    }
    ESP_LOGI(TAG_MAIN, "DONE Copy screen memory.");

    // ====[ THE END ]====
    ESP_LOGI(TAG_MAIN, "You should see something on the screen...");
    mainLed.setFeedbackSequenceAndLoop(FeedbackSequence::FALTER_THRICE);
}