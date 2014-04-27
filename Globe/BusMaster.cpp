#include "BusMaster.h"
#include <Arduino.h>

BusMaster* BusMaster::singleton = 0;

// Pin A0 on the UNO
const PIN_TYPE BusMaster::busPin = PIN_C(0);

BusMaster::BusMaster() :
  inByte(0),
  byteIndex(0)
{
}

BusMaster* BusMaster::get() {
  if (!BusMaster::singleton)
    BusMaster::singleton = new BusMaster();
  return BusMaster::singleton;
}

void BusMaster::begin() {
  // Prepare the bus PIN, set to 1 by default
  busPin.makeOutput(1);
  busPin.set(1);
}

void BusMaster::pulse() {
  busPin.set(0);
  delayMicroseconds(BUS_PULSE);
  busPin.set(1);
}

void BusMaster::sendSymbol(uint8_t symbol) {
  // Send the first pulse if not already in a communication
  if (!inByte) {
    inByte = 1;
    pulse();
  }
  // Wait the right delay (-10 us to account for the processing time)
  delayMicroseconds(BUS_SYMBOL_BASE * (symbol+1) - BUS_PULSE - 10);
  // Send the second pulse if it is not an end symbol
  if (symbol != BUS_SYM_END) {
    pulse();
  } else {
    inByte = 0;
  }
}

void BusMaster::send(const char data[], uint16_t length) {
  if (length == 0)
    length = strlen(data) + 1;
  // Send each byte 
  uint8_t curRepeat, curBit, curData, control, parity = 0;
  for (uint16_t i = 0; i < length; i++) {
    // Each byte is repeated to ensure it is received
    for (curRepeat = 0; curRepeat < BUS_BYTE_REPEAT; curRepeat++) {
      // Send data
      for (curBit = 0; curBit < 7; curBit++) {
        curData = (data[i] >> curBit) & 1;
        parity^= curData;
        sendSymbol(curData);
      }
      // Send control
      control = byteIndex;
      for (curBit = 0; curBit < BUS_BYTE_INDEX_BITS; curBit++) {
        curData = (control >> curBit) & 1;
        parity^= curData;
        sendSymbol(curData);
      }
      // Send a parity bit
      sendSymbol(parity);
      // Send an end symbol
      sendSymbol(BUS_SYM_END);
    }
    // Take the sent byte into account
    byteIndex++;
    if (byteIndex >= (1 << BUS_BYTE_INDEX_BITS))
      byteIndex = 0;
  }
}