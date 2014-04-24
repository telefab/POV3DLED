/**
 * Sender on the communication bus
 */

#ifndef BUS_MASTER
#define BUS_MASTER

#include "Pin.h"
#include "BusParams.h"

class BusMaster {
  
private:

  /**
   * The bus master is a singleton
   */
  static BusMaster* singleton;

  /**
   * PIN number to control the bus.
   * 1 sends a 0 (connected to ground).
   * 0 sends a 1 (5V pull-up).
   */
  const static PIN_TYPE busPin;
  
  /**
   * 1 if symbols are currently being sent
   */
  uint8_t sending;
  
  /**
   * Empty private constructor: only one bus master
   */
  BusMaster();
  
  /**
   * Send one pulse on the bus (blocking).
   */
  void pulse();
  
  /**
   * Send one symbol (blocking).
   * @param symbol can be 0 or 1 (bit value) or 2 (end of message)
   */
  void sendSymbol(uint8_t symbol);
  
public:
  
  /**
   * Get the singleton master.
   * It will be created if it does not exist
   */
  static BusMaster* get();
  
  /**
   * Initializes the PIN used for the bus
   */
  void begin();
  
  /**
   * Send data through the bus (blocking).
   * @param data Data to send
   * @param bitLength Length of the data to send in bits.
   * If set to 0, data will be considered as a string and all characters
   * will be sent, including the end character.
   */
  void send(const char data[], uint16_t bitLength = 0);
  
  /**
   * Send a string
   * With the ability to add redundancy
   * @param data String to send
   * @param redundancy Level of redundancy: 0 => no redundancy
   */
  void sendString(const char data[], uint8_t redundancy = 0);
};

#endif