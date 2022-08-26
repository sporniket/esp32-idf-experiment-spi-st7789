#include "sdkconfig.h"

#include "FeedbackLed.hpp"
#include "DigitalInputOutputEsp32.hpp"
#include "GeneralPurposeInputOutput.hpp"

#include "Task.h"

/**
 * Main program.
 */
extern "C" {
	void app_main(void); //entry point to the main program
}


FeedbackLed mainLed ;
GeneralPurposeInputOutput* gpio ;

class LedUpdaterTask : public Task {
    public:
        void run(void *data) {
            const TickType_t SLEEP_TIME = 100 / portTICK_PERIOD_MS ; // 10 Hz
            while(true) {
                gpio->getDigital()->write(CONFIG_PIN_STATUS_MAIN, mainLed.next());
                vTaskDelay(SLEEP_TIME);
            }
            
        }
} ;

LedUpdaterTask *updateTask ;

void app_main() {
	// ====[ PREPARE ]====

	// setup gpio
	gpio = (new GeneralPurposeInputOutput())
		->withDigital(new DigitalInputOutputEsp32()) ;
    gpio->getDigital()->setup(CONFIG_PIN_STATUS_MAIN, WRITE) ;

	// setup main led
	// -- start update task
	updateTask = new LedUpdaterTask() ;
	updateTask->start();
	// -- setup led
	mainLed.setFeedbackSequenceOnce(FeedbackSequence::BLINK_ONCE);

	// setup SPI Port (speed, pins, etc...) as HOST (a set of Peripheral Select pins)
	// setup ST7789 (D/C pin, reset pin, backlight pin)
	// -- send SWRESET
	// -- send COLMOD(0x53) // (262k RGB, 12bpps)

	// ====[ EXECUTE ]====
	// ~~~~[plot at (8,8), color 0xfff]~~~~
	// ST7789 --> CASET(8,8)
	// ST7789 --> RASET(8,8)
	// ST7789 --> RAMWR(0xff,0xf0)
	// ~~~~[plot hline at (16,8)-->(23,8), color 0x0ff]~~~~
	// ST7789 --> CASET(16,23)
	// ST7789 --> RASET(8,8) // could be optimised out
	// ST7789 --> RAMWR(0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff)
	// ~~~~[plot vline at (8,16)-->(8,23), color 0xf0f]~~~~
	// ST7789 --> CASET(8,8) 
	// ST7789 --> RASET(16,23)
	// ST7789 --> RAMWR(0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f,0xf0,0xff,0x0f)
	// ~~~~[plot filled box at (16,16)-->(23,23), color 0xfdb]~~~~
	// ST7789 --> CASET(8,8) 
	// ST7789 --> RASET(16,23)
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


	// ====[ THE END ]====
	mainLed.setFeedbackSequenceAndLoop(FeedbackSequence::BLINK_TWICE);
}