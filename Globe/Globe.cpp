#include "Globe.h"
#include <avr/interrupt.h>

#define START_TIMER3 TCCR3B |= (1 << WGM12) | (1 << CS11) | (1 << CS10)
#define STOP_TIMER3 TCCR3B &= ~((1 << WGM12) | (1 << CS11) | (1 << CS10))

Globe* Globe::singleton = 0;

const PIN_TYPE Globe::ledPins[GLOBE_LEDS] = {PIN_F(2), PIN_F(3), PIN_F(4), PIN_F(5), PIN_F(6), PIN_F(7), PIN_K(0), PIN_K(1), PIN_K(2), PIN_B(1), PIN_B(0),
  PIN_B(3), PIN_B(2), PIN_L(1), PIN_L(0), PIN_L(3), PIN_L(2), PIN_L(5), PIN_L(4), PIN_L(7), PIN_L(6), PIN_G(1), PIN_G(0), PIN_D(7), PIN_G(2), 
  PIN_C(1), PIN_C(0), PIN_C(3), PIN_C(2), PIN_C(5), PIN_C(4), PIN_C(7), PIN_C(6), PIN_A(6), PIN_A(7), PIN_A(4), PIN_A(5), PIN_A(2), PIN_A(3)};

const PIN_TYPE Globe::colorPins[GLOBE_COLORS] = {PIN_A(0), PIN_F(0), PIN_A(1)};

const PIN_TYPE Globe::sensorPin = PIN_E(4);

Globe::Globe() :
  columnCur(GLOBE_COLUMNS-1),
  ledCur(GLOBE_LEDS-1),
  columnNxt(0),
  ledNxt(0),
  columnRotNxt(0),
  firstRound(1),
  paused(1),
  remRounds(0)
{
}

Globe* Globe::get() {
  if (!Globe::singleton)
    Globe::singleton = new Globe();
  return Globe::singleton;
}

void Globe::begin() {
  uint8_t i, j;
  // Reset the globe buffer to all black
  for (i = 0; i < GLOBE_COLUMNS; i++)
    for (j = 0; j < GLOBE_LEDS; j++)
      imageBuffer[i][j] = 0;
  // LED controls are outputs
  for (i = 0; i < GLOBE_LEDS; i++)
    ledPins[i].makeOutput(1);
  for (i = 0; i < GLOBE_COLORS; i++)
    colorPins[i].makeOutput(1);
  // Round sensor is an input with a pull-up resistor
  sensorPin.makeOutput(0);
  sensorPin.set(1);
  // Disable global interrupts
  cli();
  // Initialize the round timer (Timer1, /256, counting)
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12);
  // Initialize the LED timer (Timer3, /64, compare interrupt, not started yet)
  TCCR3A = 0;
  TCCR3B = 0;
  TIMSK3 |= (1 << OCIE1A);
  // Activate round sensor interrupts (INT4, falling)
  EIMSK |= (1 << INT4);
  EICRB |= (1 << ISC41); 
  // enable global interrupts
  sei();
}

void Globe::_restartRound() {
  // Restart the display
  // Keep the rotation, to do before resetting anything
  columnRotNxt = getRotation();
  columnNxt = 0;
  ledNxt = 0;
  paused = 0;
  // Decrease the rounds delay if any
  if (remRounds > 0)
    remRounds--;
}

uint8_t Globe::_displayLedNxt() {
  uint8_t i;
  // Switch the previous led off
  ledPins[ledCur].set(0);
  // Pause after switching the LED off if done
  if (paused)
    return 0;
  // Select the right colors
  for (i = 0; i < GLOBE_COLORS; i++)
    colorPins[i].set(((~imageBuffer[columnRotNxt][ledNxt]) >> i) & 1);
  // Switch the proper led on
  ledPins[ledNxt].set(1);
  // Save the new state
  columnCur = columnNxt;
  ledCur = ledNxt;
  // Select the next LED to display
  ledNxt = ledCur + 1;
  columnNxt = columnCur;
  if (ledNxt >= GLOBE_LEDS) {
    ledNxt = 0;
    columnNxt+= 1;
    if (columnNxt >= GLOBE_COLUMNS) {
      columnNxt = 0;
      // Finished the round, pause
      paused = 1;
    }
    // Precompute the rotated column to display
    columnRotNxt+= 1;
    if (columnRotNxt >= GLOBE_COLUMNS)
      columnRotNxt = 0;
  }
  return 1;
}

uint8_t Globe::_isFirstRound() {
  uint8_t result = firstRound;
  // Not the first round any more
  firstRound = 0;
  return result;
}

uint8_t Globe::getWidth() {
  return GLOBE_COLUMNS;
}

uint8_t Globe::getHeight() {
  return GLOBE_LEDS;
}

void Globe::setLed(uint8_t column, uint8_t line, uint8_t color) {
  imageBuffer[column][line] = color;
}

uint8_t Globe::getLed(uint8_t column, uint8_t line) {
  return imageBuffer[column][line];
}

void Globe::rotate(int16_t steps) {
  // The rotated column must stay in the buffer width
  if (steps > columnRotNxt)
    columnRotNxt-= steps - GLOBE_COLUMNS;
  else {
    columnRotNxt-= steps;
    if (columnRotNxt >= GLOBE_COLUMNS)
      columnRotNxt-= GLOBE_COLUMNS;
  }
}

void Globe::clearRotation() {
  // Displayed column is the actual column
  columnRotNxt = columnNxt;
}

uint8_t Globe::getRotation() {
  if (columnRotNxt >= columnNxt)
    return columnRotNxt - columnNxt;
  else
    return GLOBE_COLUMNS - columnNxt + columnRotNxt;
}

void Globe::delayRound(uint16_t rounds) {
  // Actively wait for the rounds to get to 0
  remRounds = rounds;
  while (remRounds > 0) {}
}

/**
 * Called at each round by
 * the round sensor to synchronize the display
 */
ISR (INT4_vect)
{
  // Set the measured delay as LED timer delay and reset
  // The 8 factor is due to the different timer speeds
  // The -1 is an adjustment parameter
  // to make sure that everything is displayed
  OCR3A = TCNT1 / (GLOBE_LEDS * GLOBE_COLUMNS / 4) - 1;
  TCNT1 = 0;
  // Start the round again
  // not at first round because the speed of the globe is unknown
  if (!Globe::get()->_isFirstRound()) {
    Globe::get()->_restartRound();
    START_TIMER3;
  }
}

/**
 * Called by a timer once for each LED to display,
 * at a variable speed depending on the round sensor
 */
ISR(TIMER3_COMPA_vect) {
  if (!Globe::get()->_displayLedNxt()) {
    // Stop the timer if paused
    STOP_TIMER3;
    TCNT3 = 0;
  }
}
