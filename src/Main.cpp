/*
This is part of Battle Arena Board Game project.

@section author Author
Written by Jan Kaluza <hanzz.k@gmail.com>

@section license License
MIT license, all text above must be included in any redistribution
*/
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Game.h>
#include <Screens/MovementScreen.h>
#include <AccelStepper.h>

/*
 * Chip used in board is FT5436
 * Library used: https://github.com/DustinWatts/FT6236
 * Just changed CHIPID and VENDID
 * Library is included in the project so it does not need to be downloaded
 */
#include "FT6236.h"
#define V2_1

#define dirPin 2
#define stepPin 26
#define STEPS_PER_REV 200  // Full steps per revolution
#define MICROSTEPS 1      // Microstepping setting on DRV8825

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

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

void refreshColorMatrix() {
	ColorMatrix *cm = game.getMap()->getColorMatrix();

	for(int x = 0; x < MAP_W; x++) {
		for(int y = 0; y < MAP_H; y++) {
			uint32_t color = (*cm)[y][x];
			tft.fillRect(x*10, y*10, 10, 10, color);
		}
	}
}

// Print initial screen
void touchScreen()
{
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextFont(4);
	// Print SD test button
	tft.setTextColor(TFT_WHITE, TFT_BROWN);
	tft.fillRoundRect(0, TFT_RES_Y - RECT_SIZE_Y, RECT_SIZE_X, RECT_SIZE_Y, 3, TFT_BROWN);
	tft.setTextDatum(MC_DATUM);
	tft.drawString("SD test", (RECT_SIZE_X / 2), TFT_RES_Y - (RECT_SIZE_Y / 2));
	// Print I2C scanner button
	tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
	tft.fillRoundRect(RECT_SIZE_X + 27, TFT_RES_Y - RECT_SIZE_Y, RECT_SIZE_X, RECT_SIZE_Y, 3, TFT_DARKGREEN);
	tft.setTextDatum(BC_DATUM);
	tft.drawString("I2C", (RECT_SIZE_X / 2) + RECT_SIZE_X + 27, TFT_RES_Y - (RECT_SIZE_Y / 2));
	tft.setTextDatum(TC_DATUM);
	tft.drawString("scanner", (RECT_SIZE_X / 2) + RECT_SIZE_X + 27, TFT_RES_Y - (RECT_SIZE_Y / 2));
	// Print Power off button
	tft.setTextColor(TFT_WHITE, TFT_RED);
	tft.fillRoundRect(2 * RECT_SIZE_X + 54, TFT_RES_Y - RECT_SIZE_Y, RECT_SIZE_X, RECT_SIZE_Y, 3, TFT_RED);
	tft.setTextDatum(BC_DATUM);
	tft.drawString("Power", (RECT_SIZE_X / 2) + 2 * RECT_SIZE_X + 54, TFT_RES_Y - (RECT_SIZE_Y / 2));
	tft.setTextDatum(TC_DATUM);
	tft.drawString("off", (RECT_SIZE_X / 2) + 2 * RECT_SIZE_X + 54, TFT_RES_Y - (RECT_SIZE_Y / 2));
	// Print Screen test button
	tft.setTextColor(TFT_WHITE, TFT_BLUE);
	tft.fillRoundRect(TFT_RES_X - RECT_SIZE_X, TFT_RES_Y - RECT_SIZE_Y, RECT_SIZE_X, RECT_SIZE_Y, 3, TFT_BLUE);
	tft.setTextDatum(BC_DATUM);
	tft.drawString("Screen", TFT_RES_X - (RECT_SIZE_X / 2), TFT_RES_Y - (RECT_SIZE_Y / 2));
	tft.setTextDatum(TC_DATUM);
	tft.drawString("test", TFT_RES_X - (RECT_SIZE_X / 2), TFT_RES_Y - (RECT_SIZE_Y / 2));

	tft.setTextSize(1);
	tft.setTextFont(2);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextDatum(TL_DATUM);
	tft.drawString("X: ", 3, 0);
	tft.drawString("Y: ", 3, 16);
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
	// touchScreen();
	// refreshColorMatrix();

	stepper.setMaxSpeed(100);
	stepper.setAcceleration(30);
	stepper.moveTo(52);
}

void loop()
{
	Screen *new_screen = nullptr;
	stepper.run();

	if (ts.touched()) {
		TS_Point p = ts.getPoint();
		if (!isTouching) {
			new_screen = screen->touch(p.x, p.y);
			// If at the end of travel go to the other end
			if (stepper.distanceToGo() == 0)
			stepper.moveTo(-stepper.currentPosition());
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
