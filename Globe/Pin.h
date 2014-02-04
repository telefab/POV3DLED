/**
 * Utility to simplify the use of dynamic
 * input and output PINS
 */
#ifndef PORTS_H
#define PORTS_H

#include <avr/io.h>

 // Type of a pin
 #define PIN_TYPE Pin

 // Shortcuts to name the pins
 #define PIN_A(x) Pin(&PORTA, &PINA, &DDRA, (x))
 #define PIN_B(x) Pin(&PORTB, &PINB, &DDRB, (x))
 #define PIN_C(x) Pin(&PORTC, &PINC, &DDRC, (x))
 #define PIN_D(x) Pin(&PORTD, &PIND, &DDRD, (x))
 #define PIN_E(x) Pin(&PORTE, &PINE, &DDRE, (x))
 #define PIN_F(x) Pin(&PORTF, &PINF, &DDRF, (x))
 #define PIN_G(x) Pin(&PORTG, &PING, &DDRG, (x))
 #define PIN_H(x) Pin(&PORTH, &PINH, &DDRH, (x))
 #define PIN_J(x) Pin(&PORTJ, &PINJ, &DDRJ, (x))
 #define PIN_K(x) Pin(&PORTK, &PINK, &DDRK, (x))
 #define PIN_L(x) Pin(&PORTL, &PINL, &DDRL, (x))

class Pin {
 	private:
 		// Port PORT register
 		volatile uint8_t* port;
 		// Port PIN register
		volatile uint8_t* pin;
		// Port DDR register
		volatile uint8_t* ddr;
		// Index of the PIN
		uint8_t index;

	public:

		/**
		 * Constructor: port and pin
		 */
		Pin(volatile uint8_t* port, volatile uint8_t* pin, volatile uint8_t* ddr, uint8_t index);

		/**
		 * Set the value of a PIN.
		 * Can also be used to set the pullup on an input
		 */
		void set(uint8_t value) const;

		/**
		 * Get the value of a PIN
		 */
		uint8_t get() const;

		/**
		 * Make a PIN an output or input
		 */
		void makeOutput(uint8_t value) const;
};

#endif