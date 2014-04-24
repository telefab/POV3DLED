#include "BusMaster.h"
#include <Arduino.h>

BusMaster* BusMaster::singleton = 0;

// Pin A0 on the UNO
const PIN_TYPE BusMaster::busPin = PIN_C(0);

BusMaster::BusMaster() :
  sending(0)
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
  if (!sending) {
    sending = 1;
    pulse();
  }
  // Wait the right delay (-10 us to account for the processing time)
  delayMicroseconds(BUS_SYMBOL_BASE * (symbol+1) - BUS_PULSE - 10);
  // Send the second pulse if it is not an end symbol
  if (symbol != BUS_SYM_END) {
    pulse();
  } else {
    sending = 0;
  }
}

void BusMaster::send(const char data[], uint16_t bitLength) {
  if (bitLength == 0)
    bitLength = (strlen(data) + 1) << 3;
  // Send each bit 
  uint8_t curBit;
  for (uint16_t i = 0; i < bitLength; i++) {
    // Send an end symbol after each byte
    if (i > 0 && (i & 7) == 0)
      sendSymbol(BUS_SYM_END);
    // Send the bit
    curBit = (data[i >> 3] >> (i & 7)) & 1;
    sendSymbol(curBit);
  }
  // Send end of message
  sendSymbol(BUS_SYM_END);
}

void BusMaster::sendString(const char data[], uint8_t redundancy) {
  // Send the string multiple times
  for (int i = 0; i <= redundancy; i++) {
    send(data);
    // If there is redundancy, send a flag indicating the end of this string
    if (redundancy > 0) {
      for (int i = 0; i <= 2*(redundancy+1); i++)
        send("");
    }
  }
}