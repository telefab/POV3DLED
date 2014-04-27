#if defined(__AVR_ATmega2560__)

#include "BusSlave.h"
#include <avr/interrupt.h>

BusSlave* BusSlave::singleton = 0;

// Pin 3 on the Arduino MEGA (INT5)
const PIN_TYPE BusSlave::busPin = PIN_E(5);

BusSlave::BusSlave() :
  dataReadPtr(0),
  dataWritePtr(0),
  availableBytes(0),
  currentBit(0),
  currentParity(0),
  currentByteIndex(0),
  lastByteIndex(0xFF),
  lastPulseDate(0),
  lastPulseDateSet(0),
  stringCharIndex(0)
{
}

BusSlave* BusSlave::get() {
  if (!BusSlave::singleton)
    BusSlave::singleton = new BusSlave();
  return BusSlave::singleton;
}

void BusSlave::begin() {
  // Prepare the bus PIN
  busPin.makeOutput(0);
  // Disable global interrupts
  cli();
  // Initialize the bus delay timer (Timer4, /8, 0.5us, compare interrupt when symbol is finished)
  // Not using ICP because interrupts have an overly high priority
  // and prevent the globe from working.
  TCCR4A = 0;
  TCCR4B = 0;
  TIMSK4 = 0;
  TCCR4B |= (1<<ICES4) | (1 << CS41);
  TIMSK4 |= (1 << OCIE4A);
  OCR4A = 0xFFFF;
  // Activate bus interrupts (INT5, rising)
  EIMSK |= (1 << INT5);
  EICRB |= (1 << ISC50) | (1 << ISC51); 
  // enable global interrupts
  sei();
}

uint16_t BusSlave::available() {
  return availableBytes;
}

char BusSlave::read() {
  if (availableBytes == 0)
    return 0;
  char result = dataFifo[dataReadPtr];
  availableBytes--;
  dataReadPtr++;
  if (dataReadPtr >= BUS_FIFO_SIZE)
    dataReadPtr = 0;
  return result;
}

uint8_t BusSlave::readString(char data[], uint16_t maxSize) {
  if (available() > 0) {
    char curByte = read();
    // Store the character if possible
    if (stringCharIndex < (maxSize-1)) {
      data[stringCharIndex] = curByte;
      stringCharIndex++;
    }
    if (curByte == 0 || stringCharIndex >= maxSize-1) {
      data[maxSize-1] = 0;
      stringCharIndex = 0;
      return 1;
    }
  }
  return 0;
}

void BusSlave::saveSymbol(uint8_t symbol) {
  // Ignore received symbols if no space to store them
  if (availableBytes >= BUS_FIFO_SIZE)
    return;
  if (symbol < BUS_SYM_END) {
    // Received a normal bit symbol
    if (currentBit == 0) {
      // Initialize the byte data
      dataFifo[dataWritePtr] = 0;
      currentParity = 0;
      currentByteIndex = 0;
    }
    if (currentBit <= 7) {
      // Save a data bit
      dataFifo[dataWritePtr] |= (symbol << currentBit);
      currentParity^= symbol;
      currentBit++;
    } else if (currentBit <= 7 + BUS_BYTE_INDEX_BITS) {
      // Save an index bit
      currentByteIndex |= (symbol << (currentBit - 7));
      currentParity^= symbol;
      currentBit++;
    } else if (currentBit == 7 + BUS_BYTE_INDEX_BITS + 1) {
      // Parity check bit
      currentParity^= symbol;
      currentBit = 7 + BUS_BYTE_INDEX_BITS + 2;
    } else {
      // Ignore, too many bits were received
      currentBit = 7 + BUS_BYTE_INDEX_BITS + 3;
    }
  } else {
    // Received an end of byte
    // Ignore the byte if it is not the good length or the parity is wrong
    if (currentBit == 7 + BUS_BYTE_INDEX_BITS + 2 && currentParity == 0) {
      // Check that the byte has not already been received
      if (currentByteIndex != lastByteIndex) {
        // Make the byte available
        availableBytes++;
        dataWritePtr++;
        if (dataWritePtr >= BUS_FIFO_SIZE)
          dataWritePtr = 0;
        lastByteIndex = currentByteIndex;
      }
    }
    // Get ready for the next byte
    currentBit = 0;
  }
}

uint16_t BusSlave::_measurePulseDelay(uint16_t date, uint8_t &first, uint8_t &noise) {
  if (lastPulseDateSet == 0) {
    // First pulse of a communication
    lastPulseDate = date;
    lastPulseDateSet = 1;
    first = 1;
    noise = 0;
    return 0;
  }
  first = 0;
  // Measure the delay
  uint16_t delay = date - lastPulseDate;
  if (delay >= BUS_SYMBOL_BASE) {
    lastPulseDate = date;
    noise = 0;
  } else {
    // Pulses too close: this is noise
    // Long pulses sent are received as a large number of very short pulses
    noise = 1;
  }
  return delay;
}

void BusSlave::_onReceivedPulse(uint16_t delay) {
  // Interpret the symbol and save
  uint8_t symbol = (delay - (BUS_SYMBOL_BASE+1)) / (BUS_SYMBOL_BASE*2);
  saveSymbol(symbol);
}

void BusSlave::_onSymbolTimeout() {
  if (lastPulseDateSet) {
    lastPulseDateSet = 0;
    saveSymbol(BUS_SYM_END);
  }
}

uint16_t _dateBusSlave, _delayBusSlave;
uint8_t _firstBusSlave, _noiseBusSlave;
BusSlave* _instBusSlave = BusSlave::get();

/**
 * Called at each bus rising edge
 */
ISR (INT5_vect)
{
  // Measure the delay
  _dateBusSlave = TCNT4;
  _delayBusSlave = _instBusSlave->_measurePulseDelay(_dateBusSlave, _firstBusSlave, _noiseBusSlave);
  if (!_noiseBusSlave) {
    // Reset the timer compare to detect an end pulse
    OCR4A = _dateBusSlave + (BUS_SYMBOL_BASE*(BUS_SYM_END+1)+BUS_SYMBOL_BASE/2)*2;
    // Analyze the pulse
    if (!_firstBusSlave)
      _instBusSlave->_onReceivedPulse(_delayBusSlave);
  }
}

/**
 * Called by the timer if a symbol is done
 */
ISR(TIMER4_COMPA_vect) {
  // Symbol finished
  _instBusSlave->_onSymbolTimeout();
}

#endif