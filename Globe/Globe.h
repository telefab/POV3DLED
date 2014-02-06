/**
 * Class to control the globe display.
 * Only one globe can be managed at a time.
 */
#ifndef GLOBE_H
#define GLOBE_H

#include "Pin.h"

/**
 * Size of the globe
 */
#define GLOBE_COLUMNS 80
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
     * Is it the first detected round?
     * Important to determine the speed
     */
    volatile uint8_t firstRound;
    
    /**
     * If a round is done, pause the display
     */
    volatile uint8_t paused;

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
     * Display the next LED.
         * Return 0 if paused, 1 otherwise
     */
    uint8_t _displayLedNxt();
        
    /**
     * To call on a new round to check
     * if it is the first
     */
    uint8_t _isFirstRound();
    
    /**
     * Number of columns
     */
    uint8_t getWidth();
    
    /**
     * Number of LEDs
     */
    uint8_t getHeight();
    
    /**
     * Set the color of a LED in the buffer
     */
    void setLed(uint8_t column, uint8_t line, uint8_t color);

};

#endif