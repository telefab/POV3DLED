#include "Debug.h"

Debug* Debug::singleton = 0;

Debug::Debug() :
  values(0),
  storedValues(0)
{
}

Debug* Debug::get() {
  if (!Debug::singleton)
    Debug::singleton = new Debug();
  return Debug::singleton;
}

void Debug::begin() {
  Serial.begin(9600);
  // Initialize the number of stored values
 getStoredValuesEEPROM();
  if (storedValues > EEPROM_VALUE_SLOTS) {
    storedValues = 0;
    setStoredValuesEEPROM();
  }
  // Synchronize local data
  syncWithEEPROM();
}

void Debug::log(uint16_t value) {
  // Check available space
  if (values >= EEPROM_VALUE_SLOTS)
    return;
  // Write
  localData[values] = value;
  values++;
}

void Debug::listen() {
  // Synchronize local data with EEPROM
  syncWithEEPROM();
  // Listen to commands
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'd') {
      // Dump debug data to serial output
      Serial.print(values);
      Serial.println(" debug values in EEPROM:");
      for (uint8_t i = 0; i < values; i++)
        Serial.println(localData[i]);
    } else if (command == 's') {
      // Output data as a string
      Serial.print(values);
      Serial.println(" debug characters in EEPROM:");
      for (uint8_t i = 0; i < values; i++) {
        char letter = localData[i];
        Serial.print(letter);
      }
      Serial.println();
    } else if (command == 'c') {
      // Clear debug data
      values = 0;
      storedValues = 0;
      setStoredValuesEEPROM();
      Serial.println("Cleared debug values in EEPROM.");
    }
  }
}

void Debug::syncWithEEPROM() {
  if (storedValues == values)
    return;
  // Write to EEPROM if needed
  uint8_t address;
  uint16_t value;
  while (storedValues < values) {
    address = storedValues;
    value = localData[address];
    EEPROM.write((address+1) * 2, value & 0xFF);
    EEPROM.write((address+1) * 2 + 1, (value >> 8) & 0xFF);
    storedValues++;
  }
  // Read from EEPROM if needed
  uint16_t lsb, msb;
  while (values < storedValues) {
    address = values;
    lsb = EEPROM.read((address+1) * 2);
    msb = EEPROM.read((address+1) * 2 + 1);
    localData[address] =  (msb << 8) | lsb; 
    values++;
  }
  // Synchronize the number of stored values
  setStoredValuesEEPROM();
}

void Debug::setStoredValuesEEPROM() {
  EEPROM.write(0, storedValues);
}

void Debug::getStoredValuesEEPROM() {
  storedValues = EEPROM.read(0);
}