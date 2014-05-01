/**
 * Receiver on the communication bus
 */

#ifndef BUS_SLAVE
#define BUS_SLAVE

#include "Pin.h"
#include "BusParams.h"

class BusSlave {
  
private:

  /**
   * The bus slave is a singleton
   */
  static BusSlave* singleton;

  /**
   * PIN number to receive data
   */
  const static PIN_TYPE busPin;
  
  /**
   * FIFO storing received data
   */
  volatile uint8_t dataFifo[BUS_FIFO_SIZE];
  
  /**
   * Pointer to the next byte to be read
   */
  uint16_t dataReadPtr;
  
  /**
   * Pointer to the next byte to be written
   */
  volatile uint16_t dataWritePtr;
  
  /**
   * Number of bytes stored in the FIFO
   */
  volatile uint16_t availableBytes;
  
  /**
   * Current index of the bit to be received
   */
  volatile uint8_t currentBit;
  
  /**
   * Parity computed for last received bits in this byte
   */
  volatile uint8_t currentParity;
  
  /**
   * Current index of the byte received (read in received data)
   */
  volatile uint8_t currentByteIndex;
  
  /**
   * Index of the last received byte
   */
  volatile uint8_t lastByteIndex;
  
  /**
   * Date at which the last pulse of the ongoing
   * conversation was received
   */
  volatile uint16_t lastPulseDate;
  
  /**
   * Is there a date of a pulse in the ongoing conversation?
   */
  volatile uint8_t lastPulseDateSet;
  
  /**
   * Index of the char in the string received by readString
   */
  uint16_t stringCharIndex;
  
  /**
   * Empty private constructor: only one bus slave
   */
  BusSlave();
  
  /**
   * A symbol has been received:
   * add it to received data.
   * @param symbol Received symbol
   */
  void saveSymbol(uint8_t symbol);
  
public:
  
  /**
   * Get the singleton slave.
   * It will be created if it does not exist
   */
  static BusSlave* get();
  
  /**
   * Initializes the bus
   */
  void begin();
  
  /**
   * Data availability check
   * @return the number of available bytes
   */
  uint16_t available();
  
  /**
   * Read one byte of data
   * @return data as a byte
   */
  char read();
  
  /**
   * Read a string
   * @param data should be a string of size maxSize, will contain received string
   * @param maxSize Maximum size of the received string
   * @return if the string is ready to use
   */
  uint8_t readString(char data[], uint16_t maxSize);
  
  /**
   * Measure a pulse delay
   * @param date The date of the received pulse
   * @param first Will contain 1 if the pulse is the first of a communication, 0 otherwise
   * @param noise Will contain 1 if the pulse is only noise, 0 otherwise
   * @return The delay from the 
   */
  uint16_t _measurePulseDelay(uint16_t date, uint8_t &first, uint8_t &noise);
  
  /**
   * To call when a pulse has been received
   * @param delay The delay between the two pulses of the symbol
   */
  void _onReceivedPulse(uint16_t delay);
  
  /**
   * To call when a received symbol has timed
   * out: one full symbol has been received.
   */
  void _onSymbolTimeout();
};

#endif