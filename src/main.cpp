
// std
#include <cstdint>
#include <iostream>

// esp32
#include "driver/spi_master.h"
#include "esp_log.h"
#include "sdkconfig.h"

//
#include "Task.h"

#include "DigitalInputOutputEsp32.hpp"
#include "FeedbackLed.hpp"
#include "GeneralPurposeInputOutput.hpp"

#include "SpiSimplistEsp32.hpp"
#include "St7789Esp32.hpp"

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
        0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb, 0xfd, 0xbf, 0xdb  // line 8
};

FeedbackLed mainLed;
GeneralPurposeInputOutput *gpio;
SpiSimplistEsp32 *spi;
St7789Esp32 *lcd7789;

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

void onBeforeTransactionForSt7789(spi_transaction_t *t) {
    ESP_LOGI(TAG_MAIN,"--> onBeforeTransactionForSt7789") ;
    St7789TransactionExtra *extra = (St7789TransactionExtra *)t->user;
    bool isData = extra->nature != COMMAND;
    // manage DC pin
    gpio->getDigital()->write(extra->target->getDataCommandPin(), isData);
    ESP_LOGI(TAG_MAIN,"set DC pin to %d", isData) ;
    // manage R/W pin
    if (extra->target->getReadWritePin() >= 0) {
        bool isRead = extra->nature == DATA_READ ;
        gpio->getDigital()->write(extra->target->getReadWritePin(), isRead);
        ESP_LOGI(TAG_MAIN,"set RW pin to %d", isRead) ;
    }
    ESP_LOGI(TAG_MAIN,"<-- onBeforeTransactionForSt7789") ;
}

void app_main() {
    // ====[ PREPARE ]====
    ESP_LOGI(TAG_MAIN, "Start of app_main()");
    ESP_LOGI(TAG_MAIN, "Start of app_main() -- warn");

    // setup gpio
    gpio = (new GeneralPurposeInputOutput())->withDigital(new DigitalInputOutputEsp32());
    // control panel
    gpio->getDigital()->setup(CONFIG_PIN_STATUS_MAIN, PinDirection::WRITE);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_MENU, PinDirection::READ);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_BACK, PinDirection::READ);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_DOWN, PinDirection::READ);
    gpio->getDigital()->setup(CONFIG_PIN_BUTTON_UP, PinDirection::READ);
    // SPI pins
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
    gpio->getDigital()->setup(CONFIG_PIN_ST7789_DATA_COMMAND, PinDirection::WRITE);
    // -- FIXME read/write pin

    // setup main led
    // -- start update task
    updateTask = new LedUpdaterTask();
    updateTask->start();
    // -- setup led
    mainLed.setFeedbackSequenceOnce(FeedbackSequence::BLINK_ONCE);
    // setup SPI Port (speed, pins, etc...) as HOST (a set of Peripheral Select pins)
    spi = SpiSimplistEsp32::define()                                                        //
                  ->withHostSpecs(SPI2_HOST, (new SpiSerialPinsMapping())                   //
                                                     ->withClock(CONFIG_PIN_SPI_HOST_SCK)   //
                                                     ->withDataIn(CONFIG_PIN_SPI_HOST_SDI)  //
                                                     ->withDataOut(CONFIG_PIN_SPI_HOST_SDO) //
                                  )
                  ->withDeviceForHostSpecs(SPI2_HOST,
                                           (new SpiDeviceForHostSpecs(0))                    //
                                                   ->withClockFrequency(SPI_MASTER_FREQ_26M) // FIXME configurable
                                                   ->withSelectPin(CONFIG_PIN_SPI_HOST_CS0)  //
                                           )
                  ->withPreTransactionListener(SPI2_HOST, 0, onBeforeTransactionForSt7789)
                  ->build();

    mainLed.setFeedbackSequenceAndLoop(FeedbackSequence::BLINK_TWICE);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Initialize the LCD
    // setup ST7789 (D/C pin, reset pin, backlight pin)
    lcd7789 = St7789Esp32::define()                                        //
                      ->withSpiDeviceHandler(spi->getDevice(SPI2_HOST, 0)) //
                      ->withDataCommandPin(CONFIG_PIN_ST7789_DATA_COMMAND) //
                      ->withPixelFormat(BPP12)                             //
                      ->build();
    /*

        // -- send COLMOD(0x53) // (262k RGB, 12bpps) --> done by the instanciation
        mainLed.setFeedbackSequenceAndLoop(FeedbackSequence::BLINK_THRICE);

        // ====[ EXECUTE ]====
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
        // ST7789 --> CASET(8,8)
        lcd7789->await(lcd7789->caset(16, 23));
        // ST7789 --> RASET(16,23)
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
    */
    // ====[ THE END ]====
    mainLed.setFeedbackSequenceAndLoop(FeedbackSequence::FALTER_THRICE);
}