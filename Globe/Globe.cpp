#include "Globe.h"
#include <avr/interrupt.h>

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
	ledNxt(0)
{
}

Globe* Globe::get() {
	if (!Globe::singleton)
		Globe::singleton = new Globe();
	return Globe::singleton;
}

void Globe::setup() {
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
	// Initialize the LED timer (Timer3, /64, compare interrupt)
    TCCR3A = 0;
    TCCR3B = 0;
    TCCR3B |= (1 << WGM12);
    TCCR3B |= (1 << CS11) | (1 << CS10);
    TIMSK3 |= (1 << OCIE1A);
    OCR3A = 25;
    // Activate round sensor interrupts (INT4, falling)
    EIMSK |= (1 << INT4);
    EICRB |= (1 << ISC41); 
    // enable global interrupts
    sei();
}

void Globe::_restartRound() {
	columnNxt = 0;
	ledNxt = 0;
}

void Globe::_displayLedNxt() {
	uint8_t i;
	// Switch the previous led off
	ledPins[ledCur].set(0);
	// Select the right colors
	for (i = 0; i < GLOBE_COLORS; i++)
		colorPins[i].set((imageBuffer[columnNxt][ledNxt] & (1 << i)) == 0 ? 1 : 0);
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
		if (columnNxt >= GLOBE_COLUMNS)
			columnNxt = 0;
	}
}

/**
 * Called at each round by
 * the round sensor to synchronize the display
 */
ISR (INT4_vect)
{
    // Set the measured delay as LED timer delay and reset
    // The 8 factor is due to the different timer speeds
    OCR3A = TCNT1 / (GLOBE_LEDS * GLOBE_COLUMNS / 8);
    TCNT1 = 0;
    // Start the round again
    Globe::get()->_restartRound();
}

/**
 * Called by a timer once for each LED to display,
 * at a variable speed depending on the round sensor
 */
ISR(TIMER3_COMPA_vect) {
	Globe::get()->_displayLedNxt();
}
