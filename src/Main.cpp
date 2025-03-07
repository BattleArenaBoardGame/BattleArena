/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <FastLED.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Game.h>
#include <MapItems/Hero.h>
#include <Screens/MovementScreen.h>
#include <AccelStepper.h>
#include <GameBoardActuator.h>

/*
 * Chip used in board is FT5436
 * Library used: https://github.com/DustinWatts/FT6236
 * Just changed CHIPID and VENDID
 * Library is included in the project so it does not need to be downloaded
 */
#include "FT6236.h"
#define V2_1

#define STEPS_PER_REV 200  // Full steps per revolution
#define MICROSTEPS 1      // Microstepping setting on DRV8825

GameBoardActuator gameBoardActuator = GameBoardActuator(18, 19, 2, 26, 34, 35);

// TFT SPI
#define TFT_LED 33		// TFT backlight pin
#define TFT_LED_PWM 255 // dutyCycle 0-255 last minimum was 15
#define TFT_RES_X 480
#define TFT_RES_Y 320
#define CENTRE_X TFT_RES_X / 2
#define TFT_GREY 0x7BEF
#define RECT_SIZE_X 100
#define RECT_SIZE_Y 70


#define SD_CS_PIN 4
#define POWER_OFF_PIN 17


#define NUM_LEDS 256
#define DATA_PIN 6
CRGB leds[NUM_LEDS];

FT6236 ts = FT6236(480, 320);	// Create object for Touch library
TFT_eSPI tft = TFT_eSPI();  	// Invoke custom library with default width and height
Game game = Game();
Screen *screen = nullptr;
bool isTouching = true;


// Delay function with milis()
uint8_t isTouched(uint16_t time)
{
	unsigned long currentMillis = millis();
	unsigned long previousMillis = currentMillis;
	while ((currentMillis - previousMillis) < time)
	{
		if (ts.touched())
		{
			return 1;
		}
		currentMillis = millis();
	}
	return 0;
}

#ifndef PIO_UNIT_TESTING

void setup(){
	// Enable the LED backlight.
	pinMode(TFT_LED, OUTPUT);
	analogWrite(TFT_LED, TFT_LED_PWM);

	randomSeed(analogRead(0)); // Generate the random seed by reading from unconnected pin.
	Serial.begin(115200); // Start the Serial Console.

	// Initialize the touchscreen. The 40 is the sensitivity.
	if (!ts.begin(40)) {
		Serial.println("Unable to start the capacitive touchscreen.");
	}
	ts.setRotation(3);

	// Initialize display.
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);

	game.start(3, 3);
	screen = new MovementScreen(&tft, &game);

	gameBoardActuator.setup();

	FastLED.addLeds<NEOPIXEL, 23>(leds, NUM_LEDS);
}

int current_led = 0;

void loop()
{
	Screen *new_screen = nullptr;
	gameBoardActuator.run();

	current_led++;
	if (current_led == NUM_LEDS) {
		current_led = 0;
	}
	if (current_led == 0) {
		leds[NUM_LEDS - 1] = CRGB::Black;
	}
	else {
		leds[current_led - 1] = CRGB::Black;
	}
	leds[current_led] = CHSV(random8(),255,255);;
	FastLED.show();
	// // delay(50);


	if (ts.touched()) {
		TS_Point p = ts.getPoint();
		if (!isTouching) {
			new_screen = screen->touch(p.x, p.y);
			gameBoardActuator.rotateToTeam(game.getCurrentHero()->getTeam());
			gameBoardActuator.tilt(game.getMap()->isMapVisibleToBothTeams());
		}
		isTouching = true;
	}
	else {
		if (isTouching) {
			new_screen = screen->refresh();
		}
		isTouching = false;
	}
	if (new_screen) {
		delete screen;
		screen = new_screen;
		screen->refresh();
	}
}

#endif
