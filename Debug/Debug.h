/**
 * Tool to debug when the Arduino
 * cannot always be connected to Serial.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <EEPROM.h>

/**
 * Number of 8-bit slots in EEPROM
 */
#define EEPROM_SLOTS 512

/**
 * Number of slots to store values using Debug
 */
#define EEPROM_VALUE_SLOTS (EEPROM_SLOTS/2-1)

class Debug {
  
private:

  /**
   * The debugger is a singleton
   */
  static Debug* singleton;
  
  /**
   * Local temporary storage
   */
  volatile uint16_t localData[EEPROM_VALUE_SLOTS];
  
  /**
   * Current number of values stored locally
   */
  volatile uint8_t values;
  
  /**
   * Current number of values stored in the EEPROM
   */
  uint8_t storedValues;
  
  /**
   * Empty private constructor: only one debugger
   */
  Debug();
  
  /**
   * Synchronize local information with EEPROM information
   */
  void syncWithEEPROM();
  
  /**
   * Write the number of stored values to EEPROM
   */
  void setStoredValuesEEPROM();
  
  /**
   * Get the number of stored values from EEPROM
   */
  void getStoredValuesEEPROM();
  
public:
  
  /**
   * Get the singleton debugger.
   * It will be created if it does not exist
   */
  static Debug* get();
  
  /**
   * Initializes the debugger
   */
  void begin();
  
  /**
   * Log a value in the debugger
   * @param value The value to log
   */
  void log(uint16_t value);
  
  /**
   * Listen to serial to know when to
   * dump or clear debug data
   */
  void listen();
};

#endif