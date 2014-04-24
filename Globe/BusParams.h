/**
 * Parameters of the bus. All times in us.
 * These delays are long because the receiving Arduino is overloaded, which reduces its ccuracy.
 */

/**
 * Duration of a pulse
 */
#define BUS_PULSE 20

/**
 * Basic duration of a symbol, multiplied for each different symbol
 */
#define BUS_SYMBOL_BASE 200

/**
 * Symbol: end of message
 */
#define BUS_SYM_END 2

/**
 * Size (in bytes) of the buffer to store data when receiving
 */
#define BUS_FIFO_SIZE 120