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
  const static Pin ledPins[GLOBE_LEDS];

  /**
   * PIN numbers to choose colors
   * 0 means the color is on
   */
  const static Pin colorPins[GLOBE_COLORS];

  /**
   * PIN number of the round sensor
   * 1 -> 0 -> 1 once per round
   */
  const static Pin sensorPin;

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
   * Rotated column to display.
   * This is a way to rotate the image
   * without changing the buffer.
   */
   volatile uint8_t columnRotNxt;
      
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
   * Utility made to wait for a certain number of rounds.
   * If set to a value > 0, it will be decremented each round.
   */
  volatile uint16_t remRounds;

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
   * and starts the display.
   * Something will be visible only after one round.
   */
  void begin();

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
  
  /**
   * Get the color of a LED in the buffer
   */
  uint8_t getLed(uint8_t column, uint8_t line);

  /**
   * Rotate the image in an efficient way.
   * The buffer is not changed, just read from a different start.
   * Positive value: direction of the globe rotation.
   * Negative value: inverted direction.
   */
  void rotate(int16_t steps);

  /**
   * Reset the rotation.
   */
  void clearRotation();

  /**
   * Return the current rotation.
   * The value is never negative or over GLOBE_COLUMNS-1
   */
  uint8_t getRotation();

  /**
   * Blocks during a certain number of rounds.
   * One by default.
   */
  void delayRound(uint16_t rounds = 1);

};

#endif