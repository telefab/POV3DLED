/**
 * Class to control the globe display.
 * Only one globe can be managed at a time.
 * The class is a singleton.
 */
#ifndef GLOBE_H
#define GLOBE_H

#include "Pin.h"

/**
 * Size of the globe
 */
#define GLOBE_COLUMNS 100
#define GLOBE_LEDS 39
#define GLOBE_COLORS 3


class Globe {

	private:

		/**
		 * The globe is a singleton
		 */
		static Globe* singleton;

		/**
		 * PIN numbers to select a LED
		 * 1 means selected
		 */
		const static PIN_TYPE ledPins[GLOBE_LEDS];

		/**
		 * PIN numbers to choose colors
		 * 0 means the color is on
		 */
		const static PIN_TYPE colorPins[GLOBE_COLORS];

		/**
		 * PIN number of the round sensor
		 * 1 -> 0 -> 1 once per round
		 */
		const static PIN_TYPE sensorPin;

		/**
		 * Buffer to store the image to display.
		 * Any change is reflected immediately
		 */
		uint8_t imageBuffer[GLOBE_COLUMNS][GLOBE_LEDS];

		/**
		 * Current state: last displayed LED
		 */
		volatile uint8_t columnCur;
		volatile uint8_t ledCur;

		/**
		 * Next state: LED to display
		 */
		volatile uint8_t columnNxt;
		volatile uint8_t ledNxt;

		/**
		 * Empty private constructor: unique
		 */
		Globe();

	public:

		/**
		 * Get the singleton globe.
		 * It will be created if it does not exist
		 */
		static Globe* get();

		/**
		 * Initializes the PINs used to control the globe
		 * and starts the display
		 */
		void setup();

		/**
		 * Get ready to restart the round at next start
		 */
		void _restartRound();

		/**
		 * Display the next LED
		 */
		void _displayLedNxt();

};

#endif